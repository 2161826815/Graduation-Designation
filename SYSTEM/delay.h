#ifndef __DELAY_H

#define __DELAY_H

#define USE_HSI 0 //64M
#include "stm32f10x.h"
#include "core_cm3.h"
#include "ESP_USART.h"
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
void delay_init(void);
#endif



