#include "motor.h"
#include "bsp.h"

void Motor_Init(void)
{
    BSP_Motor_GPIO_Init();
    BSP_Motor_PWM_Init();

    Motor_Stop();
}


/* Set PWM duty cycle: 0 - 100% */
void Motor_SetDuty(uint8_t duty_percent)
{
    uint32_t compare_value;

    if (duty_percent > 100)
    {
        duty_percent = 100;
    }

    compare_value =
        ((uint32_t)duty_percent * MOTOR_PWM_MAX) / 100U;

    TIM_SetCompare1(MOTOR_PWM_TIMER, compare_value);
}


/* Stop motor: IN1 = LOW, IN2 = LOW */
void Motor_Stop(void)
{
    Motor_SetDuty(0);
}


/* Rotate motor in one direction */
void Motor_Forward(uint8_t duty_percent)
{
    GPIO_ResetBits(MOTOR_IN2_GPIO, MOTOR_IN2_PIN);

    Motor_SetDuty(duty_percent);
}



