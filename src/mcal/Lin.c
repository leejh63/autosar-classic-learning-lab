#include "Lin.h"
#include "VirtualHw.h"

Std_ReturnType Lin_SendFrame(uint16_t frame_id, const uint8_t *data, uint8_t length)
{
    return VirtualHw_SendLinFrame(frame_id, data, length);
}

Std_ReturnType Lin_ReadLastTxFrame(uint16_t frame_id, uint8_t *data, uint8_t *length, boolean *valid)
{
    return VirtualHw_ReadLastLinTx(frame_id, data, length, valid);
}
