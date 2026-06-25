#ifndef BOARD_TYPES_H
#define BOARD_TYPES_H

#include <stdint.h>
#include "Std_Types.h"

typedef enum
{
    VHW_PORT_PTA = 0,
    VHW_PORT_PTB,
    VHW_PORT_PTC,
    VHW_PORT_PTD,
    VHW_PORT_PTE,
    VHW_PORT_COUNT
} Vhw_PortIdType;

typedef enum
{
    VHW_PIN_INPUT = 0,
    VHW_PIN_OUTPUT
} Vhw_PinDirectionType;

typedef enum
{
    VHW_ACTIVE_LOW  = 0,
    VHW_ACTIVE_HIGH = 1
} Vhw_ActivePolarityType;

typedef struct
{
    uint8_t channel_id;
    const char *symbolic_name;
    Vhw_PortIdType port;
    uint8_t pin;
    Vhw_PinDirectionType direction;
    Vhw_ActivePolarityType active_polarity;
    boolean initial_level;
} Board_DioChannelConfigType;

typedef struct
{
    uint8_t channel_id;
    const char *symbolic_name;
    uint8_t adc_unit;
    uint8_t adc_channel;
    uint16_t max_raw;
    uint16_t initial_raw;
} Board_AdcChannelConfigType;

typedef struct
{
    uint8_t channel_id;
    const char *symbolic_name;
    uint8_t pwm_unit;
    uint8_t pwm_channel;
    uint32_t period_us;
    uint16_t initial_duty_permille;
} Board_PwmChannelConfigType;

typedef struct
{
    uint8_t controller_id;
    const char *symbolic_name;
    uint32_t baudrate;
} Board_CanControllerConfigType;

typedef struct
{
    uint16_t pdu_id;
    const char *symbolic_name;
    uint32_t can_id;
    uint8_t dlc;
    uint8_t controller_id;
} Board_CanPduConfigType;

typedef struct
{
    uint8_t channel_id;
    const char *symbolic_name;
    uint32_t baudrate;
} Board_LinChannelConfigType;

typedef struct
{
    uint16_t frame_id;
    const char *symbolic_name;
    uint8_t protected_id;
    uint8_t dlc;
    uint8_t channel_id;
} Board_LinFrameConfigType;

#endif /* BOARD_TYPES_H */
