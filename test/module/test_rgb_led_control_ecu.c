#include <stdio.h>
#include "Std_Types.h"
#include "App_Types.h"
#include "Board_Cfg.h"
#include "Ecu.h"
#include "Rte.h"
#include "Dio.h"

static int failures = 0;

static boolean expected_hw_level(uint8_t channel, LedStateType state)
{
    const Board_DioChannelConfigType *cfg = &Board_DioChannels[channel];
    boolean active_level = (cfg->active_polarity == VHW_ACTIVE_HIGH) ? TRUE : FALSE;
    boolean inactive_level = (active_level == TRUE) ? FALSE : TRUE;

    return (state == LED_STATE_ON) ? active_level : inactive_level;
}

static void expect_rgb_dio_levels(RgbLedStateType expected_state, const char *name)
{
    boolean red_level = FALSE;
    boolean blue_level = FALSE;
    boolean green_level = FALSE;
    boolean expected_red_level = expected_hw_level(BOARD_DIO_LED1, expected_state.red);
    boolean expected_blue_level = expected_hw_level(BOARD_DIO_LED2, expected_state.blue);
    boolean expected_green_level = expected_hw_level(BOARD_DIO_LED3, expected_state.green);

    if ((Dio_ReadChannel(BOARD_DIO_LED1, &red_level) != E_OK) ||
        (Dio_ReadChannel(BOARD_DIO_LED2, &blue_level) != E_OK) ||
        (Dio_ReadChannel(BOARD_DIO_LED3, &green_level) != E_OK)) {
        printf("[FAIL] %s DIO read failed\n", name);
        failures++;
        return;
    }

    if ((expected_red_level != red_level) ||
        (expected_blue_level != blue_level) ||
        (expected_green_level != green_level)) {
        printf("[FAIL] %s expected=(red=%d blue=%d green=%d) actual=(red=%d blue=%d green=%d)\n",
               name,
               (int)expected_red_level,
               (int)expected_blue_level,
               (int)expected_green_level,
               (int)red_level,
               (int)blue_level,
               (int)green_level);
        failures++;
    } else {
        printf("[PASS] %s\n", name);
    }
}

static void run_rgb_command(RgbCommandType command)
{
    Ecu_ResetForTest();
    Rte_Inject_RgbCommand(command);

    Ecu_MainFunction_Rgb_100ms();
}

static void test_rgb_red_command_writes_red_channel_active(void)
{
    const RgbLedStateType expected_state = {
        .red = LED_STATE_ON,
        .blue = LED_STATE_OFF,
        .green = LED_STATE_OFF
    };

    run_rgb_command(RGB_COMMAND_RED);

    expect_rgb_dio_levels(expected_state, "RGB RED command -> DIO levels");
}

static void test_rgb_blue_command_writes_blue_channel_active(void)
{
    const RgbLedStateType expected_state = {
        .red = LED_STATE_OFF,
        .blue = LED_STATE_ON,
        .green = LED_STATE_OFF
    };

    run_rgb_command(RGB_COMMAND_BLUE);

    expect_rgb_dio_levels(expected_state, "RGB BLUE command -> DIO levels");
}

static void test_rgb_green_command_writes_green_channel_active(void)
{
    const RgbLedStateType expected_state = {
        .red = LED_STATE_OFF,
        .blue = LED_STATE_OFF,
        .green = LED_STATE_ON
    };

    run_rgb_command(RGB_COMMAND_GREEN);

    expect_rgb_dio_levels(expected_state, "RGB GREEN command -> DIO levels");
}

static void test_rgb_off_command_writes_all_channels_inactive(void)
{
    const RgbLedStateType expected_state = {
        .red = LED_STATE_OFF,
        .blue = LED_STATE_OFF,
        .green = LED_STATE_OFF
    };

    run_rgb_command(RGB_COMMAND_OFF);

    expect_rgb_dio_levels(expected_state, "RGB OFF command -> DIO levels");
}

int main(void)
{
    (void)Ecu_Init();

    test_rgb_red_command_writes_red_channel_active();
    test_rgb_blue_command_writes_blue_channel_active();
    test_rgb_green_command_writes_green_channel_active();
    test_rgb_off_command_writes_all_channels_inactive();

    if (failures == 0) {
        printf("rgb module tests passed\n");
        return 0;
    }

    printf("rgb module tests failed: %d\n", failures);
    return 1;
}
