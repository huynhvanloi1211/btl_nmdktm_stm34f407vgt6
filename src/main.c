#include "stm32f4xx.h"

#include "motor.h"
#include "encoder.h"
#include "delay.h"

int main(void)
{
    Delay_Init();

    Motor_Init();

    Encoder_Init();

    while (1)
    {
        Motor_Forward(40);

        Encoder_Update();

        Delay_ms(ENCODER_SAMPLE_TIME_MS);
			
    }
}
