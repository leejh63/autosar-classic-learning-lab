# AUTOSAR Classic Virtual Hardware Config Learning Lab

이 프로젝트는 실제 AUTOSAR Classic 구현체가 아니라, AUTOSAR Classic의 계층 구조를 학습하기 위한 C 기반 AUTOSAR-like lab입니다.

이번 버전의 핵심은 기존의 `EcuC_Cfg.h` 매크로 중심 설정을 줄이고, `Board_Cfg.c` 설정 테이블과 `VirtualHw` 보드 모델을 추가한 것입니다.

## 핵심 구조

```text
SWC
  ↓ RTE
IoHwAb
  ↓
MCAL-like Driver: Dio / Port / Adc / Pwm / Can / Lin
  ↓
VirtualHw
  ↓
Board_Cfg.c 설정 테이블 기반 가상 보드 상태
```

## 현재 완성된 기준 기능

현재는 LED vertical slice 하나가 끝까지 연결되어 있습니다.

```text
Rte_Inject_LedCommand()
  ↓
SWC_LedControl
  ↓
RTE LedState buffer
  ↓
IoHwAb_UpdateOutputs()
  ↓
Dio_WriteChannel(BOARD_DIO_LED1, level)
  ↓
VirtualHw 내부 LED1 pin level 변경
```

## 준비된 일반 모듈 설정

`config/Board_Cfg.c`에 다음 설정이 준비되어 있습니다.

| 구분 | 예시 설정 | 현재 용도 |
|---|---|---|
| DIO output | `BOARD_DIO_LED1`, `BOARD_DIO_DEBUG_PIN` | LED 출력, 디버그 핀 |
| DIO input | `BOARD_DIO_BUTTON1` | 다음 Button 단계 |
| ADC | `BOARD_ADC_POT1`, `BOARD_ADC_TEMP1` | 다음 ADC 단계 |
| PWM | `BOARD_PWM_LED_DIMMER`, `BOARD_PWM_BUZZER` | 다음 PWM 단계 |
| CAN | `BOARD_CAN_PDU_LED_STATUS`, `BOARD_CAN_PDU_BUTTON_STATUS` | 다음 CAN TX 단계 |
| LIN | `BOARD_LIN_FRAME_SENSOR_REQ`, `BOARD_LIN_FRAME_SENSOR_RESP` | 다음 LIN 단계 |

## 사용 방법

```sh
make compile
make test
```

개별 테스트는 다음처럼 실행합니다.

```sh
make vhw
make unit
make module
```

## 권장 학습 순서

1. `config/Board_Cfg.c`에서 가상 보드 설정을 확인한다.
2. `src/vhw/VirtualHw.c`가 설정을 어떻게 상태로 바꾸는지 확인한다.
3. `src/mcal/Dio.c`가 VirtualHw에 어떻게 위임하는지 확인한다.
4. `src/bsw/IoHwAb.c`에서 LED 논리 상태가 실제 DIO level로 변환되는지 확인한다.
5. LED 요구사항/설계/테스트 추적성을 확인한다.
6. 다음 단계로 Button 입력을 추가한다.

## 중요한 규칙

- SWC는 `Dio`, `Adc`, `Pwm`, `Can`, `Lin`을 직접 호출하지 않는다.
- SWC는 RTE만 사용한다.
- IoHwAb는 application signal과 hardware signal 사이를 변환한다.
- MCAL-like driver는 VirtualHw에 접근한다.
- VirtualHw는 실제 보드가 아니라 host test용 가상 보드 모델이다.
- 이 프로젝트를 실제 AUTOSAR Classic 구현체라고 표현하지 않는다.
