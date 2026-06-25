# 03 ADC Input Next

ADC 입력으로 LED 정책을 바꾸는 단계다.

예시 요구사항:

```text
REQ-ADC-F-001:
POT1 raw value가 3000 이상이면 LED 상태는 ON이어야 한다.

REQ-ADC-F-002:
POT1 raw value가 3000 미만이면 LED 상태는 OFF이어야 한다.
```

추가 흐름:

```text
VirtualHw ADC raw
  ↓
Adc_ReadChannel(BOARD_ADC_POT1)
  ↓
IoHwAb_UpdateInputs()
  ↓
RTE AdcSignal
  ↓
SWC policy
```
