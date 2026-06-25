# 02. Classic Platform 계층 아키텍처

## 큰 그림

Classic Platform 의 소프트웨어는 명확한 층으로 쌓인다. 핵심 의도는 **위로 갈수록
하드웨어를 모르게** 만드는 것.

```
+-----------------------------------------------------------+
|              Application Layer (SWC 들)                    |  HW 무지, 재사용 단위
+--------------------------- RTE ---------------------------+  SWC↔BSW 유일 통로 (생성됨)
+-----------------------------------------------------------+
|                  Basic Software (BSW)                     |
|  +-----------------+ +-------------+ +------------------+  |
|  | Services Layer  | | ECU Abstr.  | | Complex Drivers  |  |
|  | (System/Comm/   | | Layer       | | (CDD)            |  |
|  |  Diag/Mem/Crypto)| |             | |                  |  |
|  +-----------------+ +-------------+ +------------------+  |
|  +-----------------------------------------------------+   |
|  |  Microcontroller Abstraction Layer (MCAL)           |   |
|  +-----------------------------------------------------+   |
+-----------------------------------------------------------+
|                   Microcontroller (HW)                    |
+-----------------------------------------------------------+
```

BSW 는 크게 네 덩어리다: **Services Layer**(가장 위, 추상적 서비스), **ECU
Abstraction Layer**(드라이버를 보드 단위로 추상화), **MCAL**(레지스터 직결), 그리고
이 셋을 가로지를 수 있는 **Complex Device Drivers(CDD)**.

## 각 계층의 책임

### MCAL (Microcontroller Abstraction Layer)
레지스터를 직접 만지는 **유일한** 계층. 칩마다 다른 페리페럴을 표준 드라이버 API
뒤로 숨긴다. 상위는 `Can`, `Adc`, `Pwm` 같은 벤더 무관 API 만 본다. MCU 가 바뀌면
원칙적으로 MCAL 만 교체하면 된다.
- 드라이버 그룹: 마이크로컨트롤러(Mcu/Gpt/Wdg), 메모리(Fls/Eep), 통신(Can/Lin/Fr/
  Eth/Spi), I/O(Port/Dio/Adc/Pwm/Icu/Ocu).
- 특성: 하드웨어 인스턴스에 묶임, 비동기+상태/콜백, DET 검증.

### ECU Abstraction Layer
MCAL 위에서 "어떤 컨트롤러/디바이스를 쓰든 동일하게" 보이도록 한 겹 더 추상화한다.
- 예: `CanIf` 는 CAN 컨트롤러가 1개든 N개든, 트랜시버 종류가 무엇이든 상위엔 동일
  인터페이스를 준다.
- `IoHwAb` 는 "이 신호가 DIO 핀인지 ADC 채널인지 PWM 인지"를 상위로부터 숨긴다.
- 핵심: MCAL=하드웨어 인스턴스, ECU Abstraction=보드 레벨 논리 자원.

### Services Layer (BSW 의 가장 위)
실제 표준 서비스들이 사는, 가장 두껍고 추상적인 층. 다섯 갈래로 묶인다:
- **System Services**: OS, EcuM, BswM, SchM, WdgM, 시간(StbM/Tm). → `09`.
- **Communication Services**: Com, PduR, 버스 매니저/NM/TP, ComM, SecOC, E2E. → `06`.
- **Diagnostic Services**: Dcm, Dem, Det, FiM. → `07`.
- **Memory Services**: NvM, MemIf, Fee/Ea. → `08`.
- **Crypto Services**: Csm, CryIf, KeyM, IdsM. → `11`.
이 층은 하드웨어를 (거의) 모르고 논리적 서비스만 제공한다.

### Complex Device Drivers (CDD)
표준 BSW 로 못 담는 특수 요구(극단적 타이밍, 비표준 프로토콜, 표준화 안 된 센서)를
위한 **탈출구**. 계층을 가로질러 애플리케이션과 하드웨어를 직접 잇는 게 허용된다.
- 용도: 정밀 분사 제어처럼 마이크로초 단위 타이밍, 또는 아직 표준이 없는 신기능.
- 주의: 강력하지만 이식성/표준성을 깨므로 "정말 필요할 때만". 남용하면 AUTOSAR 의
  이점이 사라진다.

### RTE (Runtime Environment)
BSW 가 아니라 SWC 와 BSW 사이의 **배선**이다. SWC 는 오직 `Rte_*` 만 호출하고, 그
뒤가 다른 SWC 인지 Com 인지 모른다. ARXML 설정으로부터 **생성**된다. → `05`.

## 의존 방향 — 가장 중요한 규칙

핵심 규칙 하나: **호출(의존)은 위에서 아래로만 흐른다.**

```
SWC → RTE → Services → ECU Abstraction → MCAL → HW      (O)
HW/하위 → 상위를 직접 호출                                (X)
```

하위가 상위에 무언가를 알려야 할 때(수신 도착, 변환 완료 등)는 **콜백
(notification)** 을 쓴다. 즉 상위가 미리 등록해 둔 콜백 함수를 하위가 호출한다.
제어 흐름은 위→아래, 통지는 "이미 위가 정해 둔 콜백을 통해" 아래→위.

- 예: CAN 수신 시 `Can`(MCAL)이 `CanIf_RxIndication` 을, `CanIf` 가
  `PduR_RxIndication` 을, `PduR` 이 `Com_RxIndication` 을 부른다. 각 단계는 "바로
  위" 모듈의 정해진 콜백만 안다(전체 위쪽을 아는 게 아니다).

이 단방향 규칙이 계층 분리의 본질이다. 어기면(하위가 상위 헤더를 직접 include 하면)
이식성이 깨진다.

## 인터페이스의 세 종류 (계층 간 약속)

AUTOSAR 는 모듈 간 인터페이스를 셋으로 분류한다:
- **AUTOSAR Interface**: 애플리케이션 레벨(포트 기반) 인터페이스. 하드웨어 무관.
- **Standardized AUTOSAR Interface**: 표준화된 서비스 인터페이스(SWC↔BSW 서비스,
  예: NvM/Dem 을 RTE 통해 사용).
- **Standardized Interface**: BSW 모듈 간 표준 C API(예: `PduR`↔`CanIf`). API 시그니처가
  명세로 고정.

이 구분 덕에 "어디까지가 이식 가능한 추상이고, 어디부터가 고정된 C API 인지"가 명확하다.

## 왜 이렇게까지 층을 나누나 (분석)

- **교체 단위 = 모듈 경계**: MCU 교체→MCAL 만, 버스 교체→If/Driver 만, 기능 교체→SWC
  만. 변경의 파급을 모듈 경계 안에 가둔다.
- **검증 재사용**: 한 번 검증한 BSW 를 재사용해 재검증 비용을 줄인다(특히 안전 인증).
- **분업**: 층마다 다른 팀/공급사가 병렬 작업. 인터페이스만 맞추면 통합된다.

직접 S32K144 베어메탈로 짜면 이 단계들이 한 파일에 뭉친다. AUTOSAR 는 "교체·재사용·
분업이 일어날 모든 지점"에서 칼같이 모듈을 끊는다. 그 대가가 복잡도이고, 보상이
대규모·장수명 개발에서의 효율이다(→ `01` trade-off).

## 원문에서 확인할 것
- *Layered Software Architecture* (EXP) — 이 장의 1차 출처. 계층/모듈 분류 다이어그램.
- *List of Basic Software Modules* (TR) — 표준 BSW 모듈 전체 목록과 약어.
- *General Specification of Basic Software Modules* (SWS BSWGeneral) — 모든 BSW 공통 규약.
