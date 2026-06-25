# LED Control ECU Requirements Specification

문서 ID: REQ-LED-ECU-001  
상태: 초안  
대상 시스템: LED Control ECU 학습용 vertical slice

## 1. 목적

본 문서는 학습용 LED Control ECU의 요구사항을 정의한다. 이 단계의 목적은 LED 출력 하나를 통해 AUTOSAR Classic의 SWC, RTE, IoHwAb, MCAL 계층 분리를 이해하는 것이다.

## 2. 시스템 개요

| 항목 | 정의 |
|---|---|
| 입력 | `LedCommand` |
| 출력 | `LedState`, 가상 DIO LED level |
| 실행 주기 | 100 ms |
| 하드웨어 | 가상 DIO 채널 1개 |

## 3. 기능 요구사항

### REQ-LED-F-001: LED ON 명령 처리

`LedCommand`가 `LED_COMMAND_ON`이면 `LedState`는 `LED_STATE_ON`이어야 한다.

### REQ-LED-F-002: LED OFF 명령 처리

`LedCommand`가 `LED_COMMAND_OFF`이면 `LedState`는 `LED_STATE_OFF`이어야 한다.

### REQ-LED-F-003: LED 상태의 하드웨어 출력 반영

`LedState`는 설정된 DIO 채널 `ECUC_DIO_CHANNEL_LED1`의 출력 level로 반영되어야 한다.

### REQ-LED-F-004: Active-high/active-low 설정 반영

LED 출력 level은 `ECUC_DIO_LED1_ACTIVE_HIGH` 설정을 따라야 한다.

- active-high: `LED_STATE_ON` → DIO TRUE
- active-low: `LED_STATE_ON` → DIO FALSE

## 4. 비기능 요구사항

### REQ-LED-NF-001: 계층 분리

`Swc_LedControl`은 MCAL 또는 IoHwAb 함수를 직접 호출하지 않고 RTE API만 사용해야 한다.

### REQ-LED-NF-002: 보드 없는 테스트 가능성

실제 보드 없이 가상 DIO 상태를 읽어 LED 출력 결과를 검증할 수 있어야 한다.
