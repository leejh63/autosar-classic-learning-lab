# RGB LED Control ECU Requirements Specification

문서 ID: REQ-RGB-LED-ECU-001  
상태: 초안  
대상 시스템: RGB LED Control ECU 학습용 vertical slice

## 1. 목적

본 문서는 빨강, 초록, 파랑 DIO 출력 3개를 사용하여 RGB LED 색상을 제어하기 위한 요구사항을 정의한다.

## 2. 시스템 개요

| 항목 | 정의 |
|---|---|
| 입력 | `RgbCommand` |
| 논리 출력 | `RgbLedState.red`, `RgbLedState.green`, `RgbLedState.blue` |
| 하드웨어 출력 | `BOARD_DIO_LED1`, `BOARD_DIO_LED2`, `BOARD_DIO_LED3` 가상 DIO level |
| 실행 주기 | 100 ms |

## 3. 보드 채널 매핑

| 논리 색상 | Board DIO channel | 현재 symbolic name |
|---|---|---|
| Red | `BOARD_DIO_LED1` | `RED` |
| Blue | `BOARD_DIO_LED2` | `BLUE` |
| Green | `BOARD_DIO_LED3` | `GREEN` |

## 4. 기능 요구사항

### REQ-RGB-F-001: RGB OFF 명령 처리

`RgbCommand`가 `RGB_COMMAND_OFF`이면 `RgbLedState.red`, `RgbLedState.green`, `RgbLedState.blue`는 모두 `LED_STATE_OFF`이어야 한다.

### REQ-RGB-F-002: RGB RED 명령 처리

`RgbCommand`가 `RGB_COMMAND_RED`이면 `RgbLedState.red`는 `LED_STATE_ON`이고, `RgbLedState.green`과 `RgbLedState.blue`는 `LED_STATE_OFF`이어야 한다.

### REQ-RGB-F-003: RGB GREEN 명령 처리

`RgbCommand`가 `RGB_COMMAND_GREEN`이면 `RgbLedState.green`은 `LED_STATE_ON`이고, `RgbLedState.red`와 `RgbLedState.blue`는 `LED_STATE_OFF`이어야 한다.

### REQ-RGB-F-004: RGB BLUE 명령 처리

`RgbCommand`가 `RGB_COMMAND_BLUE`이면 `RgbLedState.blue`는 `LED_STATE_ON`이고, `RgbLedState.red`와 `RgbLedState.green`은 `LED_STATE_OFF`이어야 한다.

### REQ-RGB-F-005: RGB 상태의 DIO 출력 반영

`RgbLedState.red`, `RgbLedState.blue`, `RgbLedState.green`은 각각 `BOARD_DIO_LED1`, `BOARD_DIO_LED2`, `BOARD_DIO_LED3`의 가상 DIO level로 반영되어야 한다.

### REQ-RGB-F-006: RGB active polarity 반영

각 RGB LED state가 `LED_STATE_ON`일 때 실제 DIO level은 해당 `Board_DioChannels[channel].active_polarity` 설정에 따라 결정되어야 한다.

- `VHW_ACTIVE_HIGH`: ON -> TRUE
- `VHW_ACTIVE_LOW`: ON -> FALSE

## 5. 비기능 요구사항

### REQ-RGB-NF-001: SWC 계층 분리

RGB LED 제어 SWC는 MCAL-like driver 또는 VirtualHw 함수를 직접 호출하지 않아야 한다.

### REQ-RGB-NF-002: 보드 없는 테스트 가능성

실제 보드 없이 VirtualHw의 가상 DIO 상태를 통해 red, green, blue 출력 결과를 검증할 수 있어야 한다.
