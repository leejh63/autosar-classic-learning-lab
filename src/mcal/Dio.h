#ifndef DIO_H
#define DIO_H

#include <stdint.h>
#include "Std_Types.h"

Std_ReturnType Dio_WriteChannel(uint8_t channel, boolean level);
Std_ReturnType Dio_ReadChannel(uint8_t channel, boolean *level);
void Dio_Reset(void);

#endif /* DIO_H */
