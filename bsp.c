#include "bsp.h"



void BSP_Motor_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIO clocks */
    RCC_AHB1PeriphClockCmd(
        MOTOR_PWM_GPIO_RCC | MOTOR_IN2_GPIO_RCC,
        ENABLE
    );

    /* PA6: TIM3_CH1 alternate function */
    GPIO_PinAFConfig(
        MOTOR_PWM_GPIO,
        MOTOR_PWM_PIN_SOURCE,
        MOTOR_PWM_GPIO_AF
    );

    GPIO_InitStructure.GPIO_Pin = MOTOR_PWM_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

    GPIO_Init(MOTOR_PWM_GPIO, &GPIO_InitStructure);

    /* PB0: IN2 */
    /* PB0 = TIM3_CH3 (IN2 PWM) */

		/* Configure Alternate Function */
		GPIO_PinAFConfig(
				GPIOB,
				GPIO_PinSource0,
				GPIO_AF_TIM3
		);

		/* GPIO configuration */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

		GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void BSP_Motor_PWM_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    /* Enable TIM3 clock */
    RCC_APB1PeriphClockCmd(MOTOR_PWM_TIMER_RCC, ENABLE);

    /* Time base configuration */
    TIM_BaseInitStructure.TIM_Prescaler = MOTOR_PWM_PRESCALER;
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseInitStructure.TIM_Period = MOTOR_PWM_PERIOD;
    TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseInitStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(MOTOR_PWM_TIMER, &TIM_BaseInitStructure);

    /* PWM mode configuration */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(MOTOR_PWM_TIMER, &TIM_OCInitStructure);
		
		/* TIM3 Channel 3 */
		TIM_OC3Init(MOTOR_PWM_TIMER, &TIM_OCInitStructure);

		TIM_OC3PreloadConfig(
				MOTOR_PWM_TIMER,
				TIM_OCPreload_Enable
		);

    /* Enable preload */
    TIM_OC1PreloadConfig(
        MOTOR_PWM_TIMER,
        TIM_OCPreload_Enable
    );

    TIM_ARRPreloadConfig(MOTOR_PWM_TIMER, ENABLE);

    /* Start TIM3 */
    TIM_Cmd(MOTOR_PWM_TIMER, ENABLE);
}
