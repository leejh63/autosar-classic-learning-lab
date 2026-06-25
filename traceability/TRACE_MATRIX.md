# Traceability Matrix

| Requirement ID | Design Section | Code File | Test Case |
|---|---|---|---|
| REQ-LED-F-001 | DESIGN 5 | `src/app/Swc_LedControl.c` | TC-LED-U-001 |
| REQ-LED-F-002 | DESIGN 5 | `src/app/Swc_LedControl.c` | TC-LED-U-002 |
| REQ-LED-F-003 | DESIGN 6 | `src/bsw/IoHwAb.c`, `src/mcal/Dio.c` | TC-LED-M-001, TC-LED-M-002 |
| REQ-LED-F-004 | DESIGN 6, 7 | `src/bsw/IoHwAb.c`, `generated/include/EcuC_Cfg.h` | TC-LED-M-001, TC-LED-M-002 |
| REQ-LED-NF-001 | DESIGN 3, 5 | `src/app/Swc_LedControl.c` | code review |
| REQ-LED-NF-002 | DESIGN 7 | `src/mcal/Dio.c`, `test/module/test_led_control_ecu.c` | TC-LED-M-001, TC-LED-M-002 |
