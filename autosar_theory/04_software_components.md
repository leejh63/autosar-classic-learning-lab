# 04. 소프트웨어 컴포넌트(SWC)와 애플리케이션 모델

애플리케이션 계층의 재사용 단위가 **SWC(Software Component)** 다. 이 장은 SWC 가
무엇으로 이루어지고, 어떻게 서로(그리고 BSW 와) 연결되는지의 모델을 다룬다. 모두
VFB(→`03`) 레벨의 개념이고, 실제 실현은 RTE(→`05`)가 한다.

## SWC 의 본질

SWC 는 **하드웨어를 모르는, 포트로만 소통하는 기능 단위**다. 두 가지 핵심 성질:
- **캡슐화**: 내부 구현은 숨고, 바깥과는 오직 **포트(port)** 로만 연결.
- **이식성**: 어느 ECU 에 올라가든 코드가 같다. RTE 가 차이를 흡수.

SWC 는 ARXML 의 SW Component Description 으로 기술되고(인터페이스/포트/internal
behavior), 내부 로직(runnable 본문)만 C 로 구현한다.

## SWC 의 종류

| 종류 | 역할 |
|------|------|
| Application SWC | 순수 기능 로직. 가장 일반적. |
| Sensor/Actuator SWC | 센서/액추에이터에 논리적으로 결합된 SWC. IoHwAb 를 통해 물리 IO 접근. |
| Service SWC | BSW 서비스를 SWC 세계로 노출하는 프록시(예: 진단/NV 관련). |
| ECU Abstraction SWC | ECU 추상화 자원을 SWC 로 표현. |
| Complex Device Driver SWC | CDD 를 SWC 인터페이스로 감싼 형태. |
| Parameter SWC | 보정/파라미터(calibration) 데이터를 제공. |
| NvBlock SWC | NV 블록을 SWC 포트로 다루게 하는 특수 컴포넌트. |
| Composition | 여러 SWC 를 묶는 상위 구성요소(자신은 로직 없음, 연결만). |

**Composition** 이 중요하다: SWC 들을 계층적으로 묶어 큰 단위로 다루되, 컴파일/배치
시점엔 내부의 원자 SWC(atomic component)들로 펼쳐진다. 설계의 모듈화를 위한 그릇.

## 포트와 포트 인터페이스

SWC 는 **포트(port)** 로 연결된다. 포트에는 **포트 인터페이스(port interface)** 가
붙어 통신의 종류를 정한다.

포트 방향:
- **P-Port (Provide)**: 데이터/서비스를 제공.
- **R-Port (Require)**: 데이터/서비스를 요구.
- (PR-Port: 둘 다)

포트 인터페이스 종류:

| 인터페이스 | 의미 | 대응 RTE |
|------------|------|----------|
| Sender-Receiver (S-R) | 데이터 송수신 | `Rte_Write/Read`, `Rte_Send/Receive` |
| Client-Server (C-S) | 함수(오퍼레이션) 호출 | `Rte_Call`, `Rte_Result` |
| Mode Switch | 모드 전환/통지 | `Rte_Switch`, `Rte_Mode` |
| NV Data | 비휘발성 데이터 접근 | NvBlock 연계 |
| Parameter | 보정/파라미터 | `Rte_Prm`, `Rte_CData` |
| Trigger | 트리거(외부 자극) 전달 | `Rte_Trigger`, `Rte_IrTrigger` |

S-R 과 C-S 가 가장 기본이다. S-R 은 "데이터를 공유", C-S 는 "함수를 호출". 각 패턴의
의미론은 `05` 에서 깊이 다룬다.

## Internal Behavior — SWC 의 동작 모델

SWC 가 "무엇으로 동작하는가"는 **internal behavior** 로 기술된다. 핵심 요소:

### Runnable (Runnable Entity)
SWC 의 실행 단위(C 함수에 대응). SWC 자체는 "도는" 게 아니라, 그 안의 runnable 들이
이벤트에 의해 호출된다. runnable 은 어떤 포트에 접근하는지, 어떤 이벤트로 트리거되는지가
모델에 명시된다.

### RTE Event — runnable 을 깨우는 트리거
| 이벤트 | 트리거 조건 |
|--------|-------------|
| TimingEvent | 주기적(예: 10ms마다) |
| DataReceivedEvent | S-R 데이터 수신 시 |
| DataReceiveErrorEvent | 수신 오류/타임아웃 시 |
| OperationInvokedEvent | C-S 서버측: 오퍼레이션이 호출됨 |
| ModeSwitchEvent | 모드 진입/이탈 시 |
| AsynchronousServerCallReturnsEvent | 비동기 C-S 결과 도착 |
| InitEvent | 초기화 시 |
| BackgroundEvent | 유휴(background) 시 |

한 SWC 는 보통 여러 runnable 을 갖고, 각각 다른 이벤트로 트리거된다. 예: 10ms 주기
제어 runnable + 수신 시 도는 runnable + 진단 호출 시 도는 runnable.

### 동시성 제어 요소
- **Exclusive Area**: SWC 내부 데이터 일관성을 위한 임계영역. RTE 가 실제 락(인터럽트
  잠금 또는 OS resource)으로 매핑. → `05`, `09`.
- **Inter-Runnable Variable (IRV)**: 같은 SWC 의 runnable 들이 공유하는 변수(RTE 가
  일관성 보장). 전역변수 직접 사용 대신 사용.
- **Per-Instance Memory (PIM)**: SWC 가 여러 인스턴스로 존재할 때 인스턴스별 상태.

## 데이터 타입의 3층 구조

AUTOSAR 의 데이터 타입은 의도적으로 추상-구현을 분리한다:

```
Application Data Type (ADT)      물리적 의미 (예: 온도 [°C], -40~215, 0.1 단위)
        │  (Data Type Mapping)
        ▼
Implementation Data Type (IDT)   C 표현 (예: uint16)
        │
        ▼
Base Type                        플랫폼 기본형 (예: 16-bit unsigned)
```

- **Application Data Type**: 물리량/의미 중심. 단위, 범위, 스케일(compu-method:
  내부값↔물리값 변환), enum 의 의미.
- **Implementation Data Type**: 실제 C 타입(`uint8`, struct 등).
- **Data Type Mapping**: ADT↔IDT 연결. 같은 물리 의미를 다른 ECU 에서 다른 C 타입으로
  구현할 수 있게 한다.

의의: SWC 설계자는 "온도"라는 물리 의미로 설계하고, 구현 타입은 ECU 사정에 맞춰
매핑한다. **의미와 표현의 분리**가 이식성과 재사용을 돕는다.

## 한 SWC 의 구성 요약

```
SWC "MyControl"
├─ Ports
│   ├─ R-Port "Sensor"  (S-R interface: 온도 데이터)
│   └─ P-Port "Actuator"(C-S interface: 출력 설정 오퍼레이션)
├─ Internal Behavior
│   ├─ Runnable "Run_10ms" (TimingEvent 10ms; Sensor 읽고 Actuator 호출)
│   ├─ Inter-Runnable Variable "state"
│   └─ Exclusive Area "ctrl_ea"
└─ Implementation (C: Run_10ms 본문)
```

이 전체(포트/인터페이스/internal behavior/데이터타입)는 ARXML 로 기술되고, RTE
generator 가 이 SWC 가 부를 `Rte_*` 함수들을 생성한다. 개발자는 runnable 본문의
로직만 C 로 채운다.

## 왜 이런 모델인가 (분석)

- **설계-구현 분리**: 포트/인터페이스(설계)와 runnable 로직(구현)을 나눠, 인터페이스
  계약을 먼저 합의하고 구현은 병렬로.
- **이식성**: SWC 는 RTE 만 의존하므로 ECU 간 이동 시 재컴파일만으로 동작.
- **조합성**: composition 으로 SWC 를 레고처럼 묶고 재배치.
- **의미 중심 데이터**: 물리 의미(ADT)로 설계해 단위/스케일 오류를 모델 단계에서 차단.

## 원문에서 확인할 것
- *Software Component Template* (TPS SWCT) — SWC/포트/internal behavior/runnable 의 1차 출처.
- *Specification of RTE* (SWS RTE) — runnable·RTE 이벤트·API 정의.
- *Generic Structure Template* — 데이터 타입(ADT/IDT/mapping) 모델.
