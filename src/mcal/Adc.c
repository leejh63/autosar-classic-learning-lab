#include "Adc.h"
#include "VirtualHw.h"

Std_ReturnType Adc_ReadChannel(uint8_t channel, uint16_t *raw_value)
{
    return VirtualHw_ReadAdcChannel(channel, raw_value);
}

Std_ReturnType Adc_InjectRawValue(uint8_t channel, uint16_t raw_value)
{
    return VirtualHw_InjectAdcRawValue(channel, raw_value);
}
