# 06 LIN Next

LIN frame을 통해 센서 값을 받거나 응답 frame을 저장하는 단계다.

학습용 단순 흐름:

```text
LIN frame data
  ↓
VirtualHw LIN RX/TX capture
  ↓
Lin driver
  ↓
IoHwAb or service
  ↓
RTE/SWC
```

처음에는 scheduler, checksum, NAD 같은 세부사항보다 frame id, dlc, data 흐름부터 학습한다.
