# 04 PWM Next

PWM을 사용해 LED dimmer 또는 buzzer duty를 제어하는 단계다.

예시 흐름:

```text
SWC duty request
  ↓
RTE
  ↓
IoHwAb
  ↓
Pwm_SetDutyCycle(BOARD_PWM_LED_DIMMER, duty_permille)
  ↓
VirtualHw PWM duty state
```

주의:

- duty는 0~1000 permille로 제한한다.
- brightness 같은 application 개념과 PWM duty 변환은 IoHwAb 또는 별도 service에서 담당한다.
