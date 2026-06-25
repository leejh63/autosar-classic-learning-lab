#ifndef VIRTUAL_HW_H
#define VIRTUAL_HW_H

#include <stdint.h>
#include "Std_Types.h"
#include "Board_Types.h"

Std_ReturnType VirtualHw_Init(void);
void VirtualHw_Reset(void);

Std_ReturnType VirtualHw_WriteDioChannel(uint8_t channel, boolean level);
Std_ReturnType VirtualHw_ReadDioChannel(uint8_t channel, boolean *level);
Std_ReturnType VirtualHw_InjectDioInput(uint8_t channel, boolean level);
Std_ReturnType VirtualHw_ReadPin(Vhw_PortIdType port, uint8_t pin, boolean *level);

Std_ReturnType VirtualHw_ReadAdcChannel(uint8_t channel, uint16_t *raw_value);
Std_ReturnType VirtualHw_InjectAdcRawValue(uint8_t channel, uint16_t raw_value);

Std_ReturnType VirtualHw_SetPwmDuty(uint8_t channel, uint16_t duty_permille);
Std_ReturnType VirtualHw_GetPwmDuty(uint8_t channel, uint16_t *duty_permille);

Std_ReturnType VirtualHw_SendCanPdu(uint16_t pdu_id, const uint8_t *data, uint8_t length);
Std_ReturnType VirtualHw_ReadLastCanTx(uint16_t pdu_id, uint8_t *data, uint8_t *length, boolean *valid);

Std_ReturnType VirtualHw_SendLinFrame(uint16_t frame_id, const uint8_t *data, uint8_t length);
Std_ReturnType VirtualHw_ReadLastLinTx(uint16_t frame_id, uint8_t *data, uint8_t *length, boolean *valid);

#endif /* VIRTUAL_HW_H */
