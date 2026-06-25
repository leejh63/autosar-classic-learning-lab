# LED Control ECU Requirements Specification

문서 ID: REQ-LED-ECU-001  
상태: 초안  
대상 시스템: LED Control ECU 학습용 vertical slice

## 1. 목적

본 문서는 LED 하나를 대상으로 SWC, RTE, IoHwAb, MCAL-like Dio, VirtualHw 계층을 끝까지 연결하기 위한 요구사항을 정의한다.

## 2. 시스템 개요

| 항목 | 정의 |
|---|---|
| 입력 | `LedCommand` |
| 논리 출력 | `LedState` |
| 하드웨어 출력 | `BOARD_DIO_LED1` 가상 DIO level |
| 실행 주기 | 100 ms |

## 3. 기능 요구사항

### REQ-LED-F-001: LED ON 명령 처리

`LedCommand`가 `LED_COMMAND_ON`이면 `LedState`는 `LED_STATE_ON`이어야 한다.

### REQ-LED-F-002: LED OFF 명령 처리

`LedCommand`가 `LED_COMMAND_OFF`이면 `LedState`는 `LED_STATE_OFF`이어야 한다.

### REQ-LED-F-003: LED 상태의 DIO 출력 반영

`LedState`는 `BOARD_DIO_LED1`의 가상 DIO level로 반영되어야 한다.

### REQ-LED-F-004: LED active polarity 반영

`LedState`가 `LED_STATE_ON`일 때 실제 DIO level은 `Board_DioChannels[BOARD_DIO_LED1].active_polarity` 설정에 따라 결정되어야 한다.

- `VHW_ACTIVE_HIGH`: ON → TRUE
- `VHW_ACTIVE_LOW`: ON → FALSE

## 4. 비기능 요구사항

### REQ-LED-NF-001: SWC 계층 분리

`Swc_LedControl`은 MCAL-like driver 또는 VirtualHw 함수를 직접 호출하지 않아야 한다.

### REQ-LED-NF-002: 보드 없는 테스트 가능성

실제 보드 없이 VirtualHw의 가상 DIO 상태를 통해 LED 출력 결과를 검증할 수 있어야 한다.
