#ifndef APP_TYPES_H
#define APP_TYPES_H

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

typedef enum
{
    BUTTON_STATE_RELEASED = 0,
    BUTTON_STATE_PRESSED  = 1
} ButtonStateType;

#endif /* APP_TYPES_H */
