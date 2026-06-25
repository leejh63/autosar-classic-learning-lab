#include "Swc_LedControl.h"
#include "Rte.h"

void Swc_LedControl_Runnable_100ms(void)
{
    LedCommandType command = LED_COMMAND_OFF;
    LedStateType state = LED_STATE_OFF;

    if (Rte_Read_Rp_LedCommand(&command) != E_OK) {
        (void)Rte_Write_Pp_LedState(LED_STATE_OFF);
        return;
    }

    if (command == LED_COMMAND_ON) {
        state = LED_STATE_ON;
    } else {
        state = LED_STATE_OFF;
    }

    (void)Rte_Write_Pp_LedState(state);
}
