# LED Control ECU Software Design

문서 ID: DESIGN-LED-ECU-001  
상태: 초안

## 1. 설계 목적

LED 출력 하나를 대상으로 AUTOSAR Classic의 기본 계층 구조를 학습한다.

## 2. 계층 구조

```text
SWC_LedControl
  ↓ RTE
IoHwAb
  ↓
Dio / Port
```

## 3. 모듈 책임

| 모듈 | 책임 |
|---|---|
| `Swc_LedControl` | LED 명령을 읽고 논리 LED 상태를 결정한다. |
| `Rte` | SWC 입력/출력 buffer를 제공한다. |
| `IoHwAb` | 논리 LED 상태를 실제 DIO level로 변환한다. |
| `Dio` | 가상 DIO channel level을 저장/조회한다. |
| `Port` | 핀 초기화 역할을 가진다. 학습용에서는 설정 확인 수준으로 시작한다. |
| `Ecu` | 100ms 주기 호출 순서를 관리한다. |

## 4. 주기 처리 순서

```text
Ecu_MainFunction_100ms()
  1. IoHwAb_UpdateInputs()
  2. Swc_LedControl_Runnable_100ms()
  3. IoHwAb_UpdateOutputs()
```

LED-only 단계에서는 입력 하드웨어가 없으므로 `IoHwAb_UpdateInputs()`는 no-op이다. 다음 Button 단계에서 입력 갱신 로직이 추가된다.

## 5. SWC 설계

`Swc_LedControl_Runnable_100ms()`는 다음 순서로 동작한다.

```text
Rte_Read_Rp_LedCommand()
  ↓
LedCommand 판단
  ↓
Rte_Write_Pp_LedState()
```

SWC는 `Dio_WriteChannel()`을 직접 호출하지 않는다.

## 6. IoHwAb 설계

`IoHwAb_UpdateOutputs()`는 다음 순서로 동작한다.

```text
Rte_Observe_LedState()
  ↓
active-high/active-low 변환
  ↓
Dio_WriteChannel(ECUC_DIO_CHANNEL_LED1, level)
```

## 7. 설정 설계

하드웨어 매핑은 다음 파일에서 관리한다.

```text
config/hardware_config.json
  ↓ generator or manual copy
generated/include/EcuC_Cfg.h
```

예시:

```c
#define ECUC_DIO_CHANNEL_LED1      (0u)
#define ECUC_DIO_LED1_PIN_NUMBER   (15u)
#define ECUC_DIO_LED1_ACTIVE_HIGH  (1u)
```
