
#ifndef __DATA_LOGGER_H
#define __DATA_LOGGER_H

#include "stm32f4xx.h"

#define LOG_MAX_SAMPLES     1000U

typedef struct
{
    uint32_t time_ms;
    uint32_t duty_percent;
    int32_t encoder_delta;
    int32_t rpm_x100;

} Motor_Log_t;

/* Data buffer */
extern volatile Motor_Log_t Motor_Log_Buffer[LOG_MAX_SAMPLES];

/* Number of saved samples */
extern volatile uint32_t Motor_Log_Count;

/* Logging status */
extern volatile uint8_t Motor_Log_Finished;

/* Functions */
void DataLogger_Init(void);

void DataLogger_Record(
    uint32_t time_ms,
    uint32_t duty,
    int32_t encoder_delta,
    float rpm
);

#endif
