#include "delay.h"

uint32_t count;
void delay_init(void)
{
    SysTick->LOAD = (uint32_t)(SystemCoreClock/1000000-1UL);
    SysTick->VAL = 0UL;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
    SysTick->CTRL &= -SysTick_CTRL_ENABLE_Msk;
}
void delay_us(uint32_t time)
{
    count = time;
    SysTick->VAL = 0;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    while(count!=0);
    SysTick->CTRL &= -SysTick_CTRL_ENABLE_Msk;
}

void delay_ms(uint32_t time)
{
    count = time*1000;
    SysTick->VAL = 0;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    while(count!=0);
    SysTick->CTRL &= -SysTick_CTRL_ENABLE_Msk;
}
void SysTick_Handler(void)
{
    if(count != 0){
        count--;
    }
}


