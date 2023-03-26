#include "delay.h"

#if USE_HSI
static uint8_t nus = 0;
static uint16_t nms = 0;
void delay_init(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //8分频
    nus = SystemCoreClock/8000000;
    nms = (uint16_t)nus*1000;
}
#endif

void delay_ms(uint32_t ms)
{
    uint32_t i;
#if USE_HSI
    SysTick->LOAD = (uint32_t)ms*nms;
    SysTick->VAL = 0x00;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
    do{
        i = SysTick->CTRL;
    }while((i&0x01)&&!(i&(1<<16)));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
#else
    SysTick_Config(72000);

    for(i=0;i<ms;i++){
        while(!( (SysTick->CTRL) & (1<16) ));
    }

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
#endif
}

void delay_us(uint32_t us)
{
    uint32_t i;
#if USE_HSI
    SysTick->LOAD = us*nus;
    SysTick->VAL = 0x00;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
    do{
        i = SysTick->CTRL;
    }while((i&0x01)&&!(i&(1<<16)));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
#else
    SysTick_Config(72);

    for(i=0;i<us;i++){
        while(!( (SysTick->CTRL) & (1<16) ));
    }

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
#endif
}


