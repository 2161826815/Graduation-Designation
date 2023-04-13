#ifndef __BEEP_H

#define __BEEP_H
#include "stm32f10x.h"

#define BEEP_Clock_Port RCC_APB2Periph_GPIOB

#define BEEP_Port GPIOB
#define BEEP_Pin  GPIO_Pin_12



#endif


void BEEP_Init(void);
void BEEP_ON(void);
void BEEP_OFF(void);
void BEEP_Toggle(void);
