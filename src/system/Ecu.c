#include "Ecu.h"
#include "Port.h"
#include "Rte.h"
#include "VirtualHw.h"
#include "IoHwAb.h"
#include "Swc_LedControl.h"
#include "Swc_RgbLedControl.h"

Std_ReturnType Ecu_Init(void)
{
    Rte_Reset();
    return Port_Init();
}

void Ecu_MainFunction_100ms(void)
{
    IoHwAb_UpdateInputs();
    Swc_LedControl_Runnable_100ms();
    IoHwAb_UpdateOutputs();
}

void Ecu_MainFunction_Rgb_100ms(void)
{
    IoHwAb_UpdateInputs();
    (void)Swc_RgbLedControl_Runnable_100ms();
    IoHwAb_UpdateRgbOutputs();
}

void Ecu_ResetForTest(void)
{
    Rte_Reset();
    VirtualHw_Reset();
}
