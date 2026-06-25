# 09. OS 와 시스템 서비스

ECU 가 "켜지고, 태스크를 돌리고, 모드를 바꾸고, 감시되고, 안전하게 꺼지는" 전 과정을
관장하는 모듈군. 통신/진단/메모리가 "무엇을"이라면, 시스템 서비스는 "언제·어떤
순서로·어떤 상태에서"를 정한다.

| 모듈 | 역할 |
|------|------|
| `Os` | AUTOSAR OS — task/event/alarm/resource/counter/schedule table/멀티코어 |
| `EcuM` | ECU State Manager — startup/shutdown/sleep 오케스트레이션 |
| `BswM` | BSW Mode Manager — 모드 요청↔액션 규칙 중재 |
| `SchM` | BSW Scheduler — BSW main function 구동, exclusive area |
| `WdgM` | Watchdog Manager — alive/deadline/logical 감시 |
| `StbM`/`Tm` | 시간 — 동기화 타임베이스 / 시간 서비스 |

## AUTOSAR OS — OSEK 의 후예

AUTOSAR OS 는 **OSEK/VDX OS** 를 기반으로 확장한 정적 실시간 OS 다. 일반 RTOS(예:
FreeRTOS)와 같은 범주지만 철학이 다르다.

### 핵심 성질
- **정적 구성**: task/alarm/resource/counter 를 런타임에 생성하지 않는다. 전부 설정
  (OIL/ARXML)으로 미리 정의되고 생성 시점에 고정. 동적 할당 없음 → 결정론·검증성.
- **우선순위 기반 선점 스케줄링**: 고정 우선순위. 같은 우선순위는 FIFO.
- **단일 스택 가능(basic task)**: 일부 태스크는 스택을 공유할 수 있어 RAM 절약.

### Task 의 두 종류
- **Basic Task**: 실행→종료만. 대기(블로킹) 불가. run-to-completion. 스택 공유 가능.
- **Extended Task**: `WaitEvent` 로 이벤트를 기다릴 수 있음(블로킹). 자체 스택 필요.

### Conformance Class (적합성 등급)
어떤 기능을 지원하느냐로 OS 등급이 나뉜다:
| 등급 | 의미 |
|------|------|
| BCC1 | Basic, task당 1 activation, 우선순위별 1 task |
| BCC2 | Basic, 다중 activation/우선순위당 다중 task |
| ECC1 | Extended(event), 기본 |
| ECC2 | Extended + 다중 activation |
ECC 는 event(블로킹)를 지원. 시스템 요구에 맞춰 최소 등급을 고른다.

### 동기화/통신 객체
- **Resource (리소스)**: 상호배제. **Priority Ceiling Protocol**(우선순위 천장)으로
  우선순위 역전과 데드락을 원천 차단 — 리소스를 잡으면 그 리소스의 천장 우선순위로
  즉시 상승. `GetResource`/`ReleaseResource`.
- **Event**: extended task 의 대기/시그널(`SetEvent`/`WaitEvent`/`ClearEvent`).
- **Counter / Alarm**: 카운터(틱 소스)에 알람을 걸어 주기/단발로 task 활성화·event
  설정·콜백 호출. 시간 기반 스케줄링의 토대.
- **Schedule Table**: 알람보다 정교한 시간 동기 스케줄. expiry point 들을 테이블로
  정의해 결정론적 주기 실행(동기화/위상 제어에 유리).
- **Spinlock**: 멀티코어에서 코어 간 상호배제(바쁜 대기).

> FreeRTOS 대비 사고 전환 포인트:
> - 동적 생성 금지(전부 정적 설정) ↔ FreeRTOS 의 `xTaskCreate` 런타임 생성.
> - Resource = 우선순위 천장(자동 상승) ↔ FreeRTOS 뮤텍스의 우선순위 상속.
> - Alarm/Counter/ScheduleTable 로 시간 활성화 ↔ FreeRTOS 의 `vTaskDelay`/타이머.
> - Event(extended task) ↔ FreeRTOS task notification/event group.
> RTOS 를 직접 다뤄봤다면 이 대응표를 정리해 두는 게 면접·실무 양쪽에서 강력하다.

### Protection (보호) — 안전과 직결
AUTOSAR OS 는 안전(→`10`)을 위한 보호 기능을 제공:
- **Memory Protection**: MPU 로 OS-Application(파티션) 간 메모리 격리.
- **Timing Protection**: task 실행시간/도착간격 초과 감시(execution budget).
- **Service Protection**: 잘못된 OS 호출 차단.

### OS-Application 과 멀티코어
- **OS-Application**: task/ISR/알람 등을 묶는 보호 도메인. trusted/non-trusted 로 권한
  구분. 안전 파티셔닝의 단위.
- **멀티코어**: 코어마다 OS-Application 을 두고, 코어 간 통신은 **IOC(Inter-OS-
  Application Communication)** 로. RTE 가 파티션 경계를 넘는 SWC 통신을 IOC 로 매핑
  (같은 코어면 직접, 다른 코어면 IOC). 스핀락으로 코어 간 상호배제.

## EcuM — ECU 상태/슬립

전원 인가부터 종료까지의 상태를 관장한다. 대략의 부팅 순서:
```
reset → EcuM_Init
  → Mcu_Init / Port_Init        (클럭·핀, 가장 먼저)
  → 드라이버/BSW 기본 Init, Det/Dem 초기화
  → NvM_ReadAll                 (지난 상태 복원, →`08`)
  → SchM/BswM 초기화
  → StartOS                     (OS 스케줄링 시작)
      → 주기 task 에서 SchM 이 *_MainFunction 들 호출
      → Rte_Start → SWC runnable 들이 돌기 시작
```
- 상태: STARTUP → RUN → SHUTDOWN, 그리고 SLEEP.
- **슬립/웨이크업**: 웨이크업 소스(특정 핀, CAN 메시지, 타이머)를 설정해 두고, 깨우는
  이벤트로 복귀. 저전력 진입/탈출의 중심. (EcuM Flexible 변형은 BswM 와 역할 분담)

## BswM — 규칙 기반 모드 중재

"이런 조건(통신 모드·진단 세션·NV 상태 등)이면 이런 액션을 하라"는 **규칙표(action
list)**를 실행한다. 여러 모듈의 모드 신호를 모아 시스템 차원 결정을 내린다.
- 예: 진단 세션 진입 → 통신 채널 Full 유지 / shutdown 요청 → NvM_WriteAll 후 EcuM 에
  종료 통보 / partial networking 제어.
- 연결: ComM/Dcm/NvM/EcuM 과 두루 엮인다. AUTOSAR 의 "모드 관리" 중심.

## SchM — BSW 스케줄러

RTE 가 SWC 의 runnable 을 돌리듯, BSW 의 `*_MainFunction` 들을 적절한 주기/태스크에서
호출하고, BSW 의 **exclusive area** 를 실제 OS 락으로 매핑한다(RTE 의 Rte_Enter/Exit
와 형제). 실제로 RTE 와 함께 생성되며 RTE 의 일부로 취급되기도 한다.

## WdgM — 워치독 매니저

소프트웨어가 살아있고 올바르게 흐르는지 **다층 감시**:
- **Alive Supervision**: 체크포인트가 기대 빈도로 찍히는가(너무 자주/드물지 않은가).
- **Deadline Supervision**: 두 체크포인트 사이 시간이 한계 내인가.
- **Logical Supervision**: 체크포인트 순서(제어 흐름/program flow)가 올바른가.

위반이 누적되면 `WdgIf`→`Wdg`(MCAL) 트리거를 **멈춰** 하드웨어 워치독이 결국 리셋을
유발한다. "감시 실패 → 워치독 안 먹임 → 리셋"의 안전 사슬(→`10` program flow monitoring).

## 시간 — StbM / Tm

- `StbM`(Synchronized Time-Base Manager): 차량 전역 동기 시계 제공. `CanTSyn`/`EthTSyn`
  (→`06`)가 네트워크로 시간을 맞추고 StbM 이 타임 도메인을 상위에 노출. 분산 로깅/
  센서퓨전/협조제어에 필수.
- `Tm`(Time Service): 가벼운 예측 타이머/짧은 busy wait.

## 왜 이런 구조인가 (분석)

- **결정론(정적 OS)**: 동적 할당을 없애 타이밍/메모리를 사전 검증 가능 → 안전 인증 용이.
- **우선순위 천장(Resource)**: 우선순위 역전/데드락을 프로토콜 차원에서 차단.
- **모드의 중앙 중재(BswM)**: 흩어진 모드 신호를 한 규칙 엔진으로 모아 시스템 일관성.
- **다층 감시(WdgM)**: 단순 워치독을 넘어 "살아있음+시간+흐름"을 표준 감시로.
- **파티셔닝/멀티코어**: OS-Application+MPU+IOC 로 안전 격리와 멀티코어를 표준화.

## 원문에서 확인할 것
- *Specification of Operating System* (SWS OS) — task/resource/alarm/schedule table/보호/멀티코어.
- OSEK/VDX OS 명세 — AUTOSAR OS 의 뿌리.
- *Specification of ECU State Manager* (SWS EcuM), *BSW Mode Manager* (SWS BswM).
- *Specification of Watchdog Manager* (SWS WdgM), *Synchronized Time-Base Manager* (SWS StbM).
