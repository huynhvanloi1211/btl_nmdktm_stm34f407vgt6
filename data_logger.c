
#include "data_logger.h"

volatile Motor_Log_t Motor_Log_Buffer[LOG_MAX_SAMPLES];

volatile uint32_t Motor_Log_Count = 0;

volatile uint8_t Motor_Log_Finished = 0;


void DataLogger_Init(void)
{
    Motor_Log_Count = 0;

    Motor_Log_Finished = 0;
}


void DataLogger_Record(
    uint32_t time_ms,
    uint32_t duty,
    int32_t encoder_delta,
    float rpm
)
{
    uint32_t index;

    if (Motor_Log_Count >= LOG_MAX_SAMPLES)
    {
        Motor_Log_Finished = 1;
        return;
    }

    index = Motor_Log_Count;

    Motor_Log_Buffer[index].time_ms = time_ms;

    Motor_Log_Buffer[index].duty_percent = duty;

    Motor_Log_Buffer[index].encoder_delta = encoder_delta;

    Motor_Log_Buffer[index].rpm_x100 =
        (int32_t)(rpm * 100.0f);

    /* Increase index after writing all fields */
    Motor_Log_Count++;
}
