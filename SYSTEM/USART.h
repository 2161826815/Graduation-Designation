#ifndef __USART_H

#define __USART_H

#include "stm32f10x.h"
#include <stdio.h>


#define Debug_Usart_RCC_GPIO_Port     RCC_APB2Periph_GPIOA
#define Debug_Usart_RCC_Port          RCC_APB2Periph_USART1

#define Debug_Usart_GPIO_Port         GPIOA
#define Debug_Usart                   USART1

#define Debug_Usart_GPIO_Speed        GPIO_Speed_50MHz

#define Debug_Usart_GPIO_Rx_Pin       GPIO_Pin_10
#define Debug_Usart_GPIO_Tx_Pin       GPIO_Pin_9

#define Debug_Usart_GPIO_Tx_Mode      GPIO_Mode_AF_PP
#define Debug_Usart_GPIO_Rx_Mode      GPIO_Mode_IN_FLOATING

#define Debug_Usart_Mode              (USART_Mode_Rx|USART_Mode_Tx)
#define Debug_Usart_BaudRate          115200
#define Debug_Usart_Parity            USART_Parity_No
#define Debug_Usart_StopBits          USART_StopBits_1
#define Debug_Usart_WordLenth         USART_WordLength_8b

#define Debug_Usart_IRQ_Channel       USART1_IRQn
#define Debug_Usart_IRQ_Handler       USART1_IRQHandler


void Debug_USART_init(void);
void Debug_USART_Send_Byte(USART_TypeDef* USARTX,uint8_t data);
void Debug_USART_Send_Array(USART_TypeDef* USARTX,uint8_t* array,uint8_t num);
void Debug_USART_Send_Str(USART_TypeDef* USARTX,uint8_t* str);

#endif
