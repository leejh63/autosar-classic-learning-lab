# 07. 진단 스택

차량 진단(정비소 스캐너, 법규 OBD, ECU 내부 고장 기록·관리)을 담당한다. 통신
스택(→`06`) 위에 얹혀, "진단 요청"이라는 특수 PDU 를 처리하고 고장 정보를 생애주기로
관리한다.

## 표준 진단 프로토콜 — UDS 와 OBD

AUTOSAR 진단은 두 표준 위에 선다:
- **UDS (Unified Diagnostic Services, ISO 14229)**: 정비/개발 진단의 핵심. 서비스
  ID 기반 요청/응답(세션, 보안접근, 데이터 read/write, 루틴 제어, DTC 조회, ECU
  리프로그래밍 등).
- **OBD (On-Board Diagnostics, 배출가스 법규)**: 법적으로 요구되는 표준 PID/DTC/
  readiness 보고. 주로 Dem 이 emission-related 정보를 관리하고 표준 서비스로 응답.

### 주요 UDS 서비스 (SID)
| SID | 이름 | 역할 |
|-----|------|------|
| 0x10 | Diagnostic Session Control | 세션 전환(default/programming/extended) |
| 0x11 | ECU Reset | ECU 리셋 |
| 0x27 | Security Access | seed-key 인증으로 보호 기능 잠금 해제 |
| 0x22 | Read Data By Identifier | DID 로 데이터 읽기 |
| 0x2E | Write Data By Identifier | DID 로 데이터 쓰기 |
| 0x14 | Clear Diagnostic Information | DTC 삭제 |
| 0x19 | Read DTC Information | DTC/상태/freeze frame 조회 |
| 0x31 | Routine Control | 진단 루틴 시작/정지/결과 |
| 0x34~0x37 | Request/Transfer/Exit | 다운로드·업로드(리프로그래밍) |
| 0x3E | Tester Present | 세션 유지(타임아웃 방지) |

## 모듈 구성

| 모듈 | 역할 |
|------|------|
| `Dcm` | Diagnostic Communication Manager — UDS 요청 처리(세션/보안/디스패치) |
| `Dem` | Diagnostic Event Manager — 고장(DTC) 저장·상태·생애주기 |
| `Det` | Default Error Tracer — **개발용** 오류 추적(진단과 별개) |
| `FiM` | Function Inhibition Manager — 고장 시 기능 억제/허가 |
| `Dlt` | Diagnostic Log and Trace — 로깅/트레이싱 |

## Dcm — 외부 진단 요청 처리

정비소 툴의 UDS 요청이 들어오는 경로(대용량이라 TP 경유):
```
툴 → 버스 → Can/Eth(MCAL) → CanIf/EthIf → CanTp/DoIP(재조립) → PduR → Dcm
Dcm: 세션/보안 상태 확인 → 서비스 ID 디스패치 → 데이터 수집(Dem/NvM/SWC 호출) → 응답 조립
응답 → 역경로로 송신
```

Dcm 의 세 가지 핵심 책임:
1. **세션 관리**: default / programming / extended 세션. 세션마다 허용 서비스가 다름.
2. **보안 접근(0x27)**: seed 발급 → 툴이 key 계산 → 검증. 통과해야 보호된 서비스 허용
   (예: 쓰기/리프로그래밍). 무차별 대입 방지(지연/시도 제한).
3. **서비스 디스패치 + 응답 조립**: 실제 데이터는 대부분 다른 모듈에서 가져온다
   (DID 읽기→SWC/NvM, DTC 조회→Dem, 루틴→SWC). Dcm 은 프로토콜 처리에 집중.

> Dcm 은 "데이터를 갖고 있지 않다". 프로토콜(UDS)을 해석하고, 실제 값은 Dem/NvM/SWC
> 에 위임해 모은다. 그래서 SWC 가 진단 데이터/루틴을 제공하는 C-S 서비스 포트가 RTE
> 를 통해 Dcm 과 연결된다.

## Dem — 고장 기록의 중심

ECU 가 자기 고장을 인지하면 Dem 에 이벤트를 보고한다(보통 SWC 가 RTE 통해, 또는 BSW
모니터가 직접).

### DTC 와 이벤트
- **Diagnostic Event**: "이 모니터가 통과/실패했다"의 원자 단위.
- **DTC (Diagnostic Trouble Code)**: 진단 툴에 노출되는 고장 코드. 이벤트 ↔ DTC 매핑.

### 이벤트 → 확정 DTC 의 생애주기
```
모니터 결과
 → Dem_SetEventStatus(EventId, PASSED/FAILED)
 → Debouncing (counter/time 기반)        : 잠깐의 글리치로 확정하지 않음
 → "failed" 확정 → Event Memory 에 기록    : DTC + status byte + Freeze Frame + Extended Data
 → Operation Cycle 동안 상태 갱신
 → Healing / Aging                        : 일정 기간 무고장 시 치유/노화로 제거
```

핵심 개념:
- **Debouncing**: 카운터 기반(±값 누적해 임계 도달 시 확정) 또는 시간 기반. 노이즈로
  인한 오확정 방지.
- **DTC Status Byte (8비트)**: testFailed, confirmedDTC, pendingDTC,
  testFailedSinceLastClear, warningIndicatorRequested 등 비트 플래그. UDS 0x19 가 이걸
  보고.
- **Freeze Frame (Snapshot)**: 고장 확정 순간의 환경 데이터(차속/회전수/온도 등) 저장.
- **Extended Data**: 발생 횟수, aging 카운터 등 부가 정보.
- **Operation Cycle**: 진단 평가의 주기 단위(예: 주행 사이클/점화 사이클). 상태 갱신/
  치유 판단의 기준.
- **Event Memory**: DTC 저장소. 영속화를 위해 `NvM`(→`08`)으로 내려가 비휘발성 저장.

이 생애주기(debounce→confirm→store→age/heal)가 진단 이론의 핵심이다. 단순히 "고장
저장"이 아니라, 오확정 방지·환경 캡처·치유까지 포함한 상태 기계다.

## Det — 개발용 (진단 아님, 반드시 구분)

이름은 비슷하지만 Det 는 **소프트웨어 버그 추적**용이다. BSW 모듈이 API 인자 검증에
실패하면(`<MOD>_DEV_ERROR_DETECT == STD_ON`) `Det_ReportError(ModuleId, InstanceId,
ApiId, ErrorId)` 를 호출. 양산에선 보통 끈다.

- Dem = 차량 고장(DTC, 정비/법규 대상).
- Det = 소프트웨어 개발 오류(디버깅 대상).
- 목적이 완전히 다르므로 절대 혼동하지 말 것.

## FiM — 고장 시 기능 억제

특정 고장이 있을 때 관련 기능을 억제한다. Dem 의 이벤트 상태를 보고 "이 기능을 돌려도
되는지(permission)"를 SWC 에 알려준다. 예: 센서 X 고장이면 X 에 의존하는 제어를 비활성.
`Dem` ↔ `FiM` ↔ `SWC`(RTE 통해 permission 조회).

## Dlt — 로그/트레이스

ECU 내부 로그를 표준 포맷으로 외부에 전송(개발/필드 텔레메트리). 진단 통신과 별개의
로깅 채널.

## DoIP / Bus Mirroring 등 확장

- **DoIP (Diagnostics over IP)**: 진단을 이더넷으로. 대용량 ECU 리프로그래밍에 유리.
  경로: 툴 → Eth → EthIf → TcpIp → SoAd → DoIP → PduR → Dcm. → `06` 이더넷 스택.
- **Bus Mirroring**: 버스 트래픽을 다른 버스로 미러링해 외부에서 관측(진단 제어로 활성).

## 왜 이런 구조인가 (분석)

- **프로토콜과 데이터 분리(Dcm vs Dem/NvM/SWC)**: Dcm 은 UDS 만, 데이터는 소유자에게
  위임 → 진단 프로토콜과 기능 로직을 독립적으로 유지.
- **고장의 상태기계화(Dem)**: 단순 저장이 아니라 debounce/confirm/age/heal 로 진단
  품질(오확정·치유)을 표준화.
- **개발/양산 분리(Det)**: 개발 단계 검증과 양산 진단을 다른 메커니즘으로.
- **법규 대응(OBD)**: 배출가스 법규를 Dem/Dcm 안의 표준 절차로 흡수.

## 원문에서 확인할 것
- *Specification of Diagnostic Communication Manager* (SWS Dcm) — UDS 처리·세션·보안.
- *Specification of Diagnostic Event Manager* (SWS Dem) — DTC 생애주기·event memory.
- *Specification of Default Error Tracer* (SWS Det), *Function Inhibition Manager* (SWS FiM).
- ISO 14229 (UDS), ISO 14229-1 서비스 정의는 ISO 표준 원문.
