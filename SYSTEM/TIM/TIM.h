#ifndef _TIM_H
#define _TIM_H

#include "stm32f10x.h"
#include "main.h"
#define USE_PWM 0

void tim2_init(uint16_t Period,uint16_t PSC);
void tim3_init(uint16_t Period,uint16_t PSC);
void tim4_init(uint16_t Period,uint16_t PSC);
uint32_t tim_get_tick(void);
#endif
