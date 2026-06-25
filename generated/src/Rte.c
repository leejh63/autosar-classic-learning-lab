#include "Rte.h"

#include <stddef.h>

static LedCommandType rte_led_command = LED_COMMAND_OFF;
static LedStateType rte_led_state = LED_STATE_OFF;

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

void Rte_Reset(void)
{
    rte_led_command = LED_COMMAND_OFF;
    rte_led_state = LED_STATE_OFF;
}

void Rte_Inject_LedCommand(LedCommandType value)
{
    rte_led_command = value;
}

LedStateType Rte_Observe_LedState(void)
{
    return rte_led_state;
}
