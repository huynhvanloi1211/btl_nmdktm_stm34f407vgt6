#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx.h"

void Delay_Init(void);
void Delay_ms(uint32_t milliseconds);
uint32_t Delay_GetTick(void);

#endif
