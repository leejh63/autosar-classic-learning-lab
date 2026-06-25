# 10. 기능안전 (Functional Safety)

차량 SW 는 오동작이 인명 피해로 이어질 수 있어 **기능안전** 이 핵심이다. AUTOSAR 는
안전 표준 ISO 26262 와 맞물려, 안전 요구를 충족하기 위한 메커니즘을 표준 모듈로 제공한다.

## ISO 26262 와 ASIL

- **ISO 26262**: 도로차량 기능안전 표준. 전기/전자 시스템의 오동작으로 인한 위험을
  체계적으로 다룬다(개발 프로세스 + 기술적 안전 요구).
- **ASIL (Automotive Safety Integrity Level)**: 위험도 등급. QM(안전 무관) < ASIL A <
  B < C < D(가장 엄격). 위험원 분석(HARA: severity·exposure·controllability)으로 결정.
  ASIL 이 높을수록 더 엄격한 개발·검증·안전 메커니즘이 요구된다.

AUTOSAR 자체가 "안전"을 보장하진 않는다. 다만 ISO 26262 를 만족하는 시스템을 **만들
수 있게 하는 메커니즘**을 제공한다. 안전 논증(safety case)은 개발자의 몫.

## 핵심 개념: Freedom From Interference (FFI)

여러 ASIL/QM 소프트웨어가 한 ECU 에 공존할 때, **낮은 ASIL/QM 이 높은 ASIL 을 오염시키지
않아야** 한다(간섭으로부터의 자유). 간섭의 세 종류와 대응:

| 간섭 유형 | 위험 | AUTOSAR 대응 |
|-----------|------|--------------|
| 공간(memory) | QM 코드가 ASIL 데이터 영역 침범 | MPU + OS-Application 파티셔닝(→`09`) |
| 시간(timing) | 한 태스크가 CPU 독점/지연 유발 | OS Timing Protection(execution budget) |
| 통신(exchange) | 데이터 손상/유실/오순서 | E2E Protection(→`06`) |

이 세 축(공간·시간·통신 격리)이 AUTOSAR 안전 메커니즘의 뼈대다.

## 안전 메커니즘들

### 1. 메모리 파티셔닝 (공간 격리)
- **OS-Application + MPU**: task/ISR 을 보호 도메인으로 묶고 MPU 로 메모리 접근을 제한.
  trusted(권한 큼)/non-trusted 구분.
- **MemMap 섹션 배치**: 코드/데이터를 정확한 섹션에 두어 파티션에 매핑(→ 컨벤션).
- 효과: QM 파티션의 버그(와일드 포인터 등)가 ASIL 파티션 메모리를 건드리지 못하게.

### 2. 타이밍 보호 (시간 격리)
- **Execution Budget / Time Frame**: task 실행시간·도착간격·리소스 점유시간의 상한을
  OS 가 감시. 초과 시 보호 액션(예: 종료/리셋).
- 효과: 한 기능의 폭주가 다른 안전 기능의 데드라인을 침범하지 못하게.

### 3. 통신 무결성 — E2E (통신 격리)
- 송신 SWC 가 데이터에 **CRC + counter** 를 씌우고 수신 SWC 가 검사. 중간 모듈(RTE/
  Com/PduR/버스)을 신뢰하지 않고 **끝에서 끝까지** 보호.
- 탐지하는 결함: 비트 손상, 유실(counter 점프), 중복, 오순서, 지연(timeout), 잘못된
  주소(데이터 ID).
- 프로파일(E2E Profile): 보호 강도/오버헤드가 다른 여러 프로파일(P01/P02/P04/P05/P07
  등)을 데이터 특성에 맞춰 선택. transformer(E2EXf)로 자동화 가능(→`06`).

### 4. 프로그램 흐름 감시 — WdgM (실행 무결성)
- alive/deadline/**logical** supervision(→`09`)으로 "코드가 살아있고 올바른 순서로
  흐르는가"를 감시. 위반 시 워치독 미트리거 → 리셋.
- logical supervision 이 특히 안전 핵심: 제어 흐름이 예상 경로를 벗어났는지(예: 안전
  체크를 건너뛰었는지) 탐지.

### 5. 그 외
- **DEM 과의 연계**: 안전 관련 고장을 Dem 에 기록/관리(→`07`).
- **중복/다양성**: 안전 요구가 높으면 이중화(redundancy)·다양성(diverse) 설계를 SWC/
  아키텍처 수준에서 적용(AUTOSAR 가 강제하진 않지만 구조가 이를 허용).

## Partitioning 의 의의 (분석)

FFI 를 위한 파티셔닝은 AUTOSAR 안전의 핵심 사상이다: **하나의 강력한 MCU 에 여러 ASIL/
QM 기능을 통합**하면서도(비용·공간 절감), 격리를 통해 안전 등급을 유지한다. 통합과
안전을 동시에 달성하려는 것. 멀티코어/MPU/OS-Application/E2E 가 모두 이 목표에 봉사한다.

## 안전과 보안의 관계

- **Safety(안전)**: 무작위/체계적 고장으로부터 사람을 보호(오동작 방지). E2E·WdgM·
  파티셔닝.
- **Security(보안)**: 악의적 공격으로부터 시스템 보호(→`11`). SecOC·Crypto·IdsM.
- 둘은 다르지만 얽힌다: 보안 침해가 안전 사고를 유발할 수 있어, 현대 차량은 둘을 함께
  설계(security for safety).

## 원문에서 확인할 것
- *Specification of E2E* (SWS E2E), *E2E Protocol Specification* (PRS E2E) — 프로파일·검사.
- *Specification of Operating System* (SWS OS) — memory/timing protection, OS-Application.
- *Specification of Watchdog Manager* (SWS WdgM) — supervision.
- ISO 26262 (Part 1~12) — 기능안전 표준 원문(특히 Part 6: SW 개발).
