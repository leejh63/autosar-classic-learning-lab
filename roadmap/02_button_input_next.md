# 02 Button Input Next

LED 단독 제어가 끝나면 버튼 입력을 추가합니다.

## 목표 흐름

```text
Virtual Button DIO input
  ↓
IoHwAb_UpdateInputs()
  ↓
RTE input buffer
  ↓
SWC_LedControl
  ↓
RTE output buffer
  ↓
IoHwAb_UpdateOutputs()
  ↓
DIO LED output
```

## 추가할 요구사항 예시

- 버튼이 눌리면 LED ON
- 버튼이 눌리지 않으면 LED OFF
- active-low 버튼 설정 반영

## 추가할 파일 변경

- `config/hardware_config.json`: `DIO_CH_BUTTON1` 활성화
- `generated/include/EcuC_Cfg.h`: button channel define 추가
- `generated/include/Rte.h`: `Rte_Read_Rp_ButtonState()` 추가
- `generated/src/Rte.c`: button input buffer 추가
- `src/bsw/IoHwAb.c`: `Dio_ReadChannel()`로 버튼 입력 읽기
- `src/app/Swc_LedControl.c`: LED command 대신 button state 기반 판단으로 확장
