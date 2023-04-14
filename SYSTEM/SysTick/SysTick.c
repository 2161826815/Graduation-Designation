#include "Systick.h"
#include "init.h"
volatile uint32_t m_tick;
extern uint8_t m_ms;

void sys_tick_init(void)
{
    m_tick = 0;
    SysTick->LOAD = (uint32_t)(SystemCoreClock/1000000-1UL);
    SysTick->VAL = 0UL;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

uint32_t inline get_tick(void)
{
    return m_tick;
}

void delay_us(uint32_t time)
{
    uint32_t delay_tick = get_tick() + time;
    while(m_tick != delay_tick);
}

void delay_ms(uint32_t time)
{
    uint32_t delay_tick = get_tick() + time*1000;
    while(m_tick != delay_tick);
}

void SysTick_Handler(void)
{   
    ++m_tick;
}


