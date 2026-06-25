#include <stdio.h>
#include "Std_Types.h"
#include "App_Types.h"
#include "Rte.h"
#include "Swc_RgbLedControl.h"

static int failures = 0;

static void test_check_func(RgbLedStateType expected,
                            RgbLedStateType actual,
                            const char *name)
{
    if ((expected.red != actual.red) ||
        (expected.blue != actual.blue) ||
        (expected.green != actual.green)) {
        printf("[FAIL] %s expected=(red=%d blue=%d green=%d) actual=(red=%d blue=%d green=%d)\n",
               name,
               (int)expected.red,
               (int)expected.blue,
               (int)expected.green,
               (int)actual.red,
               (int)actual.blue,
               (int)actual.green);
        failures++;
    } else {
        printf("[PASS] %s\n", name);
    }
}

static const RgbLedStateType test_state_off = {
    .red = LED_STATE_OFF,
    .blue = LED_STATE_OFF,
    .green = LED_STATE_OFF
};

static const RgbLedStateType test_state_red = {
    .red = LED_STATE_ON,
    .blue = LED_STATE_OFF,
    .green = LED_STATE_OFF
};

static const RgbLedStateType test_state_blue = {
    .red = LED_STATE_OFF,
    .blue = LED_STATE_ON,
    .green = LED_STATE_OFF
};

static const RgbLedStateType test_state_green = {
    .red = LED_STATE_OFF,
    .blue = LED_STATE_OFF,
    .green = LED_STATE_ON
};

static void test_rgb_command_off(void)
{
    Rte_Reset();
    Rte_Inject_RgbCommand(RGB_COMMAND_OFF);

    Swc_RgbLedControl_Runnable_100ms();

    test_check_func(test_state_off,
                  Rte_Observe_RgbLedState(),
                  "RGB command OFF");
}

static void test_rgb_command_red(void)
{
    Rte_Reset();
    Rte_Inject_RgbCommand(RGB_COMMAND_RED);

    Swc_RgbLedControl_Runnable_100ms();

    test_check_func(test_state_red,
                  Rte_Observe_RgbLedState(),
                  "RGB command RED");
}

static void test_rgb_command_blue(void)
{
    Rte_Reset();
    Rte_Inject_RgbCommand(RGB_COMMAND_BLUE);

    Swc_RgbLedControl_Runnable_100ms();

    test_check_func(test_state_blue,
                  Rte_Observe_RgbLedState(),
                  "RGB command BLUE");
}

static void test_rgb_command_green(void)
{
    Rte_Reset();
    Rte_Inject_RgbCommand(RGB_COMMAND_GREEN);

    Swc_RgbLedControl_Runnable_100ms();

    test_check_func(test_state_green,
                  Rte_Observe_RgbLedState(),
                  "RGB command GREEN");
}

static void test_rgb_command_blue_to_green(void)
{
    Rte_Reset();
    Rte_Inject_RgbCommand(RGB_COMMAND_BLUE);

    Swc_RgbLedControl_Runnable_100ms();

    test_check_func(test_state_blue,
                Rte_Observe_RgbLedState(),
                "RGB command BLUE before GREEN");

    Rte_Inject_RgbCommand(RGB_COMMAND_GREEN);

    Swc_RgbLedControl_Runnable_100ms();

    test_check_func(test_state_green,
                  Rte_Observe_RgbLedState(),
                  "RGB command GREEN");
}

int main(void)
{
    test_rgb_command_off();
    test_rgb_command_red();
    test_rgb_command_blue();
    test_rgb_command_green();
    test_rgb_command_blue_to_green();

    if (failures == 0) {
        printf("unit tests passed\n");
        return 0;
    }

    printf("unit tests failed: %d\n", failures);
    return 1;
}
