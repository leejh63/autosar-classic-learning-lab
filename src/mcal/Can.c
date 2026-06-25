#include "Can.h"
#include "VirtualHw.h"

Std_ReturnType Can_WritePdu(uint16_t pdu_id, const uint8_t *data, uint8_t length)
{
    return VirtualHw_SendCanPdu(pdu_id, data, length);
}

Std_ReturnType Can_ReadLastTxPdu(uint16_t pdu_id, uint8_t *data, uint8_t *length, boolean *valid)
{
    return VirtualHw_ReadLastCanTx(pdu_id, data, length, valid);
}
