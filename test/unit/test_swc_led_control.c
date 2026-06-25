#include <stdio.h>
#include "Std_Types.h"
#include "App_Types.h"
#include "Rte.h"
#include "Swc_LedControl.h"

static int failures = 0;

#define EXPECT_EQ_INT(expected, actual, name)                                      \
    do {                                                                          \
        int exp__ = (int)(expected);                                               \
        int act__ = (int)(actual);                                                 \
        if (exp__ != act__) {                                                      \
            printf("[FAIL] %s expected=%d actual=%d\n", (name), exp__, act__);     \
            failures++;                                                            \
        } else {                                                                   \
            printf("[PASS] %s\n", (name));                                        \
        }                                                                          \
    } while (0)

static void test_led_command_on_turns_led_state_on(void)
{
    Rte_Reset();
    Rte_Inject_LedCommand(LED_COMMAND_ON);

    Swc_LedControl_Runnable_100ms();

    EXPECT_EQ_INT(LED_STATE_ON,
                  Rte_Observe_LedState(),
                  "LED command ON -> LED state ON");
}

static void test_led_command_off_turns_led_state_off(void)
{
    Rte_Reset();
    Rte_Inject_LedCommand(LED_COMMAND_OFF);

    Swc_LedControl_Runnable_100ms();

    EXPECT_EQ_INT(LED_STATE_OFF,
                  Rte_Observe_LedState(),
                  "LED command OFF -> LED state OFF");
}

int main(void)
{
    test_led_command_on_turns_led_state_on();
    test_led_command_off_turns_led_state_off();

    if (failures == 0) {
        printf("unit tests passed\n");
        return 0;
    }

    printf("unit tests failed: %d\n", failures);
    return 1;
}
