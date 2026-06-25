#ifndef LIN_H
#define LIN_H

#include <stdint.h>
#include "Std_Types.h"

Std_ReturnType Lin_SendFrame(uint16_t frame_id, const uint8_t *data, uint8_t length);
Std_ReturnType Lin_ReadLastTxFrame(uint16_t frame_id, uint8_t *data, uint8_t *length, boolean *valid);

#endif /* LIN_H */
