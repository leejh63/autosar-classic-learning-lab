CC ?= gcc
CFLAGS ?= -std=c99 -Wall -Wextra -Werror
INCLUDES := -Iinclude -Igenerated/include -Isrc/app -Isrc/bsw -Isrc/mcal -Isrc/system

BUILD_DIR := build

RTE_SRC := generated/src/Rte.c
SWC_SRC := src/app/Swc_LedControl.c
BSW_SRC := src/bsw/IoHwAb.c
MCAL_SRC := src/mcal/Dio.c src/mcal/Port.c src/mcal/Adc.c
SYSTEM_SRC := src/system/Ecu.c

UNIT_BIN := $(BUILD_DIR)/unit_test
MODULE_BIN := $(BUILD_DIR)/module_test

.PHONY: all compile test unit module clean todo gen

all: compile

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

compile: $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(RTE_SRC) -o $(BUILD_DIR)/Rte.o
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SWC_SRC) -o $(BUILD_DIR)/Swc_LedControl.o
	$(CC) $(CFLAGS) $(INCLUDES) -c src/mcal/Dio.c -o $(BUILD_DIR)/Dio.o
	$(CC) $(CFLAGS) $(INCLUDES) -c src/mcal/Port.c -o $(BUILD_DIR)/Port.o
	$(CC) $(CFLAGS) $(INCLUDES) -c src/mcal/Adc.c -o $(BUILD_DIR)/Adc.o
	$(CC) $(CFLAGS) $(INCLUDES) -c src/bsw/IoHwAb.c -o $(BUILD_DIR)/IoHwAb.o
	$(CC) $(CFLAGS) $(INCLUDES) -c src/system/Ecu.c -o $(BUILD_DIR)/Ecu.o
	@echo "compile OK. Tests will fail until TODOs are implemented."

$(UNIT_BIN): $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) test/unit/test_swc_led_control.c $(RTE_SRC) $(SWC_SRC) -o $(UNIT_BIN)

$(MODULE_BIN): $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) test/module/test_led_control_ecu.c $(RTE_SRC) $(SWC_SRC) $(BSW_SRC) $(MCAL_SRC) $(SYSTEM_SRC) -o $(MODULE_BIN)

unit: $(UNIT_BIN)
	./$(UNIT_BIN)

module: $(MODULE_BIN)
	./$(MODULE_BIN)

test: unit module

gen:
	python3 tools/generate_from_config.py

todo:
	grep -R "TODO" -n docs traceability config tools generated include src test roadmap README.md 00_process || true

clean:
	rm -rf $(BUILD_DIR)
