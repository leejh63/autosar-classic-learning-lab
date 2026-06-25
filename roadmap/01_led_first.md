# 01 LED-first

현재 구현되어 있는 baseline이다.

목표:

```text
LedCommand
  ↓
SWC_LedControl
  ↓
LedState
  ↓
IoHwAb
  ↓
Dio
  ↓
VirtualHw LED1 level
```

확인 명령:

```sh
make unit
make module
```
