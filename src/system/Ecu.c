#include "Ecu.h"
#include "Port.h"
#include "Dio.h"
#include "Adc.h"
#include "Rte.h"
#include "IoHwAb.h"
#include "Swc_LedControl.h"

Std_ReturnType Ecu_Init(void)
{
    /* TODO(STEP-ECU-INIT-001):
     * Port_Init()을 호출하고 필요한 초기화를 수행하세요.
     */
    return Port_Init();
}

void Ecu_MainFunction_100ms(void)
{
    /* TODO(STEP-ECU-MAIN-001):
     * 100ms 주기에서 아래 순서를 구현하세요.
     * 1. IoHwAb_UpdateInputs()
     * 2. Swc_LedControl_Runnable_100ms()
     * 3. IoHwAb_UpdateOutputs()
     */
}

void Ecu_ResetForTest(void)
{
    /* TODO(STEP-ECU-RESET-001):
     * 테스트 시작 전에 가상 driver/RTE 상태를 초기화하세요.
     */
    Rte_Reset();
    Dio_Reset();
    Adc_Reset();
}
