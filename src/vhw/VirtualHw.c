#include "VirtualHw.h"

#include <string.h>
#include "Board_Cfg.h"

#define VHW_MAX_FRAME_DATA_LENGTH (8u)

static boolean vhw_initialized = FALSE;
static boolean dio_levels[BOARD_DIO_CHANNEL_COUNT];
static uint16_t adc_raw_values[BOARD_ADC_CHANNEL_COUNT];
static uint16_t pwm_duty_permille[BOARD_PWM_CHANNEL_COUNT];

static uint8_t can_last_tx_data[BOARD_CAN_PDU_COUNT][VHW_MAX_FRAME_DATA_LENGTH];
static uint8_t can_last_tx_length[BOARD_CAN_PDU_COUNT];
static boolean can_last_tx_valid[BOARD_CAN_PDU_COUNT];

static uint8_t lin_last_tx_data[BOARD_LIN_FRAME_COUNT][VHW_MAX_FRAME_DATA_LENGTH];
static uint8_t lin_last_tx_length[BOARD_LIN_FRAME_COUNT];
static boolean lin_last_tx_valid[BOARD_LIN_FRAME_COUNT];

static Std_ReturnType get_dio_cfg(uint8_t channel, const Board_DioChannelConfigType **cfg)
{
    if ((cfg == NULL) || (channel >= BOARD_DIO_CHANNEL_COUNT)) {
        return E_NOT_OK;
    }

    if (Board_DioChannels[channel].channel_id != channel) {
        return E_NOT_OK;
    }

    *cfg = &Board_DioChannels[channel];
    return E_OK;
}

static Std_ReturnType get_adc_cfg(uint8_t channel, const Board_AdcChannelConfigType **cfg)
{
    if ((cfg == NULL) || (channel >= BOARD_ADC_CHANNEL_COUNT)) {
        return E_NOT_OK;
    }

    if (Board_AdcChannels[channel].channel_id != channel) {
        return E_NOT_OK;
    }

    *cfg = &Board_AdcChannels[channel];
    return E_OK;
}

static Std_ReturnType get_pwm_cfg(uint8_t channel, const Board_PwmChannelConfigType **cfg)
{
    if ((cfg == NULL) || (channel >= BOARD_PWM_CHANNEL_COUNT)) {
        return E_NOT_OK;
    }

    if (Board_PwmChannels[channel].channel_id != channel) {
        return E_NOT_OK;
    }

    *cfg = &Board_PwmChannels[channel];
    return E_OK;
}

static Std_ReturnType get_can_pdu_cfg(uint16_t pdu_id, const Board_CanPduConfigType **cfg)
{
    if ((cfg == NULL) || (pdu_id >= BOARD_CAN_PDU_COUNT)) {
        return E_NOT_OK;
    }

    if (Board_CanPdus[pdu_id].pdu_id != pdu_id) {
        return E_NOT_OK;
    }

    *cfg = &Board_CanPdus[pdu_id];
    return E_OK;
}

static Std_ReturnType get_lin_frame_cfg(uint16_t frame_id, const Board_LinFrameConfigType **cfg)
{
    if ((cfg == NULL) || (frame_id >= BOARD_LIN_FRAME_COUNT)) {
        return E_NOT_OK;
    }

    if (Board_LinFrames[frame_id].frame_id != frame_id) {
        return E_NOT_OK;
    }

    *cfg = &Board_LinFrames[frame_id];
    return E_OK;
}

void VirtualHw_Reset(void)
{
    uint8_t i;

    for (i = 0u; i < BOARD_DIO_CHANNEL_COUNT; ++i) {
        dio_levels[i] = Board_DioChannels[i].initial_level;
    }

    for (i = 0u; i < BOARD_ADC_CHANNEL_COUNT; ++i) {
        adc_raw_values[i] = Board_AdcChannels[i].initial_raw;
    }

    for (i = 0u; i < BOARD_PWM_CHANNEL_COUNT; ++i) {
        pwm_duty_permille[i] = Board_PwmChannels[i].initial_duty_permille;
    }

    for (i = 0u; i < BOARD_CAN_PDU_COUNT; ++i) {
        (void)memset(can_last_tx_data[i], 0, VHW_MAX_FRAME_DATA_LENGTH);
        can_last_tx_length[i] = 0u;
        can_last_tx_valid[i] = FALSE;
    }

    for (i = 0u; i < BOARD_LIN_FRAME_COUNT; ++i) {
        (void)memset(lin_last_tx_data[i], 0, VHW_MAX_FRAME_DATA_LENGTH);
        lin_last_tx_length[i] = 0u;
        lin_last_tx_valid[i] = FALSE;
    }

    vhw_initialized = TRUE;
}

Std_ReturnType VirtualHw_Init(void)
{
    VirtualHw_Reset();
    return E_OK;
}

Std_ReturnType VirtualHw_WriteDioChannel(uint8_t channel, boolean level)
{
    const Board_DioChannelConfigType *cfg = NULL;

    if ((vhw_initialized == FALSE) || (get_dio_cfg(channel, &cfg) != E_OK)) {
        return E_NOT_OK;
    }

    if (cfg->direction != VHW_PIN_OUTPUT) {
        return E_NOT_OK;
    }

    dio_levels[channel] = (level != FALSE) ? TRUE : FALSE;
    return E_OK;
}

Std_ReturnType VirtualHw_ReadDioChannel(uint8_t channel, boolean *level)
{
    const Board_DioChannelConfigType *cfg = NULL;

    if ((level == NULL) || (vhw_initialized == FALSE) || (get_dio_cfg(channel, &cfg) != E_OK)) {
        return E_NOT_OK;
    }

    (void)cfg;
    *level = dio_levels[channel];
    return E_OK;
}

Std_ReturnType VirtualHw_InjectDioInput(uint8_t channel, boolean level)
{
    const Board_DioChannelConfigType *cfg = NULL;

    if ((vhw_initialized == FALSE) || (get_dio_cfg(channel, &cfg) != E_OK)) {
        return E_NOT_OK;
    }

    if (cfg->direction != VHW_PIN_INPUT) {
        return E_NOT_OK;
    }

    dio_levels[channel] = (level != FALSE) ? TRUE : FALSE;
    return E_OK;
}

Std_ReturnType VirtualHw_ReadPin(Vhw_PortIdType port, uint8_t pin, boolean *level)
{
    uint8_t i;

    if ((level == NULL) || (vhw_initialized == FALSE)) {
        return E_NOT_OK;
    }

    for (i = 0u; i < BOARD_DIO_CHANNEL_COUNT; ++i) {
        if ((Board_DioChannels[i].port == port) && (Board_DioChannels[i].pin == pin)) {
            *level = dio_levels[i];
            return E_OK;
        }
    }

    return E_NOT_OK;
}

Std_ReturnType VirtualHw_ReadAdcChannel(uint8_t channel, uint16_t *raw_value)
{
    const Board_AdcChannelConfigType *cfg = NULL;

    if ((raw_value == NULL) || (vhw_initialized == FALSE) || (get_adc_cfg(channel, &cfg) != E_OK)) {
        return E_NOT_OK;
    }

    (void)cfg;
    *raw_value = adc_raw_values[channel];
    return E_OK;
}

Std_ReturnType VirtualHw_InjectAdcRawValue(uint8_t channel, uint16_t raw_value)
{
    const Board_AdcChannelConfigType *cfg = NULL;

    if ((vhw_initialized == FALSE) || (get_adc_cfg(channel, &cfg) != E_OK)) {
        return E_NOT_OK;
    }

    if (raw_value > cfg->max_raw) {
        return E_NOT_OK;
    }

    adc_raw_values[channel] = raw_value;
    return E_OK;
}

Std_ReturnType VirtualHw_SetPwmDuty(uint8_t channel, uint16_t duty_permille)
{
    const Board_PwmChannelConfigType *cfg = NULL;

    if ((vhw_initialized == FALSE) || (get_pwm_cfg(channel, &cfg) != E_OK)) {
        return E_NOT_OK;
    }

    if (duty_permille > 1000u) {
        return E_NOT_OK;
    }

    (void)cfg;
    pwm_duty_permille[channel] = duty_permille;
    return E_OK;
}

Std_ReturnType VirtualHw_GetPwmDuty(uint8_t channel, uint16_t *duty_permille)
{
    const Board_PwmChannelConfigType *cfg = NULL;

    if ((duty_permille == NULL) || (vhw_initialized == FALSE) || (get_pwm_cfg(channel, &cfg) != E_OK)) {
        return E_NOT_OK;
    }

    (void)cfg;
    *duty_permille = pwm_duty_permille[channel];
    return E_OK;
}

Std_ReturnType VirtualHw_SendCanPdu(uint16_t pdu_id, const uint8_t *data, uint8_t length)
{
    const Board_CanPduConfigType *cfg = NULL;

    if ((vhw_initialized == FALSE) || (get_can_pdu_cfg(pdu_id, &cfg) != E_OK)) {
        return E_NOT_OK;
    }

    if ((length > cfg->dlc) || (length > VHW_MAX_FRAME_DATA_LENGTH)) {
        return E_NOT_OK;
    }

    if ((length > 0u) && (data == NULL)) {
        return E_NOT_OK;
    }

    (void)memset(can_last_tx_data[pdu_id], 0, VHW_MAX_FRAME_DATA_LENGTH);
    if (length > 0u) {
        (void)memcpy(can_last_tx_data[pdu_id], data, length);
    }
    can_last_tx_length[pdu_id] = length;
    can_last_tx_valid[pdu_id] = TRUE;

    return E_OK;
}

Std_ReturnType VirtualHw_ReadLastCanTx(uint16_t pdu_id, uint8_t *data, uint8_t *length, boolean *valid)
{
    const Board_CanPduConfigType *cfg = NULL;

    if ((data == NULL) || (length == NULL) || (valid == NULL) ||
        (vhw_initialized == FALSE) || (get_can_pdu_cfg(pdu_id, &cfg) != E_OK)) {
        return E_NOT_OK;
    }

    (void)cfg;
    (void)memcpy(data, can_last_tx_data[pdu_id], VHW_MAX_FRAME_DATA_LENGTH);
    *length = can_last_tx_length[pdu_id];
    *valid = can_last_tx_valid[pdu_id];

    return E_OK;
}

Std_ReturnType VirtualHw_SendLinFrame(uint16_t frame_id, const uint8_t *data, uint8_t length)
{
    const Board_LinFrameConfigType *cfg = NULL;

    if ((vhw_initialized == FALSE) || (get_lin_frame_cfg(frame_id, &cfg) != E_OK)) {
        return E_NOT_OK;
    }

    if ((length > cfg->dlc) || (length > VHW_MAX_FRAME_DATA_LENGTH)) {
        return E_NOT_OK;
    }

    if ((length > 0u) && (data == NULL)) {
        return E_NOT_OK;
    }

    (void)memset(lin_last_tx_data[frame_id], 0, VHW_MAX_FRAME_DATA_LENGTH);
    if (length > 0u) {
        (void)memcpy(lin_last_tx_data[frame_id], data, length);
    }
    lin_last_tx_length[frame_id] = length;
    lin_last_tx_valid[frame_id] = TRUE;

    return E_OK;
}

Std_ReturnType VirtualHw_ReadLastLinTx(uint16_t frame_id, uint8_t *data, uint8_t *length, boolean *valid)
{
    const Board_LinFrameConfigType *cfg = NULL;

    if ((data == NULL) || (length == NULL) || (valid == NULL) ||
        (vhw_initialized == FALSE) || (get_lin_frame_cfg(frame_id, &cfg) != E_OK)) {
        return E_NOT_OK;
    }

    (void)cfg;
    (void)memcpy(data, lin_last_tx_data[frame_id], VHW_MAX_FRAME_DATA_LENGTH);
    *length = lin_last_tx_length[frame_id];
    *valid = lin_last_tx_valid[frame_id];

    return E_OK;
}
