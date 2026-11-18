
#ifndef __BSP_H
#define __BSP_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

/* PWM configuration */
#define MOTOR_PWM_TIMER             TIM3
#define MOTOR_PWM_TIMER_RCC         RCC_APB1Periph_TIM3

#define MOTOR_PWM_GPIO              GPIOA
#define MOTOR_PWM_GPIO_RCC          RCC_AHB1Periph_GPIOA
#define MOTOR_PWM_PIN               GPIO_Pin_6
#define MOTOR_PWM_PIN_SOURCE        GPIO_PinSource6
#define MOTOR_PWM_GPIO_AF           GPIO_AF_TIM3
#define MOTOR_PWM_CHANNEL           TIM_Channel_1

/* Motor direction pin */
#define MOTOR_IN2_GPIO              GPIOB
#define MOTOR_IN2_GPIO_RCC          RCC_AHB1Periph_GPIOB
#define MOTOR_IN2_PIN               GPIO_Pin_0

/* PWM parameters */
#define MOTOR_PWM_FREQUENCY         20000U
#define MOTOR_PWM_PRESCALER         0U
#define MOTOR_PWM_PERIOD            4199U

#define MOTOR_PWM_MAX               (MOTOR_PWM_PERIOD + 1U)

#endif
