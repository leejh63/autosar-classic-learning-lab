#include "Dio.h"

#define DIO_MAX_CHANNELS (16u)

static boolean dio_levels[DIO_MAX_CHANNELS];

Std_ReturnType Dio_WriteChannel(uint8_t channel, boolean level)
{
    /* TODO(STEP-MCAL-DIO-001):
     * channel 범위를 확인한 뒤 dio_levels[channel]에 level을 저장하세요.
     */
    (void)channel;
    (void)level;
    return E_NOT_OK;
}

Std_ReturnType Dio_ReadChannel(uint8_t channel, boolean *level)
{
    /* TODO(STEP-MCAL-DIO-002):
     * channel 범위와 level NULL을 확인한 뒤 값을 복사하세요.
     * 이 함수는 module test에서 가상 하드웨어 출력 상태를 확인하는 데 사용합니다.
     */
    (void)channel;
    (void)level;
    return E_NOT_OK;
}

void Dio_Reset(void)
{
    /* TODO(STEP-MCAL-DIO-003):
     * 모든 DIO level을 FALSE로 초기화하세요.
     */
    (void)dio_levels;
}
