#ifndef RTE_H
#define RTE_H

#include "Std_Types.h"
#include "App_Types.h"

/*
 * Learning RTE API.
 * In real AUTOSAR Classic, RTE is generated from component/interface configuration.
 * Here it is hand-written only to learn the data-flow boundary.
 */

Std_ReturnType Rte_Read_Rp_LedCommand(LedCommandType *value);
Std_ReturnType Rte_Write_Pp_LedState(LedStateType value);

/* BSW/IoHwAb-side access for this learning project. */
Std_ReturnType Rte_Bsw_Read_LedState(LedStateType *value);

/* Test/external hooks. These are not AUTOSAR RTE APIs. */
void Rte_Reset(void);
void Rte_Inject_LedCommand(LedCommandType value);
LedStateType Rte_Observe_LedState(void);

#endif /* RTE_H */
