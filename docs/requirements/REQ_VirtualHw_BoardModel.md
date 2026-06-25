# Virtual Hardware Board Model Requirements

문서 ID: REQ-VHW-001  
상태: 초안

## 1. 목적

본 문서는 실제 보드 없이 DIO, ADC, PWM, CAN, LIN 동작을 학습하기 위한 가상 하드웨어 모델 요구사항을 정의한다.

## 2. 기능 요구사항

### REQ-VHW-F-001: Board 설정 기반 초기화

VirtualHw는 `Board_Cfg.c`의 설정 테이블을 기반으로 DIO, ADC, PWM, CAN, LIN 내부 상태를 초기화해야 한다.

### REQ-VHW-F-002: DIO output write

DIO output channel에 대한 write 요청은 해당 channel의 가상 level 상태에 반영되어야 한다.

### REQ-VHW-F-003: DIO input injection

DIO input channel은 테스트 코드에서 주입한 level을 저장하고 읽을 수 있어야 한다.

### REQ-VHW-F-004: ADC raw injection

ADC channel은 테스트 코드에서 주입한 raw value를 저장하고 읽을 수 있어야 한다.

### REQ-VHW-F-005: PWM duty storage

PWM channel은 duty permille 값을 저장하고 읽을 수 있어야 한다.

### REQ-VHW-F-006: CAN TX capture

CAN PDU 전송 요청은 마지막 TX data, length, valid 상태로 저장되어야 한다.

### REQ-VHW-F-007: LIN TX capture

LIN frame 전송 요청은 마지막 TX data, length, valid 상태로 저장되어야 한다.
