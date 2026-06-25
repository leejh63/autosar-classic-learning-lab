# 03 ADC Input Next

Button 입력 이후 ADC 입력을 추가합니다.

## 목표 흐름

```text
Virtual ADC raw value
  ↓
Adc_ReadChannel()
  ↓
IoHwAb_UpdateInputs()
  ↓
RTE input buffer
  ↓
SWC policy
```

## 학습 포인트

- raw ADC 값과 application 값 분리
- threshold 요구사항 작성
- invalid/fault 정책 설계
- 단위 테스트와 모듈 테스트 분리
