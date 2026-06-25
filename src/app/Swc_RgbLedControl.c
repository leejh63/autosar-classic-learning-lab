#include "Swc_RgbLedControl.h"

#include "Rte.h"

static void Swc_Set_RGB_Command_helper_off(RgbLedStateType *swc_rgb_state){
    swc_rgb_state->red = LED_STATE_OFF;
    swc_rgb_state->blue = LED_STATE_OFF;
    swc_rgb_state->green = LED_STATE_OFF;
}

static void Swc_Set_RGB_Command_helper_red(RgbLedStateType *swc_rgb_state){
    swc_rgb_state->red = LED_STATE_ON;
    swc_rgb_state->blue = LED_STATE_OFF;
    swc_rgb_state->green = LED_STATE_OFF;
}

static void Swc_Set_RGB_Command_helper_blue(RgbLedStateType *swc_rgb_state){
    swc_rgb_state->red = LED_STATE_OFF;
    swc_rgb_state->blue = LED_STATE_ON;
    swc_rgb_state->green = LED_STATE_OFF;
}

static void Swc_Set_RGB_Command_helper_green(RgbLedStateType *swc_rgb_state){
    swc_rgb_state->red = LED_STATE_OFF;
    swc_rgb_state->blue = LED_STATE_OFF;
    swc_rgb_state->green = LED_STATE_ON;
}

Std_ReturnType Swc_RgbLedControl_Runnable_100ms(void)
{
    RgbCommandType swc_rgb_command = RGB_COMMAND_OFF;
    RgbLedStateType swc_rgb_state = {
        .red = LED_STATE_OFF,
        .blue = LED_STATE_OFF,
        .green = LED_STATE_OFF
    };

    if (Rte_Read_Rp_RgbCommand(&swc_rgb_command) != E_OK) {
        // all off 및 오류 반환
        (void)Rte_Write_Pp_RgbLedState(swc_rgb_state);
        return E_NOT_OK;
    }

    switch (swc_rgb_command){
        case RGB_COMMAND_OFF: 
            Swc_Set_RGB_Command_helper_off(&swc_rgb_state); break;
        case RGB_COMMAND_RED: 
            Swc_Set_RGB_Command_helper_red(&swc_rgb_state); break;
        case RGB_COMMAND_BLUE: 
            Swc_Set_RGB_Command_helper_blue(&swc_rgb_state); break;
        case RGB_COMMAND_GREEN: 
            Swc_Set_RGB_Command_helper_green(&swc_rgb_state); break;
        default: // all off 및 오류 반환
            Swc_Set_RGB_Command_helper_off(&swc_rgb_state);
            (void)Rte_Write_Pp_RgbLedState(swc_rgb_state);
            return E_NOT_OK;
    }

    return Rte_Write_Pp_RgbLedState(swc_rgb_state);
}