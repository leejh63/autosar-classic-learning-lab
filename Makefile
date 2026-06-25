CC ?= gcc
CFLAGS ?= -std=c99 -Wall -Wextra -Werror
INCLUDES := -Iinclude -Iconfig -Igenerated/include -Isrc/app -Isrc/bsw -Isrc/mcal -Isrc/system -Isrc/vhw

BUILD_DIR := build

CONFIG_SRC := $(sort $(wildcard config/*.c))
VHW_SRC := $(sort $(wildcard src/vhw/*.c))
RTE_SRC := $(sort $(wildcard generated/src/*.c))
APP_SRC := $(sort $(wildcard src/app/*.c))
BSW_SRC := $(sort $(wildcard src/bsw/*.c))
MCAL_SRC := $(sort $(wildcard src/mcal/*.c))
SYSTEM_SRC := $(sort $(wildcard src/system/*.c))

COMMON_INFRA_SRC := $(CONFIG_SRC) $(VHW_SRC) $(MCAL_SRC)
APP_STACK_SRC := $(RTE_SRC) $(APP_SRC) $(BSW_SRC) $(SYSTEM_SRC)
COMPILE_SRC := $(COMMON_INFRA_SRC) $(APP_STACK_SRC)
COMPILE_OBJ := $(patsubst %.c,$(BUILD_DIR)/%.o,$(COMPILE_SRC))

VHW_TEST_SRC := $(sort $(wildcard test/vhw/*.c))
UNIT_TEST_SRC := $(sort $(wildcard test/unit/*.c))
MODULE_TEST_SRC := $(sort $(wildcard test/module/*.c))

VHW_TEST_BIN := $(patsubst test/vhw/%.c,$(BUILD_DIR)/vhw/%,$(VHW_TEST_SRC))
UNIT_TEST_BIN := $(patsubst test/unit/%.c,$(BUILD_DIR)/unit/%,$(UNIT_TEST_SRC))
MODULE_TEST_BIN := $(patsubst test/module/%.c,$(BUILD_DIR)/module/%,$(MODULE_TEST_SRC))

.PHONY: all compile test vhw unit module clean todo tree

all: compile

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

compile: $(COMPILE_OBJ)
	@echo "compile OK"

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/vhw/%: test/vhw/%.c $(COMMON_INFRA_SRC) | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

$(BUILD_DIR)/unit/%: test/unit/%.c $(RTE_SRC) $(APP_SRC) | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

$(BUILD_DIR)/module/%: test/module/%.c $(COMMON_INFRA_SRC) $(APP_STACK_SRC) | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

vhw: $(VHW_TEST_BIN)
	@set -e; for test_bin in $(VHW_TEST_BIN); do ./$$test_bin; done

unit: $(UNIT_TEST_BIN)
	@set -e; for test_bin in $(UNIT_TEST_BIN); do ./$$test_bin; done

module: $(MODULE_TEST_BIN)
	@set -e; for test_bin in $(MODULE_TEST_BIN); do ./$$test_bin; done

test: vhw unit module

todo:
	grep -R "TODO\|NEXT" -n docs traceability config generated include src test roadmap README.md 00_process autosar_theory || true

tree:
	find . -maxdepth 4 -type f | sort

clean:
	rm -rf $(BUILD_DIR)
