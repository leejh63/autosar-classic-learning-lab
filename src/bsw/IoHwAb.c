#include "IoHwAb.h"
#include "Rte.h"
#include "Dio.h"
#include "Board_Cfg.h"

static boolean led_state_to_dio_level(LedStateType state, uint8_t channel)
{
    const Board_DioChannelConfigType *led_cfg = &Board_DioChannels[channel];
    boolean active_level = (led_cfg->active_polarity == VHW_ACTIVE_HIGH) ? TRUE : FALSE;
    boolean inactive_level = (active_level == TRUE) ? FALSE : TRUE;

    return (state == LED_STATE_ON) ? active_level : inactive_level;
}

void IoHwAb_UpdateInputs(void)
{
    /*
     * LED-only 단계에서는 입력 하드웨어가 없다.
     * 다음 단계에서 BUTTON1을 읽어 RTE input으로 올리는 경로를 추가한다.
     */
}

void IoHwAb_UpdateOutputs(void)
{
    LedStateType state = LED_STATE_OFF;
    boolean dio_level = FALSE;

    if (Rte_Bsw_Read_LedState(&state) != E_OK) {
        return;
    }

    dio_level = led_state_to_dio_level(state, BOARD_DIO_LED1);
    (void)Dio_WriteChannel(BOARD_DIO_LED1, dio_level);
}

void IoHwAb_UpdateRgbOutputs(void)
{
    RgbLedStateType state = {
        .red = LED_STATE_OFF,
        .blue = LED_STATE_OFF,
        .green = LED_STATE_OFF
    };

    if (Rte_Bsw_Read_RgbLedState(&state) != E_OK) {
        return;
    }

    (void)Dio_WriteChannel(BOARD_DIO_LED1,
                           led_state_to_dio_level(state.red, BOARD_DIO_LED1));
    (void)Dio_WriteChannel(BOARD_DIO_LED2,
                           led_state_to_dio_level(state.blue, BOARD_DIO_LED2));
    (void)Dio_WriteChannel(BOARD_DIO_LED3,
                           led_state_to_dio_level(state.green, BOARD_DIO_LED3));
}
