# LED Control ECU Interface Design

## 1. RTE Interface

| API | 방향 | 사용자 | 설명 |
|---|---|---|---|
| `Rte_Read_Rp_LedCommand()` | input | SWC | LED 명령을 읽는다. |
| `Rte_Write_Pp_LedState()` | output | SWC | 계산된 LED 상태를 쓴다. |
| `Rte_Inject_LedCommand()` | test/hook | test | 가상 LED 명령을 주입한다. |
| `Rte_Observe_LedState()` | test/hook | test/IoHwAb | SWC 출력 상태를 확인한다. |

## 2. BSW/MCAL Interface

| API | 계층 | 설명 |
|---|---|---|
| `IoHwAb_UpdateInputs()` | IoHwAb | LED-only 단계에서는 no-op. |
| `IoHwAb_UpdateOutputs()` | IoHwAb | RTE LED 상태를 DIO 출력으로 반영한다. |
| `Dio_WriteChannel()` | MCAL-like | 가상 DIO channel level을 쓴다. |
| `Dio_ReadChannel()` | MCAL-like | 가상 DIO channel level을 읽는다. |
| `Port_Init()` | MCAL-like | 포트 초기화를 담당한다. |
