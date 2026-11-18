#include "delay.h"

static volatile uint32_t system_tick = 0;

void Delay_Init(void)
{
    SystemCoreClockUpdate();

    SysTick_Config(SystemCoreClock / 1000U);
}

void SysTick_Handler(void)
{
    system_tick++;
}

/* Tra ve so ms ke tu luc Delay_Init() */
uint32_t Delay_GetTick(void)
{
    return system_tick;
}

void Delay_ms(uint32_t milliseconds)
{
    uint32_t start_tick = system_tick;

    while ((system_tick - start_tick) < milliseconds)
    {
        /* Wait */
    }
}
