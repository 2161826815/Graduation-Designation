#ifndef __ESP_USART_H
#define __ESP_USART_H

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define ESP8266_Port                    GPIOA
#define ESP8266_Pin_TX                  GPIO_Pin_2
#define ESP8266_Pin_RX                  GPIO_Pin_3

#define ESP8266_USARTX                  USART2
#define ESP8266_Baudrate                115200

#define ESP8266_IRQn                    USART2_IRQn

#define DMA_SIZE                        256
#define ESP8266_RX_DMA_CHANNEL          DMA1_Channel6



void ESP8266_GPIO_Config(void);
void USART_Send_One_Byte(USART_TypeDef* USARTx,uint8_t byte);
void USART_Send_str(USART_TypeDef* USARTx,uint8_t *bytes);
void DMA_Reuse(DMA_Channel_TypeDef* DMA_CHx);
//void USARTx_Send_data(USART_TypeDef *USARTx, char *fmt,...);

#endif
