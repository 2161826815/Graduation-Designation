#ifndef __LED_H

#define __LED_H
#include "stm32f10x.h"

#define LED_Clock_Port RCC_APB2Periph_GPIOA

#define LED1_Port GPIOC
#define LED1_Pin  GPIO_Pin_13

#define LED3_Port GPIOA
#define LED3_Pin  GPIO_Pin_4

#define LED4_Port GPIOA 
#define LED4_Pin  GPIO_Pin_5

#define LED5_Port GPIOA 
#define LED5_Pin  GPIO_Pin_6

#endif


void LED_Init(void);
void LED_ON(int num);
void LED_OFF(int num);
