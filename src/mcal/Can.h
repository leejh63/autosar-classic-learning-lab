#ifndef CAN_H
#define CAN_H

#include <stdint.h>
#include "Std_Types.h"

Std_ReturnType Can_WritePdu(uint16_t pdu_id, const uint8_t *data, uint8_t length);
Std_ReturnType Can_ReadLastTxPdu(uint16_t pdu_id, uint8_t *data, uint8_t *length, boolean *valid);

#endif /* CAN_H */
