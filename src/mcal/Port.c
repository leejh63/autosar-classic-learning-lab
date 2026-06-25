#include "Port.h"
#include "VirtualHw.h"

Std_ReturnType Port_Init(void)
{
    return VirtualHw_Init();
}
