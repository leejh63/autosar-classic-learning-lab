#ifndef RTE_H
#define RTE_H

#include "Std_Types.h"
#include "App_Types.h"

/*
 * LED-first 학습용 RTE API입니다.
 * SWC는 Dio_WriteChannel(), Port_Init() 같은 하드웨어/BSW 함수를 직접 호출하지 않습니다.
 */

Std_ReturnType Rte_Read_Rp_LedCommand(LedCommandType *value);
Std_ReturnType Rte_Write_Pp_LedState(LedStateType value);

/* 테스트/가상 외부 입력에서 사용할 hook입니다. 실제 AUTOSAR RTE API와는 다릅니다. */
void Rte_Reset(void);
void Rte_Inject_LedCommand(LedCommandType value);
LedStateType Rte_Observe_LedState(void);

#endif /* RTE_H */
