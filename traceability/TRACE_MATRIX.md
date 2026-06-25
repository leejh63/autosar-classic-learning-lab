# Traceability Matrix

## LED vertical slice

| Requirement ID | Design Section | Code File / Function | Test Case |
|---|---|---|---|
| REQ-LED-F-001 | DESIGN-LED 5 | `src/app/Swc_LedControl.c` | TC-LED-U-001 |
| REQ-LED-F-002 | DESIGN-LED 5 | `src/app/Swc_LedControl.c` | TC-LED-U-002 |
| REQ-LED-F-003 | DESIGN-LED 6 | `src/bsw/IoHwAb.c`, `src/mcal/Dio.c`, `src/vhw/VirtualHw.c` | TC-LED-M-001, TC-LED-M-002 |
| REQ-LED-F-004 | DESIGN-LED 6 | `config/Board_Cfg.c`, `src/bsw/IoHwAb.c` | TC-LED-M-001, TC-LED-M-002 |
| REQ-LED-NF-001 | DESIGN-LED 3, 5 | `src/app/Swc_LedControl.c` | code review |
| REQ-LED-NF-002 | DESIGN-VHW 2~5 | `src/vhw/VirtualHw.c`, `test/module/test_led_control_ecu.c` | TC-LED-M-001, TC-LED-M-002 |

## Virtual hardware model

| Requirement ID | Design Section | Code File / Function | Test Case |
|---|---|---|---|
| REQ-VHW-F-001 | DESIGN-VHW 3, 4 | `config/Board_Cfg.c`, `src/vhw/VirtualHw.c` | TC-VHW-INIT |
| REQ-VHW-F-002 | DESIGN-VHW 4, 5 | `src/mcal/Dio.c`, `src/vhw/VirtualHw.c` | TC-VHW-DIO-001 |
| REQ-VHW-F-003 | DESIGN-VHW 4, 5 | `src/mcal/Dio.c`, `src/vhw/VirtualHw.c` | TC-VHW-DIO-002 |
| REQ-VHW-F-004 | DESIGN-VHW 4, 5 | `src/mcal/Adc.c`, `src/vhw/VirtualHw.c` | TC-VHW-ADC-001 |
| REQ-VHW-F-005 | DESIGN-VHW 4, 5 | `src/mcal/Pwm.c`, `src/vhw/VirtualHw.c` | TC-VHW-PWM-001 |
| REQ-VHW-F-006 | DESIGN-VHW 4, 5 | `src/mcal/Can.c`, `src/vhw/VirtualHw.c` | TC-VHW-CAN-001 |
| REQ-VHW-F-007 | DESIGN-VHW 4, 5 | `src/mcal/Lin.c`, `src/vhw/VirtualHw.c` | TC-VHW-LIN-001 |
