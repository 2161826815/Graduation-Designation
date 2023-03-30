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
#if 0
void delay_ms(uint32_t ms)
{
    uint32_t i;
    SysTick_Config(72000);
    printf("delay\r\n");
    for(i=0;i<ms;i++){
        while(!( (SysTick->CTRL) & (1<16) ));
    }
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void delay_us(uint32_t us)
{
    uint32_t i;
    SysTick_Config(72);
    for(i=0;i<us;i++){
        while(!( (SysTick->CTRL) & (1<16) ));
    }
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
#endif

