#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include "Std_Types.h"

Std_ReturnType Pwm_SetDutyCycle(uint8_t channel, uint16_t duty_permille);
Std_ReturnType Pwm_GetDutyCycle(uint8_t channel, uint16_t *duty_permille);

#endif /* PWM_H */
