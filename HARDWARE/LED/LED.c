#include "LED.h"
#include "init.h"
Task_t m_led_task;
Task_t m_led2_task;
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(LED_Clock_Port,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO,ENABLE);

	BKP_TamperPinCmd(DISABLE);		
	BKP_ITConfig(DISABLE);

	GPIO_InitTypeDef LED_Struct;
	LED_Struct.GPIO_Pin = LED2_Pin |  LED3_Pin |  LED5_Pin;
	LED_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	LED_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(LED3_Port,&LED_Struct);

	LED_Struct.GPIO_Pin = LED1_Pin;
	GPIO_Init(LED1_Port,&LED_Struct);

	GPIO_SetBits(LED3_Port,LED2_Pin |  LED3_Pin |  LED5_Pin);
	GPIO_SetBits(LED1_Port,LED1_Pin);
}

void LED_ON(int num)
{
		switch(num){
		case 1 : 
			GPIO_ResetBits(LED1_Port,LED1_Pin);
			break;
		case 2 : 
			GPIO_ResetBits(LED2_Port,LED2_Pin);
			break;
		case 3 : 
			GPIO_ResetBits(LED3_Port,LED3_Pin);
			break;
		case 5 : 
			GPIO_ResetBits(LED5_Port,LED5_Pin);
			break;
		default :
			break;
	}
}

void LED_OFF(int num)
{
	switch(num){
		case 1 : 
			GPIO_SetBits(LED1_Port,LED1_Pin);
			break;
		case 2 : 
			GPIO_SetBits(LED2_Port,LED2_Pin);
			break;
		case 3 : 
			GPIO_SetBits(LED3_Port,LED3_Pin);
			break;
		case 5 : 
			GPIO_SetBits(LED5_Port,LED5_Pin);
			break;
		default :
			break;
	}
}

void LED_Toggle(int num)
{
	switch(num){
		case 1 : 
			LED1_Port->ODR ^= LED1_Pin;
			break;
		case 2 : 
			LED2_Port->ODR ^= LED2_Pin;
			break;
		case 3 : 
			LED3_Port->ODR ^= LED3_Pin;
			break;
		case 5 : 
			LED5_Port->ODR ^= LED5_Pin;
			break;
		default :
			break;
	}
}

void led1_task(void)
{
	LED_Toggle(1);
}

void led2_task(void)
{
	LED_Toggle(2);
}

