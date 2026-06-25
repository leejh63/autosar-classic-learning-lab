# 02 Button Input Next

다음 단계 목표는 버튼 입력으로 LED를 제어하는 것이다.

예시 요구사항:

```text
REQ-BTN-F-001:
BUTTON1이 pressed 상태이면 LedCommand는 ON이어야 한다.

REQ-BTN-F-002:
BUTTON1이 released 상태이면 LedCommand는 OFF이어야 한다.
```

추가 흐름:

```text
VirtualHw BUTTON1 level
  ↓
Dio_ReadChannel(BOARD_DIO_BUTTON1)
  ↓
IoHwAb_UpdateInputs()
  ↓
RTE ButtonState 또는 LedCommand input
  ↓
SWC_LedControl
```

주의:

- SWC가 `Dio_ReadChannel()`을 직접 부르면 안 된다.
- active-low 버튼은 IoHwAb에서 pressed/released로 변환한다.
