#include "Std_Types.h"
#include "App_Types.h"
#include "Rte.h"
#include "Swc_LedControl.h"

void Swc_LedControl_Runnable_100ms(void)
{
    LedCommandType command = LED_COMMAND_OFF;
    LedStateType state = LED_STATE_OFF;

    /* TODO(STEP-SWC-LED-001):
     * Rte_Read_Rp_LedCommand(&command)를 호출하여 LED 명령을 읽으세요.
     * Read가 실패하면 fail-closed 관점에서 LED_STATE_OFF를 선택하세요.
     */

    /* TODO(STEP-SWC-LED-002):
     * command가 LED_COMMAND_ON이면 state를 LED_STATE_ON으로 설정하세요.
     * command가 LED_COMMAND_OFF이거나 알 수 없는 값이면 LED_STATE_OFF로 설정하세요.
     */

    /* TODO(STEP-SWC-LED-003):
     * Rte_Write_Pp_LedState(state)를 호출하여 결과를 RTE에 쓰세요.
     */

    (void)command;
    (void)state;
}
