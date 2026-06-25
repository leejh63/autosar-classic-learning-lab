#include "Rte.h"

#include <stddef.h>

static LedCommandType rte_led_command = LED_COMMAND_OFF;
static LedStateType rte_led_state = LED_STATE_OFF;

// RGB_LED
static RgbCommandType rte_rgb_command = RGB_COMMAND_OFF;
static RgbLedStateType rte_rgb_state = {
    .red = LED_STATE_OFF,
    .blue = LED_STATE_OFF,
    .green = LED_STATE_OFF
};

void Rte_Reset_Led(void)
{
    rte_led_command = LED_COMMAND_OFF;
    rte_led_state = LED_STATE_OFF;
}

// RGB_LED
void Rte_Reset_Rgb(void)
{
    rte_rgb_command = RGB_COMMAND_OFF;
    rte_rgb_state.red = LED_STATE_OFF;
    rte_rgb_state.blue = LED_STATE_OFF;
    rte_rgb_state.green = LED_STATE_OFF;
}

void Rte_Reset(void)
{
    Rte_Reset_Led();
    Rte_Reset_Rgb();
}

// impelement
Std_ReturnType Rte_Read_Rp_LedCommand(LedCommandType *value)
{
    if (value == NULL) {
        return E_NOT_OK;
    }

    *value = rte_led_command;
    return E_OK;
}

Std_ReturnType Rte_Write_Pp_LedState(LedStateType value)
{
    rte_led_state = value;
    return E_OK;
}

Std_ReturnType Rte_Bsw_Read_LedState(LedStateType *value)
{
    if (value == NULL) {
        return E_NOT_OK;
    }

    *value = rte_led_state;
    return E_OK;
}

void Rte_Inject_LedCommand(LedCommandType value)
{
    rte_led_command = value;
}

LedStateType Rte_Observe_LedState(void)
{
    return rte_led_state;
}

// RGB_LED
Std_ReturnType Rte_Read_Rp_RgbCommand(RgbCommandType *value)
{
    if (value == NULL) {
        return E_NOT_OK;
    }

    *value = rte_rgb_command;
    return E_OK;
}

Std_ReturnType Rte_Write_Pp_RgbLedState(RgbLedStateType value)
{
    rte_rgb_state = value;
    return E_OK;
}

Std_ReturnType Rte_Bsw_Read_RgbLedState(RgbLedStateType *value)
{
    if (value == NULL) {
        return E_NOT_OK;
    }

    *value = rte_rgb_state;
    return E_OK;
}

void Rte_Inject_RgbCommand(RgbCommandType value)
{
    rte_rgb_command = value;
}

RgbLedStateType Rte_Observe_RgbLedState(void)
{
    return rte_rgb_state;
}


