#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f4xx.h"

void Motor_Init(void);

void Motor_SetDuty(uint8_t duty_percent);

void Motor_Stop(void);

void Motor_Forward(uint8_t duty_percent);


void Motor_Reverse(uint8_t duty_percent);

#endif
