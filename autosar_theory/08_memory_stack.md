# 08. 메모리 스택

비휘발성 데이터(보정값·학습값·고장 기록·주행거리 등)를 플래시/EEPROM 에 안전하게
저장·복원하는 모듈군. "전원이 꺼져도 살아남아야 하는 데이터"는 전부 이 스택을 통한다.
견고성(power-fail safe)과 마모 관리가 이 스택의 존재 이유다.

## 추상화 체인

```
SWC / 다른 BSW (예: Dem 의 event memory)
 │  Rte_Call_<Nv...> / NvM_WriteBlock(BlockId, src)
 ▼
NvM            블록 관리: 큐잉, 우선순위, redundancy, CRC, 기본값 복구, write 보호
 │  MemIf_Write/Read
 ▼
MemIf          여러 메모리 추상화를 하나로 묶는 얇은 디스패처 (device index 로 분기)
 │
 ├─ Fee ── Fls(MCAL) ── 내장 플래시
 └─ Ea  ── Eep(MCAL) ── 외부 EEPROM
```

각 층은 위가 "논리 블록"만 보게 하고, 아래로 갈수록 물리 제약(섹터/페이지/마모)을 흡수한다.

## NvM (NVRAM Manager) — 상위가 보는 유일한 창구

상위는 "블록(block)"이라는 논리 단위로만 read/write 한다. NvM 은 요청을 **큐**에 넣고
`NvM_MainFunction` 에서 **비동기**로 처리한다(즉시 끝나지 않음; 완료는 폴링/콜백).

### RAM 미러 / NV 블록 / ROM 기본값
각 블록은 보통 셋과 연관된다:
- **RAM Block (미러)**: 런타임에 SWC 가 읽고 쓰는 RAM 상의 복사본.
- **NV Block**: 실제 비휘발성 저장본(Fee/Ea 통해).
- **ROM Block (기본값)**: 공장 초기값. NV 가 손상되면 여기서 복구.

흐름: 부팅 시 NV→RAM 복원, 런타임엔 RAM 미러 갱신, 필요 시 RAM→NV 기록.

### 블록 관리 정책
- **CRC**: 블록 무결성 검증(저장 시 계산, 복원 시 확인). 손상 감지.
- **Redundancy**: 같은 블록을 2벌 저장(redundant block). 한 벌 손상 시 다른 벌로 복구.
- **Dataset**: 한 블록에 여러 데이터셋(예: 여러 캘리브레이션 세트) 인덱싱.
- **Write protection / Immediate priority**: 쓰기 보호, 또는 긴급 즉시 기록(예: crash
  data) 우선순위.
- **Default value recovery**: CRC 실패/미초기화 시 ROM 기본값으로 RAM 채우기.

### 일괄 동작 — ReadAll / WriteAll
- `NvM_ReadAll()`: 부팅 시 EcuM/BswM 가 트리거. 설정된 모든 블록을 RAM 미러로 복원
  (CRC 검증, 실패 시 기본값). ECU 가 "지난 상태"를 회복하는 단계.
- `NvM_WriteAll()`: 종료 시 dirty 블록 일괄 flush. 안전 전원 차단 직전에 수행
  (BswM/EcuM 조율, →`09`).

## MemIf — 메모리 추상화 인터페이스

NvM 아래에서 "어느 에뮬레이션(Fee/Ea)을 쓸지"만 고르는 얇은 층. Fee 와 Ea 를 동시에
쓰는 시스템에서 device index 로 분기. 한 종류만 쓰면 거의 통과(pass-through). 의의:
NvM 이 Fee/Ea 둘 다를 알 필요 없이 단일 인터페이스만 보게 함.

## Fee (Flash EEPROM Emulation) — 플래시를 EEPROM 처럼

내장 플래시는 물리 제약이 크다: **섹터 단위 erase, 덮어쓰기 불가, 마모 한계(erase
횟수)**. Fee 는 이 제약을 숨긴다:
- 가상 블록을 섹터에 **순차 기록**하고, 같은 블록을 다시 쓰면 새 위치에 append + 옛
  위치 무효화.
- 섹터가 차면 유효 데이터만 다음 섹터로 옮긴 뒤(=garbage collection) 옛 섹터 erase →
  **wear leveling**(마모 분산).
- 전원 단절에도 일관성을 유지(쓰기 중 끊겨도 옛 데이터가 살아있도록 설계).

> S32K144 내부 플래시에 보정값을 직접 저장해 봤다면, "erase-before-write, 섹터 단위,
> 쓰기 중 전원 단절" 문제를 손으로 처리했을 것이다. AUTOSAR 는 그 고생을 Fee 라는
> 표준 모듈로 떼어낸다.

## Ea (EEPROM Abstraction) — 외부 EEPROM 용

진짜 EEPROM 은 바이트 단위 덮어쓰기가 되므로 Fee 보다 단순하다. 그래도 블록↔주소
매핑, 일관성, (필요 시) 마모 고려를 책임진다. 하부는 `Eep`(MCAL).

## Fls / Eep (MCAL) — 실제 하드웨어

- `Fls`: 내장 플래시 — 섹터 erase, 페이지 program, read. 비동기(시간 소요)+상태 폴링.
- `Eep`: 외부 EEPROM(보통 SPI/I2C 연결) — read/write/erase.

## 전형적 시나리오 (종합)

```
부팅:    EcuM/BswM → NvM_ReadAll() → (Fee/Ea→Fls/Eep) → RAM 미러 복원
         CRC 깨진 블록은 ROM 기본값으로 복구
런타임:  SWC 가 RAM 미러 갱신 → NvM_WriteBlock → 큐 → MainFunction 이 Fee→Fls 로 기록
         (Fee 가 wear leveling/GC 수행)
종료:    BswM/EcuM → NvM_WriteAll() → dirty flush → 안전 전원 차단
```

진단 연계: `Dem`(→`07`)의 event memory(DTC/freeze frame)도 이 스택으로 영속화된다.
즉 메모리 스택은 "기능 데이터"뿐 아니라 "고장 기록"의 저장 기반이기도 하다.

## 왜 이런 구조인가 (분석)

- **물리 제약의 완전한 은닉**: SWC 는 "블록 저장/복원"만 알면 됨. erase 단위·마모·
  전원단절 견고성은 아래가 처리.
- **교체 유연성**: 내장 플래시(Fee/Fls)든 외부 EEPROM(Ea/Eep)든 MemIf 아래에서 교체
  가능. NvM 위는 불변.
- **견고성 표준화**: CRC·redundancy·기본값 복구·power-fail 안전을 표준 모듈로 제공해
  매 프로젝트가 직접 구현하지 않게.
- **비동기 모델**: 느린 NV 접근을 큐+MainFunction 으로 비차단 처리 → 실시간성 보존.

## 원문에서 확인할 것
- *Specification of NVRAM Manager* (SWS NvM) — 블록 모델·ReadAll/WriteAll·정책.
- *Specification of Memory Abstraction Interface* (SWS MemIf).
- *Specification of Flash EEPROM Emulation* (SWS Fee), *EEPROM Abstraction* (SWS Ea).
- *Specification of Flash Driver* (SWS Fls), *EEPROM Driver* (SWS Eep).
