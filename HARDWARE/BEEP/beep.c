#include "beep.h"

void BEEP_Init(void)
{
	RCC_APB2PeriphClockCmd(BEEP_Clock_Port,ENABLE);
	
	GPIO_InitTypeDef BEEP_Struct;
	BEEP_Struct.GPIO_Pin = BEEP_Pin;
	BEEP_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	BEEP_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(BEEP_Port,&BEEP_Struct);
	GPIO_SetBits(BEEP_Port,BEEP_Pin);
}

void BEEP_ON(void)
{
	GPIO_ResetBits(BEEP_Port,BEEP_Pin);
}

void BEEP_OFF(void)
{
	GPIO_SetBits(BEEP_Port,BEEP_Pin);
}

void BEEP_Toggle(void)
{
	BEEP_Port->ODR ^= BEEP_Pin;
}
