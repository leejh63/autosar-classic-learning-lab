# 04 CAN TX Next

LED 상태를 CAN 메시지로 송신하는 확장 단계입니다.

## 목표 흐름

```text
SWC output
  ↓
RTE
  ↓
Mini COM
  ↓
Mini PduR
  ↓
Mini CanIf
  ↓
Virtual Can Driver
```

처음에는 실제 CAN 드라이버가 아니라 가상 CAN Tx buffer에 PDU가 기록되는지 테스트합니다.
