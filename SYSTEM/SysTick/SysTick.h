#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"
#include "core_cm3.h"
#include "ESP_USART.h"

void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
void sys_tick_init(void);
uint32_t get_tick(void);

#endif



