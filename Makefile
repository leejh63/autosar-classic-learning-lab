CC ?= gcc
CFLAGS ?= -std=c99 -Wall -Wextra -Werror
INCLUDES := -Iinclude -Iconfig -Igenerated/include -Isrc/app -Isrc/bsw -Isrc/mcal -Isrc/system -Isrc/vhw

BUILD_DIR := build

CONFIG_SRC := config/Board_Cfg.c
VHW_SRC := src/vhw/VirtualHw.c
RTE_SRC := generated/src/Rte.c
SWC_SRC := src/app/Swc_LedControl.c
BSW_SRC := src/bsw/IoHwAb.c
MCAL_SRC := src/mcal/Dio.c src/mcal/Port.c src/mcal/Adc.c src/mcal/Pwm.c src/mcal/Can.c src/mcal/Lin.c
SYSTEM_SRC := src/system/Ecu.c

COMMON_INFRA_SRC := $(CONFIG_SRC) $(VHW_SRC) $(MCAL_SRC)
APP_STACK_SRC := $(RTE_SRC) $(SWC_SRC) $(BSW_SRC) $(SYSTEM_SRC)

VHW_BIN := $(BUILD_DIR)/virtual_hw_test
UNIT_BIN := $(BUILD_DIR)/unit_test
MODULE_BIN := $(BUILD_DIR)/module_test

.PHONY: all compile test vhw unit module clean todo tree

all: compile

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

compile: $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(CONFIG_SRC) -o $(BUILD_DIR)/Board_Cfg.o
	$(CC) $(CFLAGS) $(INCLUDES) -c $(VHW_SRC) -o $(BUILD_DIR)/VirtualHw.o
	$(CC) $(CFLAGS) $(INCLUDES) -c $(RTE_SRC) -o $(BUILD_DIR)/Rte.o
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SWC_SRC) -o $(BUILD_DIR)/Swc_LedControl.o
	$(CC) $(CFLAGS) $(INCLUDES) -c src/bsw/IoHwAb.c -o $(BUILD_DIR)/IoHwAb.o
	$(CC) $(CFLAGS) $(INCLUDES) -c src/mcal/Dio.c -o $(BUILD_DIR)/Dio.o
	$(CC) $(CFLAGS) $(INCLUDES) -c src/mcal/Port.c -o $(BUILD_DIR)/Port.o
	$(CC) $(CFLAGS) $(INCLUDES) -c src/mcal/Adc.c -o $(BUILD_DIR)/Adc.o
	$(CC) $(CFLAGS) $(INCLUDES) -c src/mcal/Pwm.c -o $(BUILD_DIR)/Pwm.o
	$(CC) $(CFLAGS) $(INCLUDES) -c src/mcal/Can.c -o $(BUILD_DIR)/Can.o
	$(CC) $(CFLAGS) $(INCLUDES) -c src/mcal/Lin.c -o $(BUILD_DIR)/Lin.o
	$(CC) $(CFLAGS) $(INCLUDES) -c src/system/Ecu.c -o $(BUILD_DIR)/Ecu.o
	@echo "compile OK"

$(VHW_BIN): $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) test/vhw/test_virtual_hw.c $(COMMON_INFRA_SRC) -o $(VHW_BIN)

$(UNIT_BIN): $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) test/unit/test_swc_led_control.c $(RTE_SRC) $(SWC_SRC) -o $(UNIT_BIN)

$(MODULE_BIN): $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) test/module/test_led_control_ecu.c $(COMMON_INFRA_SRC) $(APP_STACK_SRC) -o $(MODULE_BIN)

vhw: $(VHW_BIN)
	./$(VHW_BIN)

unit: $(UNIT_BIN)
	./$(UNIT_BIN)

module: $(MODULE_BIN)
	./$(MODULE_BIN)

test: vhw unit module

todo:
	grep -R "TODO\|NEXT" -n docs traceability config generated include src test roadmap README.md 00_process autosar_theory || true

tree:
	find . -maxdepth 4 -type f | sort

clean:
	rm -rf $(BUILD_DIR)
