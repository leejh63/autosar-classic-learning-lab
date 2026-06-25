# RGB LED Control ECU Software Design

문서 ID: DESIGN-RGB-LED-ECU-001  
상태: 초안

## 1. 설계 목적

RGB LED 출력 3개를 대상으로 AUTOSAR-like 계층 분리와 VirtualHw 기반 보드 없는 테스트 흐름을 학습한다.

본 설계는 `REQ_RGB_LED_Control_ECU.md`의 `REQ-RGB-*` 요구사항을 대상으로 한다.

## 2. 계층 구조

```text
External/Test RgbCommand
  ↓
RTE input buffer
  ↓
SWC_RgbLedControl
  ↓
RTE output buffer
  ↓
IoHwAb
  ↓
Dio
  ↓
VirtualHw
```

## 3. 모듈 책임

| 모듈 | 책임 |
|---|---|
| `Swc_RgbLedControl` | RGB 명령을 읽고 red, blue, green 논리 LED 상태를 결정한다. |
| `Rte` | RGB SWC 입력/출력 buffer를 제공한다. |
| `IoHwAb` | RGB 논리 LED 상태를 보드 DIO channel level로 변환한다. |
| `Dio` | DIO channel 접근 API를 제공하고 VirtualHw에 위임한다. |
| `VirtualHw` | 가상 DIO pin level 상태를 저장한다. |
| `Board_Cfg` | RGB LED 색상과 DIO channel mapping을 정의한다. |
| `Ecu` | 초기화와 100ms 주기 호출 순서를 관리한다. |

## 4. 보드 매핑

RGB LED는 현재 `Board_Cfg.c`의 DIO output channel 3개를 사용한다.

| 논리 색상 | Board DIO channel | 현재 symbolic name |
|---|---|---|
| Red | `BOARD_DIO_LED1` | `RED` |
| Blue | `BOARD_DIO_LED2` | `BLUE` |
| Green | `BOARD_DIO_LED3` | `GREEN` |

`Dio`, `VirtualHw`, `Port`는 channel 수와 설정 배열을 기반으로 동작하므로 RGB 기능 추가를 위해 별도 수정하지 않는다.

## 5. 주기 처리 순서

RGB vertical slice에서 `Ecu_MainFunction_Rgb_100ms()`는 다음 순서로 동작한다.

```text
Ecu_MainFunction_Rgb_100ms()
  1. IoHwAb_UpdateInputs()
  2. Swc_RgbLedControl_Runnable_100ms()
  3. IoHwAb_UpdateRgbOutputs()
```

기존 단일 LED vertical slice는 `BOARD_DIO_LED1`을 사용한다. RGB vertical slice도 red 출력으로 `BOARD_DIO_LED1`을 사용하므로, 두 기능이 같은 주기에서 동시에 같은 DIO channel을 제어하지 않도록 한다.

RGB vertical slice 구현 단계에서는 RGB 기능이 `BOARD_DIO_LED1`, `BOARD_DIO_LED2`, `BOARD_DIO_LED3` 출력 소유권을 가진다. 기존 단일 LED 기능은 이전 학습 baseline으로 유지하되, RGB module test에서는 `Ecu_MainFunction_Rgb_100ms()`를 사용해 RGB runnable과 RGB 출력 변환만 검증한다.

## 6. RTE 설계

RTE는 RGB 기능을 위해 다음 buffer와 API를 제공한다.

```text
RgbCommand input buffer
RgbLedState output buffer
```

`RgbLedStateType`의 component 순서는 코드의 타입 정의와 동일하게 다음 순서를 사용한다.

```c
typedef struct
{
    LedStateType red;
    LedStateType blue;
    LedStateType green;
} RgbLedStateType;
```

예상 API는 다음과 같다.

```text
Rte_Read_Rp_RgbCommand()
Rte_Write_Pp_RgbLedState()
Rte_Bsw_Read_RgbLedState()
Rte_Inject_RgbCommand()
Rte_Observe_RgbLedState()
```

`Rte_Inject_*`와 `Rte_Observe_*` 함수는 테스트와 외부 입력 주입을 위한 학습용 hook이며 AUTOSAR 표준 RTE API로 표현하지 않는다.

## 7. SWC 설계

`Swc_RgbLedControl_Runnable_100ms()`는 다음 순서로 동작한다.

```text
Rte_Read_Rp_RgbCommand()
  ↓
RgbCommand 판단
  ↓
RgbLedState 결정
  ↓
Rte_Write_Pp_RgbLedState()
```

명령별 논리 상태는 다음과 같다.

| RgbCommand | red | blue | green |
|---|---|---|---|
| `RGB_COMMAND_OFF` | `LED_STATE_OFF` | `LED_STATE_OFF` | `LED_STATE_OFF` |
| `RGB_COMMAND_RED` | `LED_STATE_ON` | `LED_STATE_OFF` | `LED_STATE_OFF` |
| `RGB_COMMAND_BLUE` | `LED_STATE_OFF` | `LED_STATE_ON` | `LED_STATE_OFF` |
| `RGB_COMMAND_GREEN` | `LED_STATE_OFF` | `LED_STATE_OFF` | `LED_STATE_ON` |

SWC는 `Dio_WriteChannel()`, `VirtualHw_WriteDioChannel()`을 직접 호출하지 않는다.

## 8. IoHwAb 설계

`IoHwAb_UpdateRgbOutputs()`는 RGB 기능 추가 후 다음 출력 변환을 수행한다.

```text
Rte_Bsw_Read_RgbLedState()
  ↓
각 RGB channel의 Board_DioChannels[channel].active_polarity 확인
  ↓
RgbLedState component → DIO level 변환
  ↓
Dio_WriteChannel(BOARD_DIO_LED1, red_level)
Dio_WriteChannel(BOARD_DIO_LED2, blue_level)
Dio_WriteChannel(BOARD_DIO_LED3, green_level)
```

논리 상태와 DIO level 변환은 channel별 active polarity를 따른다.

| active polarity | `LED_STATE_ON` | `LED_STATE_OFF` |
|---|---|---|
| `VHW_ACTIVE_HIGH` | `TRUE` | `FALSE` |
| `VHW_ACTIVE_LOW` | `FALSE` | `TRUE` |

## 9. 테스트 설계 방향

### Unit test

`Swc_RgbLedControl` unit test는 RTE hook을 사용하여 `RgbCommand`를 주입하고 `RgbLedState`를 관찰한다.

| Test Case ID | 검증 대상 |
|---|---|
| TC-RGB-U-001 | `RGB_COMMAND_OFF`이면 red, blue, green이 모두 OFF |
| TC-RGB-U-002 | `RGB_COMMAND_RED`이면 red만 ON |
| TC-RGB-U-003 | `RGB_COMMAND_GREEN`이면 green만 ON |
| TC-RGB-U-004 | `RGB_COMMAND_BLUE`이면 blue만 ON |

### Module test

RGB module test는 `Ecu_MainFunction_Rgb_100ms()` 실행 후 `Dio_ReadChannel()` 또는 VirtualHw 관찰 API를 통해 DIO level을 검증한다.

| Test Case ID | 검증 대상 |
|---|---|
| TC-RGB-M-001 | RED 명령이 `BOARD_DIO_LED1` active level로 반영됨 |
| TC-RGB-M-002 | GREEN 명령이 `BOARD_DIO_LED3` active level로 반영됨 |
| TC-RGB-M-003 | BLUE 명령이 `BOARD_DIO_LED2` active level로 반영됨 |
| TC-RGB-M-004 | OFF 명령이 RGB 3채널 inactive level로 반영됨 |
