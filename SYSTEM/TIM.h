#ifndef _TIM_H
#define _TIM_H

#include "stm32f10x.h"
 
void tim2_init(uint16_t Period,uint16_t PSC);
void tim3_init(uint16_t Period,uint16_t PSC);
#endif
