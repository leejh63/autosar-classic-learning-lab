#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include "Std_Types.h"

Std_ReturnType Adc_ReadChannel(uint8_t channel, uint16_t *raw_value);
Std_ReturnType Adc_InjectRawValue(uint8_t channel, uint16_t raw_value);

#endif /* ADC_H */
