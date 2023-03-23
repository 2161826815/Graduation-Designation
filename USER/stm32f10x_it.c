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
    temp = USART_ReceiveData(Debug_Usart);
    USART_SendData(Debug_Usart,temp);
    USART_ClearFlag(Debug_Usart, USART_FLAG_RXNE);
  }
}

void USART2_IRQHandler(void)     //ESP8266串口DMA空闲中断    
{
	extern uint8_t RCV_CNT;
  extern uint8_t DMA_RCV_Buffer[256];
  if(USART_GetFlagStatus(ESP8266_USARTX,USART_FLAG_IDLE) != RESET){
    USART_ReceiveData(ESP8266_USARTX);//这一条是为了清楚中断标志位
    RCV_CNT = DMA_SIZE-DMA_GetCurrDataCounter(ESP8266_RX_DMA_CHANNEL);
    if(RCV_CNT > 0){
      if(strstr((const char*)DMA_RCV_Buffer,"LED1_ON") != NULL)
          LED_ON(1);
      if(strstr((const char*)DMA_RCV_Buffer,"LED1_OFF") != NULL)
          LED_OFF(1);
      if(strstr((const char*)DMA_RCV_Buffer,"LED3_ON") != NULL)
          LED_ON(3);
      if(strstr((const char*)DMA_RCV_Buffer,"LED3_OFF") != NULL)
          LED_OFF(3);
      if(strstr((const char*)DMA_RCV_Buffer,"LED4_ON") != NULL)
          LED_ON(4);
      if(strstr((const char*)DMA_RCV_Buffer,"LED4_OFF") != NULL)
          LED_OFF(4);
      if(strstr((const char*)DMA_RCV_Buffer,"LED5_ON") != NULL)
          LED_ON(5);
      if(strstr((const char*)DMA_RCV_Buffer,"LED5_OFF") != NULL)
          LED_OFF(5);
      if(strstr((const char*)DMA_RCV_Buffer,"BEEP_ON") != NULL)
          BEEP_ON();
      if(strstr((const char*)DMA_RCV_Buffer,"BEEP_OFF") != NULL)
          BEEP_OFF();
    }
    USART_ClearFlag(ESP8266_USARTX,USART_FLAG_IDLE);
    DMA_Reuse(ESP8266_RX_DMA_CHANNEL);
  }
}

void EXTI9_5_IRQHandler(void)
{
  //KEY3
  if(EXTI_GetITStatus(EXTI_Line8) == SET){
		LED_ON(1);
    


    EXTI_ClearITPendingBit(EXTI_Line8);
  }

  //KEY4
  if(EXTI_GetITStatus(EXTI_Line9) == SET){


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
