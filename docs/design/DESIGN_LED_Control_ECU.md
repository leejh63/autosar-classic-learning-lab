# LED Control ECU Software Design

문서 ID: DESIGN-LED-ECU-001  
상태: 초안

## 1. 설계 목적

LED 출력 하나를 대상으로 AUTOSAR-like 계층 분리와 VirtualHw 기반 보드 없는 테스트 흐름을 학습한다.

## 2. 계층 구조

```text
External/Test LedCommand
  ↓
RTE input buffer
  ↓
SWC_LedControl
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
| `Swc_LedControl` | LED 명령을 읽고 논리 LED 상태를 결정한다. |
| `Rte` | SWC 입력/출력 buffer를 제공한다. |
| `IoHwAb` | 논리 LED 상태를 실제 DIO level로 변환한다. |
| `Dio` | DIO channel 접근 API를 제공하고 VirtualHw에 위임한다. |
| `VirtualHw` | 가상 LED 핀 상태를 저장한다. |
| `Ecu` | 초기화와 100ms 주기 호출 순서를 관리한다. |

## 4. 주기 처리 순서

```text
Ecu_MainFunction_100ms()
  1. IoHwAb_UpdateInputs()
  2. Swc_LedControl_Runnable_100ms()
  3. IoHwAb_UpdateOutputs()
```

## 5. SWC 설계

`Swc_LedControl_Runnable_100ms()`는 다음 순서로 동작한다.

```text
Rte_Read_Rp_LedCommand()
  ↓
LedCommand 판단
  ↓
Rte_Write_Pp_LedState()
```

SWC는 `Dio_WriteChannel()`, `VirtualHw_WriteDioChannel()`을 직접 호출하지 않는다.

## 6. IoHwAb 설계

`IoHwAb_UpdateOutputs()`는 다음 순서로 동작한다.

```text
Rte_Bsw_Read_LedState()
  ↓
Board_DioChannels[BOARD_DIO_LED1].active_polarity 확인
  ↓
LedState → DIO level 변환
  ↓
Dio_WriteChannel(BOARD_DIO_LED1, level)
```
