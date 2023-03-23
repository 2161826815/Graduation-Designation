#include "LED.h"

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(LED_Clock_Port,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO,ENABLE);

	//关闭PC13侵入检测功能，使能PC13作为普通IO口
	BKP_TamperPinCmd(DISABLE);		
	BKP_ITConfig(DISABLE);

	GPIO_InitTypeDef LED_Struct;
	LED_Struct.GPIO_Pin = LED3_Pin |  LED4_Pin |  LED5_Pin;
	LED_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	LED_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(LED3_Port,&LED_Struct);

	LED_Struct.GPIO_Pin = LED1_Pin;
	GPIO_Init(LED1_Port,&LED_Struct);

	GPIO_ResetBits(LED3_Port,LED3_Pin |  LED4_Pin |  LED5_Pin);
	GPIO_ResetBits(LED1_Port,LED1_Pin);
}

void LED_ON(int num)
{
		switch(num){
		case 1 : 
			GPIO_ResetBits(LED1_Port,LED1_Pin);
			break;
		case 3 : 
			GPIO_ResetBits(LED3_Port,LED3_Pin);
			break;
		case 4 : 
			GPIO_ResetBits(LED4_Port,LED4_Pin);
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
		case 3 : 
			GPIO_SetBits(LED3_Port,LED3_Pin);
			break;
		case 4 : 
			GPIO_SetBits(LED4_Port,LED4_Pin);
			break;
		case 5 : 
			GPIO_SetBits(LED5_Port,LED5_Pin);
			break;
		default :
			break;
	}
}

