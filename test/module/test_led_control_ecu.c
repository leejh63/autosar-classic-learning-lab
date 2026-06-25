#include <stdio.h>
#include "Std_Types.h"
#include "App_Types.h"
#include "Board_Cfg.h"
#include "Ecu.h"
#include "Rte.h"
#include "Dio.h"

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

static boolean expected_hw_level_for_led_on(void)
{
    const Board_DioChannelConfigType *led_cfg = &Board_DioChannels[BOARD_DIO_LED1];
    return (led_cfg->active_polarity == VHW_ACTIVE_HIGH) ? TRUE : FALSE;
}

static boolean expected_hw_level_for_led_off(void)
{
    return (expected_hw_level_for_led_on() == TRUE) ? FALSE : TRUE;
}

static void test_ecu_led_command_on_writes_dio_active_level(void)
{
    boolean led_level = FALSE;

    Ecu_ResetForTest();
    Rte_Inject_LedCommand(LED_COMMAND_ON);

    Ecu_MainFunction_100ms();

    (void)Dio_ReadChannel(BOARD_DIO_LED1, &led_level);
    EXPECT_EQ_INT(expected_hw_level_for_led_on(),
                  led_level,
                  "ECU LED command ON -> DIO active level");
}

static void test_ecu_led_command_off_writes_dio_inactive_level(void)
{
    boolean led_level = TRUE;

    Ecu_ResetForTest();
    Rte_Inject_LedCommand(LED_COMMAND_OFF);

    Ecu_MainFunction_100ms();

    (void)Dio_ReadChannel(BOARD_DIO_LED1, &led_level);
    EXPECT_EQ_INT(expected_hw_level_for_led_off(),
                  led_level,
                  "ECU LED command OFF -> DIO inactive level");
}

int main(void)
{
    (void)Ecu_Init();

    test_ecu_led_command_on_writes_dio_active_level();
    test_ecu_led_command_off_writes_dio_inactive_level();

    if (failures == 0) {
        printf("module tests passed\n");
        return 0;
    }

    printf("module tests failed: %d\n", failures);
    return 1;
}
