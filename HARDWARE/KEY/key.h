#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

#define key2_port GPIOC
#define key2_pin  GPIO_Pin_14

#define key5_port GPIOC
#define key5_pin  GPIO_Pin_15

#define key3_port GPIOB
#define key3_pin  GPIO_Pin_8

#define key4_port GPIOB
#define key4_pin  GPIO_Pin_9

void Key_NVIC_Config(void);
void Key_Init(void);

#endif
