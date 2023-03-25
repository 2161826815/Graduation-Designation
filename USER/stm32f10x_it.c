/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "USART.h"
#include "ESP_USART.h"
#include <string.h>
#include "LED.h"
#include "beep.h"
#include "ESP8266.h"
#include "oled.h"
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}

void USART1_IRQHandler(void)         //Problem 1
{
  uint8_t temp;
  if(USART_GetFlagStatus(Debug_Usart,USART_FLAG_RXNE) != RESET){
    //USART_ClearITPendingBit(Debug_Usart,USART_IT_RXNE);
    temp = USART_ReceiveData(Debug_Usart);
    //USART_SendData(Debug_Usart,temp);
    printf("h\r\n");
    USART_Send_One_Byte(ESP8266_USARTX,temp);//串口调试ESP
    USART_ClearFlag(Debug_Usart, USART_FLAG_RXNE);
  }
}

void USART2_IRQHandler(void)     //ESP8266串口DMA空闲中断    
{
  extern uint8_t DMA_RCV_Buffer[DMA_SIZE];
  extern uint8_t RCV_CNT;
#ifdef RXNE
  uint8_t temp;
  if(USART_GetFlagStatus(ESP8266_USARTX,USART_IT_RXNE) != RESET){
    temp = USART_ReceiveData(ESP8266_USARTX);
    DMA_RCV_Buffer[RCV_CNT++] = temp;
    USART_SendData(Debug_Usart,temp);
    if(temp == '\0'){
      RCV_CNT = 0;
      //printf("rcv:%s",DMA_RCV_Buffer);
    }
    USART_ClearFlag(ESP8266_USARTX,USART_FLAG_RXNE);
  }
#endif
#ifdef IDLE
  if(USART_GetFlagStatus(ESP8266_USARTX,USART_FLAG_IDLE) == SET){
    RCV_CNT = USART1->SR;
    RCV_CNT = USART1->DR; //清USART_IT_IDLE标志
    DMA_Cmd(ESP8266_RX_DMA_CHANNEL,DISABLE);
    RCV_CNT = DMA_SIZE-DMA_GetCurrDataCounter(ESP8266_RX_DMA_CHANNEL);
    //printf("cnt=%d\r\n",RCV_CNT);
    DMA_SetCurrDataCounter(ESP8266_RX_DMA_CHANNEL,DMA_SIZE);//重新设置当前DMA容量


    DMA_Cmd(ESP8266_RX_DMA_CHANNEL,ENABLE);
    DMA_Reuse(ESP8266_RX_DMA_CHANNEL);
    USART_ClearFlag(ESP8266_USARTX,USART_IT_IDLE);
  }
#endif
}

void EXTI9_5_IRQHandler(void)
{
  //KEY3
  if(EXTI_GetITStatus(EXTI_Line8) == SET){
		LED_Toggle(1);
    LED_Toggle(3);
    
    ESP8266_Pub_Data(180,Type_HR);

    EXTI_ClearITPendingBit(EXTI_Line8);
  }

  //KEY4
  if(EXTI_GetITStatus(EXTI_Line9) == SET){
    LED_Toggle(4);
    LED_Toggle(5);

    EXTI_ClearITPendingBit(EXTI_Line9);
  }
    
}

void EXTI15_10_IRQHandler(void)
{
  //KEY2
  if(EXTI_GetITStatus(EXTI_Line14) == SET){

    BEEP_OFF();                                     //关闭蜂鸣器

    EXTI_ClearITPendingBit(EXTI_Line14);
  }

  //KEY5
  extern uint8_t OLED_State;
  if(EXTI_GetITStatus(EXTI_Line15) == SET){         //开启关闭OLED
    if(OLED_State){
      OLED_Display_On();
      OLED_State = !OLED_State; 
    }else{
      OLED_Display_On();
      OLED_State = !OLED_State;
    }
      

    EXTI_ClearITPendingBit(EXTI_Line15);
  }
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
