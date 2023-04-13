#include "TIM.h"
#include "USART.h"
void tim2_init(uint16_t Period,uint16_t PSC)
{
    TIM_TimeBaseInitTypeDef TIM2_Base_Struct;
    NVIC_InitTypeDef    TIM2_NVIC_Struct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

    TIM2_Base_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM2_Base_Struct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM2_Base_Struct.TIM_Period = Period;
    TIM2_Base_Struct.TIM_Prescaler = PSC;
    TIM_TimeBaseInit(TIM2,&TIM2_Base_Struct);

    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    TIM2_NVIC_Struct.NVIC_IRQChannel = TIM2_IRQn;
    TIM2_NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
    TIM2_NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0;
    TIM2_NVIC_Struct.NVIC_IRQChannelSubPriority = 0,
    NVIC_Init(&TIM2_NVIC_Struct);

    TIM_Cmd(TIM2,ENABLE);
}

//定时器频率为72000000/(xxx+1)
//PWM频率为 定时器频率/period
//TIM_SetCompare2(TIM3,xxx);//设置通道2占空比
void tim3_init(uint16_t Period,uint16_t PSC)
{
    GPIO_InitTypeDef TIM3_GPIO_Struct;
    TIM_TimeBaseInitTypeDef TIM3_Base_Struct;
    TIM_OCInitTypeDef TIM3_OC_Struct;
    NVIC_InitTypeDef TIM3_NVIC_Struct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);

    TIM3_GPIO_Struct.GPIO_Mode = GPIO_Mode_AF_PP;
    TIM3_GPIO_Struct.GPIO_Pin = GPIO_Pin_5;
    TIM3_GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&TIM3_GPIO_Struct);

    TIM3_Base_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM3_Base_Struct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM3_Base_Struct.TIM_Period = Period;
    TIM3_Base_Struct.TIM_Prescaler = PSC;
    TIM_TimeBaseInit(TIM3,&TIM3_Base_Struct);

    TIM3_OC_Struct.TIM_OCMode = TIM_OCMode_PWM1;//模式1
    TIM3_OC_Struct.TIM_OutputNState = TIM_OutputState_Enable;
    TIM3_OC_Struct.TIM_OCNPolarity = TIM_OCPolarity_High;
    //TIM3_OC_Struct.TIM_Pulse = xxxx;//设置占空比
    TIM_OC2Init(TIM3,&TIM3_OC_Struct);//通道2
    TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable); //使能输出比较预装载
    TIM_ARRPreloadConfig(TIM3,ENABLE); //影子寄存器

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    TIM3_NVIC_Struct.NVIC_IRQChannel = TIM2_IRQn;
    TIM3_NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
    TIM3_NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0;
    TIM3_NVIC_Struct.NVIC_IRQChannelSubPriority = 1,
    NVIC_Init(&TIM3_NVIC_Struct);

    TIM_Cmd(TIM3,ENABLE);
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetFlagStatus(TIM2,TIM_IT_Update) != RESET){
        printf("TIM2 IT\r\n");
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
    }
}

/* void TIM3_IRQHandler(void)
{
    if(TIM_GetFlagStatus(TIM3,TIM_IT_Update) != RESET){
        printf("TIM3 PWM IT\r\n");
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
    }
}
 */