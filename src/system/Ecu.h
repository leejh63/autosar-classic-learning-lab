#ifndef ECU_H
#define ECU_H

#include "Std_Types.h"

Std_ReturnType Ecu_Init(void);
void Ecu_MainFunction_100ms(void);
void Ecu_ResetForTest(void);

#endif /* ECU_H */
