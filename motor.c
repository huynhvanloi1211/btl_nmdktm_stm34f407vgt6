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
void Motor_Forward(uint8_t duty_percent)
{
    if (duty_percent > 100)
        duty_percent = 100;

    TIM_SetCompare3(TIM3, 0);

    Motor_SetDuty(duty_percent);
}


void Motor_Reverse(uint8_t duty_percent)
{
    if (duty_percent > 100)
        duty_percent = 100;

    TIM_SetCompare1(TIM3, 0);

    TIM_SetCompare3(
        TIM3,
        ((uint32_t)duty_percent * MOTOR_PWM_MAX) / 100U
    );
}


void Motor_Stop(void)
{
    TIM_SetCompare1(TIM3, 0);
    TIM_SetCompare3(TIM3, 0);
}
