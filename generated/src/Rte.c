#include "Rte.h"

/*
 * TODO(STEP-RTE-LED):
 * RTE 내부 buffer를 정의하고 Read/Write/Inject/Observe 함수를 완성하세요.
 *
 * 학습 포인트:
 * - RTE는 SWC와 바깥 세계 사이의 통로 역할을 합니다.
 * - SWC는 내부 buffer가 어떻게 저장되는지 몰라도 됩니다.
 */

static LedCommandType rte_led_command = LED_COMMAND_OFF;
static LedStateType rte_led_state = LED_STATE_OFF;

Std_ReturnType Rte_Read_Rp_LedCommand(LedCommandType *value)
{
    /* TODO(STEP-RTE-READ-001):
     * value가 NULL인지 확인한 뒤 rte_led_command를 복사하세요.
     * 성공하면 E_OK, 실패하면 E_NOT_OK를 반환하세요.
     */
    (void)rte_led_command;
    (void)value;
    return E_NOT_OK;
}

Std_ReturnType Rte_Write_Pp_LedState(LedStateType value)
{
    /* TODO(STEP-RTE-WRITE-001):
     * SWC가 계산한 LED 상태를 rte_led_state에 저장하세요.
     */
    (void)value;
    return E_NOT_OK;
}

void Rte_Reset(void)
{
    /* TODO(STEP-RTE-RESET-001):
     * command와 state를 모두 기본값 OFF로 초기화하세요.
     */
}

void Rte_Inject_LedCommand(LedCommandType value)
{
    /* TODO(STEP-RTE-INJECT-001):
     * 테스트 코드가 LED 명령을 주입할 수 있게 rte_led_command에 저장하세요.
     */
    (void)value;
}

LedStateType Rte_Observe_LedState(void)
{
    /* TODO(STEP-RTE-OBSERVE-001):
     * 테스트와 IoHwAb가 SWC 출력값을 확인할 수 있게 반환하세요.
     */
    return rte_led_state;
}
