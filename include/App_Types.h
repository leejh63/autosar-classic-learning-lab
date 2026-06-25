#ifndef APP_TYPES_H
#define APP_TYPES_H

#include "Std_Types.h"

typedef enum
{
    LED_COMMAND_OFF = 0,
    LED_COMMAND_ON  = 1
} LedCommandType;

typedef enum
{
    LED_STATE_OFF = 0,
    LED_STATE_ON  = 1
} LedStateType;

#endif /* APP_TYPES_H */
