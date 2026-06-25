#include <stdio.h>
#include <stdint.h>
#include "Std_Types.h"
#include "Board_Cfg.h"
#include "VirtualHw.h"
#include "Dio.h"
#include "Adc.h"
#include "Pwm.h"
#include "Can.h"
#include "Lin.h"

static int failures = 0;

#define EXPECT_EQ_INT(expected, actual, name)                                      \
    do {                                                                          \
        int exp__ = (int)(expected);                                               \
        int act__ = (int)(actual);                                                 \
        if (exp__ != act__) {                                                      \
            printf("[FAIL] %s expected=%d actual=%d\n", (name), exp__, act__);     \
            failures++;                                                            \
        } else {                                                                   \
            printf("[PASS] %s\n", (name));                                        \
        }                                                                          \
    } while (0)

static void test_dio_output_and_input(void)
{
    boolean level = FALSE;

    (void)VirtualHw_Init();
    EXPECT_EQ_INT(E_OK, Dio_WriteChannel(BOARD_DIO_LED1, TRUE), "DIO write LED output");
    EXPECT_EQ_INT(E_OK, Dio_ReadChannel(BOARD_DIO_LED1, &level), "DIO read LED output");
    EXPECT_EQ_INT(TRUE, level, "LED level after write");

    EXPECT_EQ_INT(E_NOT_OK, Dio_WriteChannel(BOARD_DIO_BUTTON1, TRUE), "DIO write input channel rejected");
    EXPECT_EQ_INT(E_OK, Dio_InjectInputLevel(BOARD_DIO_BUTTON1, FALSE), "DIO inject button input");
    EXPECT_EQ_INT(E_OK, Dio_ReadChannel(BOARD_DIO_BUTTON1, &level), "DIO read button input");
    EXPECT_EQ_INT(FALSE, level, "button level after injection");
}

static void test_adc_pwm_can_lin(void)
{
    uint16_t raw = 0u;
    uint16_t duty = 0u;
    uint8_t data[8] = {0u};
    uint8_t length = 0u;
    boolean valid = FALSE;
    uint8_t one_byte_payload[1] = {1u};
    uint8_t lin_payload[2] = {0xAAu, 0x55u};

    (void)VirtualHw_Init();

    EXPECT_EQ_INT(E_OK, Adc_InjectRawValue(BOARD_ADC_POT1, 3000u), "ADC inject POT1 raw");
    EXPECT_EQ_INT(E_OK, Adc_ReadChannel(BOARD_ADC_POT1, &raw), "ADC read POT1 raw");
    EXPECT_EQ_INT(3000u, raw, "ADC POT1 raw value");

    EXPECT_EQ_INT(E_NOT_OK, Adc_InjectRawValue(BOARD_ADC_POT1, 5000u), "ADC reject out-of-range raw");

    EXPECT_EQ_INT(E_OK, Pwm_SetDutyCycle(BOARD_PWM_LED_DIMMER, 250u), "PWM set duty");
    EXPECT_EQ_INT(E_OK, Pwm_GetDutyCycle(BOARD_PWM_LED_DIMMER, &duty), "PWM get duty");
    EXPECT_EQ_INT(250u, duty, "PWM duty value");
    EXPECT_EQ_INT(E_NOT_OK, Pwm_SetDutyCycle(BOARD_PWM_LED_DIMMER, 1500u), "PWM reject duty > 1000");

    EXPECT_EQ_INT(E_OK, Can_WritePdu(BOARD_CAN_PDU_LED_STATUS, one_byte_payload, 1u), "CAN write LED status PDU");
    EXPECT_EQ_INT(E_OK, Can_ReadLastTxPdu(BOARD_CAN_PDU_LED_STATUS, data, &length, &valid), "CAN read last TX");
    EXPECT_EQ_INT(TRUE, valid, "CAN TX valid");
    EXPECT_EQ_INT(1u, length, "CAN TX length");
    EXPECT_EQ_INT(1u, data[0], "CAN TX byte0");

    EXPECT_EQ_INT(E_OK, Lin_SendFrame(BOARD_LIN_FRAME_SENSOR_RESP, lin_payload, 2u), "LIN send frame");
    EXPECT_EQ_INT(E_OK, Lin_ReadLastTxFrame(BOARD_LIN_FRAME_SENSOR_RESP, data, &length, &valid), "LIN read last TX");
    EXPECT_EQ_INT(TRUE, valid, "LIN TX valid");
    EXPECT_EQ_INT(2u, length, "LIN TX length");
    EXPECT_EQ_INT(0xAAu, data[0], "LIN TX byte0");
    EXPECT_EQ_INT(0x55u, data[1], "LIN TX byte1");
}

int main(void)
{
    test_dio_output_and_input();
    test_adc_pwm_can_lin();

    if (failures == 0) {
        printf("virtual hardware tests passed\n");
        return 0;
    }

    printf("virtual hardware tests failed: %d\n", failures);
    return 1;
}
