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

## RGB LED vertical slice

| Requirement ID | Design Section | Code File / Function | Test Case |
|---|---|---|---|
| REQ-RGB-F-001 | DESIGN-RGB 7 | `src/app/Swc_RgbLedControl.c` / `Swc_RgbLedControl_Runnable_100ms` | TC-RGB-U-001, TC-RGB-M-004 |
| REQ-RGB-F-002 | DESIGN-RGB 7 | `src/app/Swc_RgbLedControl.c` / `Swc_RgbLedControl_Runnable_100ms` | TC-RGB-U-002, TC-RGB-M-001 |
| REQ-RGB-F-003 | DESIGN-RGB 7 | `src/app/Swc_RgbLedControl.c` / `Swc_RgbLedControl_Runnable_100ms` | TC-RGB-U-003, TC-RGB-M-002 |
| REQ-RGB-F-004 | DESIGN-RGB 7 | `src/app/Swc_RgbLedControl.c` / `Swc_RgbLedControl_Runnable_100ms` | TC-RGB-U-004, TC-RGB-M-003 |
| REQ-RGB-F-005 | DESIGN-RGB 8 | `src/bsw/IoHwAb.c` / `IoHwAb_UpdateRgbOutputs`, `src/mcal/Dio.c` / `Dio_WriteChannel` | TC-RGB-M-001, TC-RGB-M-002, TC-RGB-M-003, TC-RGB-M-004 |
| REQ-RGB-F-006 | DESIGN-RGB 8 | `src/bsw/IoHwAb.c` / `led_state_to_dio_level`, `config/Board_Cfg.c` / `Board_DioChannels` | TC-RGB-M-001, TC-RGB-M-002, TC-RGB-M-003, TC-RGB-M-004 |
| REQ-RGB-NF-001 | DESIGN-RGB 3, 7 | `src/app/Swc_RgbLedControl.c` | code review |
| REQ-RGB-NF-002 | DESIGN-RGB 9 | `test/module/test_rgb_led_control_ecu.c`, `src/vhw/VirtualHw.c` | TC-RGB-M-001, TC-RGB-M-002, TC-RGB-M-003, TC-RGB-M-004 |

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
