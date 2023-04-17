#include "init.h"
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
    TIM2_NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 1;
    TIM2_NVIC_Struct.NVIC_IRQChannelSubPriority = 2,
    NVIC_Init(&TIM2_NVIC_Struct);

    TIM_Cmd(TIM2,ENABLE);
}

//定时器频率为72000000/(xxx+1)
//PWM频率为 定时器频率/period
//TIM_SetCompare2(TIM3,xxx);//设置通道2占空比
volatile uint32_t tim_tick = 0;
uint32_t inline tim_get_tick(void)
{
    return tim_tick;
}

void tim3_init(uint16_t Period,uint16_t PSC)
{  
    TIM_TimeBaseInitTypeDef TIM3_Base_Struct;

    NVIC_InitTypeDef TIM3_NVIC_Struct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

    TIM3_Base_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM3_Base_Struct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM3_Base_Struct.TIM_Period = Period;
    TIM3_Base_Struct.TIM_Prescaler = PSC;
    TIM_TimeBaseInit(TIM3,&TIM3_Base_Struct);

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    TIM3_NVIC_Struct.NVIC_IRQChannel = TIM3_IRQn;
    TIM3_NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
    TIM3_NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 1;
    TIM3_NVIC_Struct.NVIC_IRQChannelSubPriority = 1,
    NVIC_Init(&TIM3_NVIC_Struct);
    TIM_Cmd(TIM3,ENABLE);
}

//定时器频率为72000000/(xxx+1)
//PWM频率为 定时器频率/period
//TIM_SetCompare2(TIM4,xxx);//设置通道2占空比
void tim4_init(uint16_t Period,uint16_t PSC)
{  
    TIM_TimeBaseInitTypeDef TIM4_Base_Struct;
#if USE_PWM
    GPIO_InitTypeDef TIM4_GPIO_Struct;
    TIM_OCInitTypeDef TIM4_OC_Struct;
#endif
    NVIC_InitTypeDef TIM4_NVIC_Struct;
#if USE_PWM
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
#endif
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
#if USE_PWM
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM4,ENABLE);

    TIM4_GPIO_Struct.GPIO_Mode = GPIO_Mode_AF_PP;
    TIM4_GPIO_Struct.GPIO_Pin = GPIO_Pin_5;
    TIM4_GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&TIM4_GPIO_Struct);
#endif
    TIM4_Base_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM4_Base_Struct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM4_Base_Struct.TIM_Period = Period;
    TIM4_Base_Struct.TIM_Prescaler = PSC;
    TIM_TimeBaseInit(TIM4,&TIM4_Base_Struct);
#if USE_PWM
    TIM4_OC_Struct.TIM_OCMode = TIM_OCMode_PWM1;//模式1
    TIM4_OC_Struct.TIM_OutputNState = TIM_OutputState_Enable;
    TIM4_OC_Struct.TIM_OCNPolarity = TIM_OCPolarity_High;
    //TIM4_OC_Struct.TIM_Pulse = xxxx;//设置占空比
    TIM_OC2Init(TIM4,&TIM4_OC_Struct);//通道2
    TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable); //使能输出比较预装载
    TIM_ARRPreloadConfig(TIM4,ENABLE); //影子寄存器
#endif
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    TIM4_NVIC_Struct.NVIC_IRQChannel = TIM4_IRQn;
    TIM4_NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
    TIM4_NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 1;
    TIM4_NVIC_Struct.NVIC_IRQChannelSubPriority = 0,
    NVIC_Init(&TIM4_NVIC_Struct);
    TIM_Cmd(TIM4,ENABLE);
}


//mpu6050
extern data_buff_t all_data;
void TIM2_IRQHandler(void)
{
    if(TIM_GetFlagStatus(TIM2,TIM_IT_Update) != RESET){
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
    }
}


//任务定时器

#if 0
void TIM3_IRQHandler(void)
{
    if(TIM_GetFlagStatus(TIM3,TIM_IT_Update) != RESET){
            tim_tick += TIM_IT_TIME;
        
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
    }
}
#endif
#if 1
extern list_item long_task;
void TIM3_IRQHandler(void)
{
    if(TIM_GetFlagStatus(TIM3,TIM_IT_Update) != RESET){
            list_item *item;
            list_item *n;
            Task_t* m_task;
            list_for_each_next_safe(item,n,&long_task){
            m_task = container_of(Task_t,task_item,item);
            
            if(!m_task->atomic){ //任务在执行时不能减少时间片
                m_task->remain -= HIT_TIME;
			}else{
            }
        }
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
    }
}
#endif

void TIM4_IRQHandler(void)
{
    if(TIM_GetFlagStatus(TIM2,TIM_IT_Update) != RESET){

        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
    }
}

