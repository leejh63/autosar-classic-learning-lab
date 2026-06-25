#include "Board_Cfg.h"

/*
 * Learning board configuration.
 * This file replaces the previous macro-only EcuC_Cfg.h approach.
 * In a real AUTOSAR project, similar configuration data would usually be
 * produced by configuration tools from ECUC/ARXML descriptions.
 */

const Board_DioChannelConfigType Board_DioChannels[BOARD_DIO_CHANNEL_COUNT] =
{
    {
        .channel_id = BOARD_DIO_LED1,
        .symbolic_name = "Red",
        .port = VHW_PORT_PTD,
        .pin = 0u,
        .direction = VHW_PIN_OUTPUT,
        .active_polarity = VHW_ACTIVE_HIGH,
        .initial_level = FALSE
    },
    {
        .channel_id = BOARD_DIO_LED2,
        .symbolic_name = "Blue",
        .port = VHW_PORT_PTD,
        .pin = 15u,
        .direction = VHW_PIN_OUTPUT,
        .active_polarity = VHW_ACTIVE_HIGH,
        .initial_level = FALSE
    },
    {
        .channel_id = BOARD_DIO_LED3,
        .symbolic_name = "Green",
        .port = VHW_PORT_PTD,
        .pin = 16u,
        .direction = VHW_PIN_OUTPUT,
        .active_polarity = VHW_ACTIVE_HIGH,
        .initial_level = FALSE
    },
    {
        .channel_id = BOARD_DIO_BUTTON1,
        .symbolic_name = "BUTTON1",
        .port = VHW_PORT_PTC,
        .pin = 12u,
        .direction = VHW_PIN_INPUT,
        .active_polarity = VHW_ACTIVE_LOW,
        .initial_level = TRUE
    },
    {
        .channel_id = BOARD_DIO_DEBUG_PIN,
        .symbolic_name = "DEBUG_PIN",
        .port = VHW_PORT_PTB,
        .pin = 0u,
        .direction = VHW_PIN_OUTPUT,
        .active_polarity = VHW_ACTIVE_HIGH,
        .initial_level = FALSE
    }
};

const Board_AdcChannelConfigType Board_AdcChannels[BOARD_ADC_CHANNEL_COUNT] =
{
    {
        .channel_id = BOARD_ADC_POT1,
        .symbolic_name = "POT1",
        .adc_unit = 0u,
        .adc_channel = 0u,
        .max_raw = 4095u,
        .initial_raw = 0u
    },
    {
        .channel_id = BOARD_ADC_TEMP1,
        .symbolic_name = "TEMP1",
        .adc_unit = 0u,
        .adc_channel = 1u,
        .max_raw = 4095u,
        .initial_raw = 2048u
    }
};

const Board_PwmChannelConfigType Board_PwmChannels[BOARD_PWM_CHANNEL_COUNT] =
{
    {
        .channel_id = BOARD_PWM_LED_DIMMER,
        .symbolic_name = "LED_DIMMER",
        .pwm_unit = 0u,
        .pwm_channel = 0u,
        .period_us = 1000u,
        .initial_duty_permille = 0u
    },
    {
        .channel_id = BOARD_PWM_BUZZER,
        .symbolic_name = "BUZZER",
        .pwm_unit = 0u,
        .pwm_channel = 1u,
        .period_us = 500u,
        .initial_duty_permille = 0u
    }
};

const Board_CanControllerConfigType Board_CanControllers[BOARD_CAN_CONTROLLER_COUNT] =
{
    {
        .controller_id = BOARD_CAN_CTRL0,
        .symbolic_name = "CAN0",
        .baudrate = 500000u
    }
};

const Board_CanPduConfigType Board_CanPdus[BOARD_CAN_PDU_COUNT] =
{
    {
        .pdu_id = BOARD_CAN_PDU_LED_STATUS,
        .symbolic_name = "CAN_PDU_LED_STATUS",
        .can_id = 0x321u,
        .dlc = 1u,
        .controller_id = BOARD_CAN_CTRL0
    },
    {
        .pdu_id = BOARD_CAN_PDU_BUTTON_STATUS,
        .symbolic_name = "CAN_PDU_BUTTON_STATUS",
        .can_id = 0x322u,
        .dlc = 1u,
        .controller_id = BOARD_CAN_CTRL0
    }
};

const Board_LinChannelConfigType Board_LinChannels[BOARD_LIN_CHANNEL_COUNT] =
{
    {
        .channel_id = BOARD_LIN_CH0,
        .symbolic_name = "LIN0",
        .baudrate = 19200u
    }
};

const Board_LinFrameConfigType Board_LinFrames[BOARD_LIN_FRAME_COUNT] =
{
    {
        .frame_id = BOARD_LIN_FRAME_SENSOR_REQ,
        .symbolic_name = "LIN_FRAME_SENSOR_REQ",
        .protected_id = 0x12u,
        .dlc = 2u,
        .channel_id = BOARD_LIN_CH0
    },
    {
        .frame_id = BOARD_LIN_FRAME_SENSOR_RESP,
        .symbolic_name = "LIN_FRAME_SENSOR_RESP",
        .protected_id = 0x13u,
        .dlc = 2u,
        .channel_id = BOARD_LIN_CH0
    }
};
