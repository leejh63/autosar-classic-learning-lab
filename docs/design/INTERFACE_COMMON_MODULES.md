# Common Module Interface Notes

## 1. DIO

| API | 설명 |
|---|---|
| `Dio_WriteChannel(channel, level)` | output channel level write |
| `Dio_ReadChannel(channel, &level)` | channel level read |
| `Dio_InjectInputLevel(channel, level)` | test-only input injection |

## 2. ADC

| API | 설명 |
|---|---|
| `Adc_ReadChannel(channel, &raw)` | ADC raw value read |
| `Adc_InjectRawValue(channel, raw)` | test-only raw value injection |

## 3. PWM

| API | 설명 |
|---|---|
| `Pwm_SetDutyCycle(channel, duty_permille)` | 0~1000 duty setting |
| `Pwm_GetDutyCycle(channel, &duty_permille)` | stored duty read |

## 4. CAN

| API | 설명 |
|---|---|
| `Can_WritePdu(pdu_id, data, length)` | virtual CAN TX capture |
| `Can_ReadLastTxPdu(pdu_id, data, &length, &valid)` | last TX observation |

## 5. LIN

| API | 설명 |
|---|---|
| `Lin_SendFrame(frame_id, data, length)` | virtual LIN TX capture |
| `Lin_ReadLastTxFrame(frame_id, data, &length, &valid)` | last TX observation |
