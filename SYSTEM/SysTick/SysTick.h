#ifndef __DELAY_H

#define __DELAY_H

#define USE_HSI 0 //64M
#include "stm32f10x.h"
#include "core_cm3.h"
#include "ESP_USART.h"
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
uint32_t get_tick(void);
void sys_tick_init(void);
#endif



