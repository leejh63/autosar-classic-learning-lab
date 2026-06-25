#ifndef ECUC_CFG_H
#define ECUC_CFG_H

/*
 * TODO(STEP-CONFIG-LED):
 * config/hardware_config.json과 설계 문서를 보고 아래 값을 맞추세요.
 * 실제 AUTOSAR에서는 ECU Configuration 값이 툴을 통해 생성됩니다.
 * 여기서는 학습용으로 직접 수정하거나 tools/generate_from_config.py로 재생성합니다.
 */

#define ECUC_MAIN_FUNCTION_PERIOD_MS    (100u)

/* LED1 hardware mapping */
#define ECUC_DIO_CHANNEL_LED1           (0u)
#define ECUC_DIO_LED1_PORT_NAME         "PTD"
#define ECUC_DIO_LED1_PIN_NUMBER        (15u)
#define ECUC_DIO_LED1_ACTIVE_HIGH       (1u)

#endif /* ECUC_CFG_H */
