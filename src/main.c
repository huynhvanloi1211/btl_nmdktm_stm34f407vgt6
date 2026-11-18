
#include "stm32f4xx.h"

#include "motor.h"
#include "encoder.h"
#include "delay.h"
#include "data_logger.h"

/* =====================================
   MOTOR CONTROL WITH ENCODER UPDATE
   ===================================== */


void Motor_Run_Log(
    uint8_t duty,
    uint32_t duration_ms
)
{
    uint32_t elapsed = 0;

    /* Set motor duty */
    if (duty == 0)
    {
        Motor_Stop();
    }
    else
    {
        Motor_Forward(duty);
    }

    while (elapsed < duration_ms)
    {
        /* Update encoder */
        Encoder_Update();

        /* Save data into RAM */
        DataLogger_Record(
            elapsed,
            duty,
            Encoder_Delta,
            Encoder_Speed_RPM
        );

        Delay_ms(ENCODER_SAMPLE_TIME_MS);

        elapsed += ENCODER_SAMPLE_TIME_MS;

        /* Stop if buffer is full */
        if (Motor_Log_Finished)
        {
            Motor_Stop();
            return;
        }
    }
}


void Motor_Run(uint8_t direction,
               uint8_t duty,
               uint32_t duration_ms)
{
    uint32_t elapsed = 0;

    while (elapsed < duration_ms)
    {
        /* Motor direction */
        if (direction == 1)
        {
            Motor_Forward(duty);
        }
        else if (direction == 2)
        {
            Motor_Reverse(duty);
        }
        else
        {
            Motor_Stop();
        }

        /* Update encoder every 50ms */
        Encoder_Update();

        Delay_ms(ENCODER_SAMPLE_TIME_MS);

        elapsed += ENCODER_SAMPLE_TIME_MS;
    }
}


/* =====================================
   MAIN
   ===================================== */

int main(void)
{
    Delay_Init();

    Motor_Init();

    Encoder_Init();

    DataLogger_Init();

    /* Initial settling */
    Motor_Run_Log(0, 2000);

    /* 30% PWM */
    Motor_Run_Log(30, 6000);

    /* Stop */
    Motor_Run_Log(0, 4000);

    /* 50% PWM */
    Motor_Run_Log(50, 6000);

    /* Stop */
    Motor_Run_Log(0, 4000);

    /* 70% PWM */
    Motor_Run_Log(70, 6000);

    /* Stop */
    Motor_Run_Log(0, 5000);

    /* Finish experiment */
    Motor_Stop();

    Motor_Log_Finished = 1;

    /* Keep data in RAM */
    while (1)
    {
        /* Do not reset or overwrite buffer */
    }
}
