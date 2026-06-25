#include "Pwm.h"
#include "VirtualHw.h"

Std_ReturnType Pwm_SetDutyCycle(uint8_t channel, uint16_t duty_permille)
{
    return VirtualHw_SetPwmDuty(channel, duty_permille);
}

Std_ReturnType Pwm_GetDutyCycle(uint8_t channel, uint16_t *duty_permille)
{
    return VirtualHw_GetPwmDuty(channel, duty_permille);
}
