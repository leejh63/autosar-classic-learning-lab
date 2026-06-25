#ifndef BOARD_CFG_H
#define BOARD_CFG_H

#include <stdint.h>
#include "Board_Types.h"

#define BOARD_MAIN_FUNCTION_PERIOD_MS (100u)

/* DIO symbolic channel IDs */
#define BOARD_DIO_LED1       (0u)
#define BOARD_DIO_BUTTON1    (1u)
#define BOARD_DIO_DEBUG_PIN  (2u)
#define BOARD_DIO_CHANNEL_COUNT (3u)

/* ADC symbolic channel IDs */
#define BOARD_ADC_POT1       (0u)
#define BOARD_ADC_TEMP1      (1u)
#define BOARD_ADC_CHANNEL_COUNT (2u)

/* PWM symbolic channel IDs */
#define BOARD_PWM_LED_DIMMER (0u)
#define BOARD_PWM_BUZZER     (1u)
#define BOARD_PWM_CHANNEL_COUNT (2u)

/* CAN symbolic controller/PDU IDs */
#define BOARD_CAN_CTRL0      (0u)
#define BOARD_CAN_CONTROLLER_COUNT (1u)

#define BOARD_CAN_PDU_LED_STATUS    (0u)
#define BOARD_CAN_PDU_BUTTON_STATUS (1u)
#define BOARD_CAN_PDU_COUNT (2u)

/* LIN symbolic channel/frame IDs */
#define BOARD_LIN_CH0        (0u)
#define BOARD_LIN_CHANNEL_COUNT (1u)

#define BOARD_LIN_FRAME_SENSOR_REQ  (0u)
#define BOARD_LIN_FRAME_SENSOR_RESP (1u)
#define BOARD_LIN_FRAME_COUNT (2u)

extern const Board_DioChannelConfigType Board_DioChannels[BOARD_DIO_CHANNEL_COUNT];
extern const Board_AdcChannelConfigType Board_AdcChannels[BOARD_ADC_CHANNEL_COUNT];
extern const Board_PwmChannelConfigType Board_PwmChannels[BOARD_PWM_CHANNEL_COUNT];
extern const Board_CanControllerConfigType Board_CanControllers[BOARD_CAN_CONTROLLER_COUNT];
extern const Board_CanPduConfigType Board_CanPdus[BOARD_CAN_PDU_COUNT];
extern const Board_LinChannelConfigType Board_LinChannels[BOARD_LIN_CHANNEL_COUNT];
extern const Board_LinFrameConfigType Board_LinFrames[BOARD_LIN_FRAME_COUNT];

#endif /* BOARD_CFG_H */
