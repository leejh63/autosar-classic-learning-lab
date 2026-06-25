#include "Dio.h"
#include "VirtualHw.h"

Std_ReturnType Dio_WriteChannel(uint8_t channel, boolean level)
{
    return VirtualHw_WriteDioChannel(channel, level);
}

Std_ReturnType Dio_ReadChannel(uint8_t channel, boolean *level)
{
    return VirtualHw_ReadDioChannel(channel, level);
}

Std_ReturnType Dio_InjectInputLevel(uint8_t channel, boolean level)
{
    return VirtualHw_InjectDioInput(channel, level);
}

void Dio_Reset(void)
{
    VirtualHw_Reset();
}
