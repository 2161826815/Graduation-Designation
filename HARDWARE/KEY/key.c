#include "key.h"

void Key_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_Struct;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_Struct.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_Struct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_Struct);

    NVIC_Struct.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_Init(&NVIC_Struct);
}

void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_Struct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

    GPIO_Struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Struct.GPIO_Pin = key3_pin | key4_pin;
    GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(key3_port,&GPIO_Struct);
    GPIO_Struct.GPIO_Pin = key2_pin | key5_pin;
    GPIO_Init(key5_port,&GPIO_Struct);

    Key_NVIC_Config();

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource14);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource15);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    
    EXTI_InitTypeDef EXTI_Struct;
    EXTI_Struct.EXTI_Line = EXTI_Line14 | EXTI_Line15 | EXTI_Line8 | EXTI_Line9;
    EXTI_Struct.EXTI_LineCmd = ENABLE;
    EXTI_Struct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_Struct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_Struct);
    
}

