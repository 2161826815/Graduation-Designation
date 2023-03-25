#include "delay.h"


void delay_ms(uint32_t ms)
{
#ifdef EX_Crystal
    uint32_t i;
    SysTick_Config(72000);

    for(i=0;i<ms;i++){
        while(!( (SysTick->CTRL) & (1<16) ));
    }

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
#else
    while(ms--){
        uint8_t i = 50000;
        while(i--);
    }
#endif

}

void delay_us(uint32_t us)
{
#ifdef EX_Crystal
    uint32_t i;
    SysTick_Config(72);

    for(i=0;i<us;i++){
        while(!( (SysTick->CTRL) & (1<16) ));
    }

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

#else
    while(us--){
        uint8_t i = 50;
        while(i--);
    }
#endif
}


