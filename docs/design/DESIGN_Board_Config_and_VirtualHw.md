# Board Configuration and Virtual Hardware Design

문서 ID: DESIGN-VHW-001  
상태: 초안

## 1. 설계 목적

이 설계의 목적은 실제 보드 없이도 일반적인 임베디드 모듈의 설정과 상태 변화를 학습할 수 있도록 `Board_Cfg.c`와 `VirtualHw`를 분리하는 것이다.

## 2. 계층 위치

```text
IoHwAb
  ↓
MCAL-like Driver: Dio / Adc / Pwm / Can / Lin
  ↓
VirtualHw
  ↓
Board_Cfg.c
```

VirtualHw는 AUTOSAR Classic 표준 계층이 아니다. 이 프로젝트에서만 사용하는 host simulation layer이다.

## 3. 설정 책임

| 파일 | 책임 |
|---|---|
| `config/Board_Cfg.h` | symbolic ID와 설정 배열 선언 |
| `config/Board_Cfg.c` | 가상 보드의 DIO/ADC/PWM/CAN/LIN 설정 정의 |
| `include/Board_Types.h` | 설정 구조체 타입 정의 |

## 4. VirtualHw 책임

| 함수 그룹 | 책임 |
|---|---|
| `VirtualHw_Init`, `VirtualHw_Reset` | 설정 테이블 기반 내부 상태 초기화 |
| `VirtualHw_WriteDioChannel` | DIO output 상태 변경 |
| `VirtualHw_InjectDioInput` | DIO input 상태 주입 |
| `VirtualHw_ReadAdcChannel`, `VirtualHw_InjectAdcRawValue` | ADC raw 상태 읽기/주입 |
| `VirtualHw_SetPwmDuty`, `VirtualHw_GetPwmDuty` | PWM duty 상태 저장/조회 |
| `VirtualHw_SendCanPdu`, `VirtualHw_ReadLastCanTx` | CAN TX capture |
| `VirtualHw_SendLinFrame`, `VirtualHw_ReadLastLinTx` | LIN TX capture |

## 5. MCAL-like driver 책임

MCAL-like driver는 VirtualHw에 대한 thin wrapper로 시작한다.

| Driver | VirtualHw 호출 |
|---|---|
| `Dio.c` | `VirtualHw_WriteDioChannel`, `VirtualHw_ReadDioChannel` |
| `Adc.c` | `VirtualHw_ReadAdcChannel` |
| `Pwm.c` | `VirtualHw_SetPwmDuty` |
| `Can.c` | `VirtualHw_SendCanPdu` |
| `Lin.c` | `VirtualHw_SendLinFrame` |

## 6. 확장 규칙

새 하드웨어를 추가할 때는 다음 순서를 따른다.

```text
Board_Cfg.c 설정 추가
  ↓
VirtualHw 상태 추가 또는 기존 상태 확장
  ↓
MCAL-like driver API 확인
  ↓
IoHwAb 변환 로직 추가
  ↓
RTE/SWC 요구사항 추가
  ↓
테스트 추가
```
