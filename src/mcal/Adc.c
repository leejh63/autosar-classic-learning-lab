#include "Adc.h"

#define ADC_MAX_CHANNELS (8u)

static uint16_t adc_raw_values[ADC_MAX_CHANNELS];

Std_ReturnType Adc_ReadChannel(uint8_t channel, uint16_t *raw_value)
{
    /* TODO(FUTURE-ADC-001):
     * 03_adc_input 단계에서 구현하세요.
     */
    (void)channel;
    (void)raw_value;
    return E_NOT_OK;
}

void Adc_InjectRawValue(uint8_t channel, uint16_t raw_value)
{
    /* TODO(FUTURE-ADC-002):
     * 03_adc_input 단계에서 구현하세요.
     */
    (void)channel;
    (void)raw_value;
}

void Adc_Reset(void)
{
    /* TODO(FUTURE-ADC-003):
     * 03_adc_input 단계에서 구현하세요.
     */
    (void)adc_raw_values;
}
