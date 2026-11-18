
#include "stm32f4xx.h"

#include "motor.h"
#include "encoder.h"
#include "delay.h"

/* =====================================
   MOTOR CONTROL WITH ENCODER UPDATE
   ===================================== */

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

    while (1)
    {
        /* Forward 40% - 3 seconds */
        Motor_Run(1, 70, 30000);

        /* Stop - 1 second */
        Motor_Run(0, 0, 1000);

        /* Reverse 40% - 3 seconds */
        Motor_Run(2, 40, 30000);


        /* Stop - 1 second */
        Motor_Run(0, 0, 1000);
    }
}
