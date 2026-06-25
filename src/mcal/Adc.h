#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include "Std_Types.h"

/* ADC는 03_adc_input 단계에서 활성화할 예정입니다. */
Std_ReturnType Adc_ReadChannel(uint8_t channel, uint16_t *raw_value);
void Adc_InjectRawValue(uint8_t channel, uint16_t raw_value);
void Adc_Reset(void);

#endif /* ADC_H */
