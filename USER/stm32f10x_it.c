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
#include "main.h"
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
void USART1_IRQHandler(void)         //Problem 1
{
  uint8_t temp;
  if(USART_GetFlagStatus(Debug_Usart,USART_FLAG_RXNE) != RESET){
    temp = USART_ReceiveData(Debug_Usart);
    //USART_SendData(Debug_Usart,temp);
    USART_SendData(ESP8266_USARTX,temp);//串口调试ESP
    USART_ClearFlag(Debug_Usart, USART_FLAG_RXNE);
  }
}

void EXTI_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) == SET){
    printf("1\r\n");
    EXTI_ClearITPendingBit(EXTI_Line8);
  }
}

extern uint8_t MAX30102_RDY;
void EXTI9_5_IRQHandler(void)
{
  //KEY3
  if(EXTI_GetITStatus(EXTI_Line8) == SET){

    EXTI_ClearITPendingBit(EXTI_Line8);
  }

  //KEY4
  if(EXTI_GetITStatus(EXTI_Line9) == SET){

    EXTI_ClearITPendingBit(EXTI_Line9);
  }

  //MAX30102
  if(EXTI_GetITStatus(EXTI_Line5) == SET){
    MAX30102_RDY = 1;

    EXTI_ClearITPendingBit(EXTI_Line5);
  }
}


void EXTI15_10_IRQHandler(void)
{
  //KEY2
  if(EXTI_GetITStatus(EXTI_Line14) == SET){

    EXTI_ClearITPendingBit(EXTI_Line15);
  }
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
