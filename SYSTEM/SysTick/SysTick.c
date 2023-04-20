#include "Systick.h"
#include "init.h"
volatile uint32_t sys_tick;


void sys_tick_init(void)
{
    sys_tick = 0;
    SysTick->LOAD = (uint32_t)(SystemCoreClock/1000000-1UL);
    SysTick->VAL = 0UL;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

uint32_t inline get_tick(void)
{
    return sys_tick;
}

void delay_us(uint32_t time)
{
    uint32_t delay_tick = get_tick() + time;
    while(sys_tick != delay_tick);
}

void delay_ms(uint32_t time)
{
    uint32_t delay_tick = get_tick() + time*1000;
    while(sys_tick != delay_tick);
}

void SysTick_Handler(void)
{   
    ++sys_tick;
}


