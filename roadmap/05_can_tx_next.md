# 05 CAN TX Next

LED 상태를 CAN PDU로 송신하는 단계다.

학습용 단순 흐름:

```text
LedState
  ↓
RTE / IoHwAb or Service
  ↓
Can_WritePdu(BOARD_CAN_PDU_LED_STATUS, data, length)
  ↓
VirtualHw CAN TX capture
```

확장 시에는 Mini COM, Mini PduR, Mini CanIf를 추가할 수 있다. 단, LED/Button/ADC가 충분히 익숙해진 뒤에 추가한다.
