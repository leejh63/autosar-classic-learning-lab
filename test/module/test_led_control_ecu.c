#include <stdio.h>
#include "Std_Types.h"
#include "App_Types.h"
#include "EcuC_Cfg.h"
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
    return (ECUC_DIO_LED1_ACTIVE_HIGH != 0u) ? TRUE : FALSE;
}

static boolean expected_hw_level_for_led_off(void)
{
    return (ECUC_DIO_LED1_ACTIVE_HIGH != 0u) ? FALSE : TRUE;
}

static void test_ecu_led_command_on_writes_dio_active_level(void)
{
    boolean led_level = FALSE;

    Ecu_ResetForTest();
    Rte_Inject_LedCommand(LED_COMMAND_ON);

    Ecu_MainFunction_100ms();

    (void)Dio_ReadChannel(ECUC_DIO_CHANNEL_LED1, &led_level);
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

    (void)Dio_ReadChannel(ECUC_DIO_CHANNEL_LED1, &led_level);
    EXPECT_EQ_INT(expected_hw_level_for_led_off(),
                  led_level,
                  "ECU LED command OFF -> DIO inactive level");
}

int main(void)
{
    test_ecu_led_command_on_writes_dio_active_level();
    test_ecu_led_command_off_writes_dio_inactive_level();

    if (failures == 0) {
        printf("module tests passed\n");
        return 0;
    }

    printf("module tests failed: %d\n", failures);
    return 1;
}
