# AUTOSAR Classic LED-first Learning Lab

이 프로젝트는 실제 AUTOSAR Classic 구현체가 아니라, AUTOSAR Classic의 구조를 작게 흉내 낸 학습용 TODO skeleton입니다.

목표는 처음부터 복잡한 Speed Warning ECU를 만들지 않고, 가장 단순한 LED 출력부터 시작해서 Button, ADC, CAN/LIN 순서로 모듈을 추가하는 것입니다.

## 01 단계: LED Control

현재 활성 학습 범위는 아래 흐름입니다.

```text
External/Test LED Command
  ↓
RTE input buffer
  ↓
SWC_LedControl
  ↓
RTE output buffer
  ↓
IoHwAb
  ↓
Dio
  ↓
Virtual LED hardware level
```

## 핵심 규칙

- SWC는 `Dio_WriteChannel()`을 직접 호출하지 않습니다.
- SWC는 `Rte_Read_*()`, `Rte_Write_*()`만 사용합니다.
- 실제 핀/채널 매핑은 `config/hardware_config.json`과 `generated/include/EcuC_Cfg.h`에서 관리합니다.
- 보드가 없어도 `Dio_ReadChannel()`로 가상 LED 출력 상태를 확인합니다.

## 사용 방법

```sh
make compile
make test
```

처음에는 테스트가 실패하는 것이 정상입니다. TODO를 채우면서 테스트가 하나씩 통과하도록 만드는 구조입니다.

설정을 바꿨다면:

```sh
make gen
make clean
make compile
```

## 추천 구현 순서

1. `docs/requirements/REQ_LED_Control_ECU.md` 확인/수정
2. `docs/design/DESIGN_LED_Control_ECU.md` 확인/수정
3. `generated/src/Rte.c` TODO 구현
4. `src/app/Swc_LedControl.c` TODO 구현
5. `make unit` 통과
6. `src/mcal/Dio.c`, `src/mcal/Port.c` TODO 구현
7. `src/bsw/IoHwAb.c`, `src/system/Ecu.c` TODO 구현
8. `make module` 통과
9. `traceability/TRACE_MATRIX.md` 채우기
10. `roadmap/02_button_input_next.md`로 확장

## 확장 순서

```text
01 LED output
  ↓
02 Button input controls LED
  ↓
03 ADC input controls LED policy or brightness-like state
  ↓
04 CAN TX sends LED state
  ↓
05 LIN/ADC/Button combination
```
