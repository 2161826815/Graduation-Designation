#include "LED.h"
#include "init.h"
Task_t m_led_task;
Task_t m_led2_task;
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

	GPIO_SetBits(LED3_Port,LED3_Pin |  LED4_Pin |  LED5_Pin);
	GPIO_SetBits(LED1_Port,LED1_Pin);
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

void LED_Toggle(int num)
{
	switch(num){
		case 1 : 
			LED1_Port->ODR ^= LED1_Pin;
			break;
		case 3 : 
			LED3_Port->ODR ^= LED3_Pin;
			break;
		case 4 : 
			LED4_Port->ODR ^= LED4_Pin;
			break;
		case 5 : 
			LED5_Port->ODR ^= LED5_Pin;
			break;
		default :
			break;
	}
}

void led_task(void)
{
	//printf("led task\r\n");
	//delay_ms(20);
}

void led2_task(void)
{
	//printf("led2 task\r\n");
	//delay_ms(20);
}

void led_task_init(void)
{
    m_led_task.Period = LED_Period;
	m_led_task.remain= m_led2_task.Period;
	m_led_task.priority = 9;
    m_led_task.task = &led_task;
}

void led2_task_init(void)
{
    m_led2_task.Period = LED2_Period;
	m_led2_task.remain= m_led2_task.Period;
	m_led2_task.priority = 10;
    m_led2_task.task = &led2_task;
}
