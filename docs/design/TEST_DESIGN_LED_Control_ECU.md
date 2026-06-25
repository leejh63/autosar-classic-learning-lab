# LED Control ECU Test Design

## 1. Unit Test 범위

대상: `Swc_LedControl_Runnable_100ms()`

| Test ID | 목적 | 입력 | 기대 결과 |
|---|---|---|---|
| TC-LED-U-001 | ON 명령 처리 | `LED_COMMAND_ON` | `LED_STATE_ON` |
| TC-LED-U-002 | OFF 명령 처리 | `LED_COMMAND_OFF` | `LED_STATE_OFF` |

## 2. Module Test 범위

대상: `Ecu_MainFunction_100ms()` 전체 흐름

| Test ID | 목적 | 입력 | 기대 결과 |
|---|---|---|---|
| TC-LED-M-001 | ON 명령이 DIO active level로 반영되는지 확인 | `LED_COMMAND_ON` | LED DIO level active |
| TC-LED-M-002 | OFF 명령이 DIO inactive level로 반영되는지 확인 | `LED_COMMAND_OFF` | LED DIO level inactive |
