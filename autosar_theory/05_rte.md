# 05. RTE — Runtime Environment

RTE 는 VFB(→`03`)를 특정 ECU 에서 **실제로 구현**하는 생성된 미들웨어다. SWC 입장에선
"바깥 세상으로 통하는 유일한 문"이고, 그 문 너머가 다른 SWC 인지 BSW 인지, 같은 ECU
인지 버스 너머인지를 모두 숨긴다.

## RTE 의 두 역할

1. **VFB 의 실현**: 설계 때 가상 버스로 연결한 SWC 포트들을, 실제 배치에 맞춰
   "함수 호출 / 메모리 복사 / 통신 스택 호출"로 변환한다.
   - 같은 ECU 의 두 SWC 연결 → RTE 내부 함수 호출/버퍼.
   - 다른 ECU 와의 연결 → `Com_SendSignal` → PduR → ... → 버스(→`06`).
   - SWC 코드는 둘을 구분하지 못한다. 이게 이식성의 원천.
2. **SWC 의 실행 환경 제공**: runnable 을 OS 태스크에 매핑하고, RTE 이벤트(→`04`)가
   오면 runnable 을 호출한다. SWC 와 OS 사이의 접착제.

## RTE 는 "생성"된다 — 두 단계

RTE 는 손으로 짜는 게 아니라 ARXML 설정에서 **RTE generator** 가 만든다. 두 단계로 나뉜다:

- **Contract Phase**: SWC 의 인터페이스(포트/데이터타입)만 보고 **`Rte_<SWC>.h`
  (application header)** 를 생성. SWC 구현자는 이 헤더에 맞춰 runnable 을 짠다. 시스템
  전체 구성이 아직 안 끝나도 SWC 개발을 시작할 수 있게 하는 "계약". 공급사가 SWC 를
  독립 개발할 수 있는 이유.
- **Generation Phase**: ECU 전체 구성(어떤 SWC 들이 어떻게 연결, BSW 설정)을 보고
  **`Rte.c`(실제 배선)** 를 생성. 포트 연결을 실제 호출/버퍼/통신으로 구현.

이 분리 덕에 "인터페이스 합의 → 병렬 구현 → 통합 시 배선 생성"이 가능하다.

## 통신 패턴 총정리

SWC 가 호출하는 `Rte_*` API 의 의미론이 통신의 성격을 결정한다. 패턴별로 정리한다.

### (1) Sender-Receiver, explicit, last-is-best
"최신 값 하나"를 단일 슬롯에 공유. 여러 번 써도 마지막 값만, 여러 번 읽어도 같은 값
(소비되지 않음).
```c
Rte_Write_<Port>_<Elem>(value);   // 송신
Rte_Read_<Port>_<Elem>(&value);   // 수신 (소비 안 함)
```
용도: 센서 최신값, 현재 setpoint 등 "지금 상태". explicit = 호출 시점에 즉시 동작.

### (2) Sender-Receiver, queued (event)
이벤트성 데이터를 **FIFO 큐**로. 보내면 쌓이고 받으면 하나씩 소비. 큐가 가득 차면
overflow, 비면 no-data.
```c
Rte_Send_<Port>_<Elem>(value);    // enqueue
Rte_Receive_<Port>_<Elem>(&value);// dequeue (소비됨)
```
> Read/Write(상태) 와 Send/Receive(이벤트)는 **완전히 다른 패턴**이다. 이름이 비슷해
> 혼동하기 쉽지만, "소비되는가"가 핵심 차이. AUTOSAR S-R 이해의 첫 관문.

### (3) Sender-Receiver, implicit (IRead/IWrite)
runnable 시작 시 입력 **스냅샷**을 떠 두고(IRead), runnable 안에선 그 복사본만 본다.
출력은 로컬에 모았다가 runnable **종료 시 한꺼번에 commit**(IWrite).
```c
v = Rte_IRead_<Run>_<Port>_<Elem>();    // 시작 시점 스냅샷
Rte_IWrite_<Run>_<Port>_<Elem>(v2);     // 종료 시 일괄 반영
```
의의: runnable 도중 입력이 안 바뀌는 **데이터 일관성** + 락 최소화. 제어 루프에 유용.

### (4) Client-Server (Call / Result)
함수 호출. 서버 SWC(또는 BSW)가 오퍼레이션 제공, 클라이언트가 호출.
```c
// 동기: 그 자리에서 끝남
Rte_Call_<Port>_<Op>(args, &result);
// 비동기: 요청만 하고 나중에 회수
Rte_Call_<Port>_<Op>(args);                 // 요청
Rte_Result_<Port>_<Op>(&result);            // 회수 (또는 AsyncReturnsEvent 로 트리거)
```
서버측 runnable 은 OperationInvokedEvent 로 트리거된다(→`04`).

### (5) Mode (Switch / Mode)
모드 머신. 모드 매니저가 전환(Switch), 다른 SWC 는 현재 모드 조회(Mode)하거나 모드
진입/이탈 runnable(ModeSwitchEvent)로 반응. mode disablement 로 특정 모드에서만 특정
runnable 이 돌게 게이팅.
```c
Rte_Switch_<Port>_<Mode>(newmode);
m = Rte_Mode_<Port>_<Mode>();
```

### (6) Inter-Runnable Variable (IRV)
같은 SWC 의 runnable 간 공유 변수. RTE 가 일관성(필요 시 락) 보장.
```c
Rte_IrvWrite_<Run>_<Var>(v);   Rte_IrvRead_<Run>_<Var>();
```

### (7) Per-Instance Memory (PIM)
SWC 가 여러 인스턴스일 때 인스턴스별 상태 메모리.
```c
p = Rte_Pim_<Name>();
```

### (8) Calibration / Parameter
보정값/파라미터 접근. 코드 상수 대신 calibration 데이터로 두고 양산에서 조정(XCP).
```c
Rte_CData_<Name>();   Rte_Prm_<Name>();
```

### (9) Trigger
외부 자극(트리거)을 runnable 활성화로 전달(주기와 무관한 이벤트 기반 실행).
```c
Rte_Trigger_<Port>_<Trig>();
```

### (10) Exclusive Area (Enter / Exit)
SWC 내부의 짧은 임계영역. RTE 가 실제 락(인터럽트 비활성 또는 OS resource)으로 매핑.
```c
Rte_Enter_<EA>();  /* 보호 구간 */  Rte_Exit_<EA>();
```

## 패턴 요약표

| 패턴 | API | 의미 | 소비? |
|------|-----|------|-------|
| S-R explicit | `Rte_Write/Read` | 최신값 1개 | 아니오 |
| S-R queued | `Rte_Send/Receive` | 이벤트 FIFO | 예 |
| S-R implicit | `Rte_IWrite/IRead` | runnable 경계 copy | - |
| C-S sync | `Rte_Call` | 함수 호출(즉시) | - |
| C-S async | `Rte_Call`+`Rte_Result` | 호출+나중 회수 | - |
| Mode | `Rte_Switch/Mode` | 모드 전환/조회 | - |
| IRV | `Rte_IrvWrite/IrvRead` | runnable 간 공유 | - |
| PIM | `Rte_Pim_*` | 인스턴스별 메모리 | - |
| Calibration | `Rte_CData/Prm` | 보정값 | - |
| Trigger | `Rte_Trigger_*` | 외부 자극 | - |
| Exclusive Area | `Rte_Enter/Exit` | 임계영역 | - |

## RTE 와 OS — runnable 의 실행

runnable 은 스스로 돌지 않는다. RTE 가 runnable 을 **OS 태스크에 매핑**하고, 그 태스크가
스케줄되면 RTE 가 해당 runnable 들을 적절한 순서로 호출한다. RTE 이벤트(TimingEvent
등)는 결국 OS 의 알람/스케줄 테이블(→`09`)로 실현된다.

- runnable→task 매핑(어떤 runnable 이 어떤 태스크/우선순위에서, 어떤 순서로)은 RTE/OS
  설정의 핵심 튜닝 포인트다.
- 데이터 일관성(같은 데이터를 여러 runnable 이 다른 태스크에서 접근)을 위해 exclusive
  area/IRV 가 OS 락으로 매핑된다.

## SchM 과의 관계

RTE 가 SWC 의 runnable 을 돌리듯, BSW 의 `*_MainFunction` 과 exclusive area 를 다루는
형제 메커니즘이 **SchM(BSW Scheduler)** 이다. 실제로 RTE 와 SchM 은 같은 generator 가
함께 생성하는 경우가 많다(BSW Scheduler 는 RTE 의 일부로 취급되기도 한다). → `09`.

## 왜 RTE 인가 (분석)

- **배치 독립성**: SWC 는 RTE 만 보므로, 같은 SWC 망을 ECU 1개로도 N개로도 배치 가능.
- **통신 투명성**: 로컬/네트워크 통신을 한 API 뒤로 통일. 통신 방식 변경이 SWC 에
  파급되지 않음.
- **생성에 의한 정확성**: 수많은 포트 연결을 손으로 배선하면 오류 천지다. 모델→생성이
  일관성을 보장.
- **계약(contract phase)**: 인터페이스만으로 SWC 개발 착수 → 대규모 병렬 개발.

## 원문에서 확인할 것
- *Specification of RTE Software* (SWS RTE) — RTE API·생성·이벤트·패턴의 1차 출처.
- *Virtual Functional Bus* (EXP VFB) — RTE 가 실현하는 추상.
- *Specification of BSW Scheduler* (RTE 문서 내) — SchM/exclusive area.
