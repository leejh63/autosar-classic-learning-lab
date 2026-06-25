#include "Port.h"
#include "EcuC_Cfg.h"

Std_ReturnType Port_Init(void)
{
    /* TODO(STEP-MCAL-PORT-001):
     * 학습용에서는 실제 핀 mux 설정 대신 설정값을 확인했다고 가정하고 E_OK를 반환해도 됩니다.
     * 확장 단계에서는 config 기반 pin direction table을 추가하세요.
     */
    (void)ECUC_DIO_CHANNEL_LED1;
    (void)ECUC_DIO_LED1_PIN_NUMBER;
    return E_NOT_OK;
}
