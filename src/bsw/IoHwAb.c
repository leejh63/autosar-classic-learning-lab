#include "Std_Types.h"
#include "App_Types.h"
#include "EcuC_Cfg.h"
#include "Rte.h"
#include "Dio.h"
#include "IoHwAb.h"

Std_ReturnType IoHwAb_UpdateInputs(void)
{
    /*
     * LED-only 단계에서는 하드웨어 입력이 없습니다.
     * 다음 단계에서 Button 입력을 추가하면 여기에서 Dio_ReadChannel()을 호출하고
     * RTE input buffer로 값을 넣는 흐름을 만들게 됩니다.
     */
    return E_OK;
}

Std_ReturnType IoHwAb_UpdateOutputs(void)
{
    LedStateType led_state = LED_STATE_OFF;
    boolean hw_level = FALSE;

    /* TODO(STEP-IOHWAB-LED-001):
     * Rte_Observe_LedState()로 SWC 출력값을 읽으세요.
     */

    /* TODO(STEP-IOHWAB-LED-002):
     * LED_STATE_ON/OFF를 실제 DIO level로 변환하세요.
     * hint:
     * - active-high LED: ON -> TRUE, OFF -> FALSE
     * - active-low LED : ON -> FALSE, OFF -> TRUE
     * - ECUC_DIO_LED1_ACTIVE_HIGH 값을 사용하세요.
     */

    /* TODO(STEP-IOHWAB-LED-003):
     * Dio_WriteChannel(ECUC_DIO_CHANNEL_LED1, hw_level)를 호출하세요.
     */

    (void)led_state;
    (void)hw_level;
    return E_NOT_OK;
}
