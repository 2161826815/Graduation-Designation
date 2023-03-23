#include "USART.h"

void Debug_USART_init(void)
{
    GPIO_InitTypeDef gpio_struct;
    USART_InitTypeDef usart_struct;

    RCC_APB2PeriphClockCmd(Debug_Usart_RCC_GPIO_Port,ENABLE);
    RCC_APB2PeriphClockCmd(Debug_Usart_RCC_Port,ENABLE);

    //GPIO初始化
    gpio_struct.GPIO_Pin = Debug_Usart_GPIO_Rx_Pin;
    gpio_struct.GPIO_Mode = Debug_Usart_GPIO_Rx_Mode;
    gpio_struct.GPIO_Speed = Debug_Usart_GPIO_Speed;
    GPIO_Init(Debug_Usart_GPIO_Port,&gpio_struct);

    gpio_struct.GPIO_Pin = Debug_Usart_GPIO_Tx_Pin;
    gpio_struct.GPIO_Mode = Debug_Usart_GPIO_Tx_Mode;
    GPIO_Init(Debug_Usart_GPIO_Port,&gpio_struct);


    //串口初始化
    usart_struct.USART_BaudRate = Debug_Usart_BaudRate;
    usart_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_struct.USART_Mode = Debug_Usart_Mode;
    usart_struct.USART_Parity = Debug_Usart_Parity;
    usart_struct.USART_StopBits = Debug_Usart_StopBits;
    usart_struct.USART_WordLength = Debug_Usart_WordLenth;
    USART_Init(Debug_Usart,&usart_struct);

    //NVIC初始化
    NVIC_InitTypeDef nvic_struct;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    nvic_struct.NVIC_IRQChannel = Debug_Usart_IRQ_Channel;
    nvic_struct.NVIC_IRQChannelCmd = ENABLE;
    nvic_struct.NVIC_IRQChannelPreemptionPriority = 1;
    nvic_struct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&nvic_struct);

    USART_ITConfig(Debug_Usart,USART_IT_RXNE,ENABLE); //使能串口接收中断
    USART_Cmd(Debug_Usart,ENABLE); //使能串口

}

void Debug_USART_Send_Byte(USART_TypeDef* USARTX,uint8_t data)
{
    USART_SendData(USARTX,data);
    while(USART_GetFlagStatus(USARTX,USART_FLAG_TXE) == RESET);
}

void Debug_USART_Send_Array(USART_TypeDef* USARTX,uint8_t* array,uint8_t num)
{
    uint8_t i;
    for(i=0;i<num;i++){
        Debug_USART_Send_Byte(USARTX,*array++);
    }
    while(USART_GetFlagStatus(USARTX,USART_FLAG_TC) == RESET);
}

void Debug_USART_Send_Str(USART_TypeDef* USARTX,uint8_t* str)
{
    uint8_t i=0;
    while(*(str+i) != '\0'){
        Debug_USART_Send_Byte(USARTX,*(str+i));
        i++;
    }
    while(USART_GetFlagStatus(USARTX,USART_FLAG_TC) == RESET);
}

//输出重定向使用printf函数
int fputc(int ch,FILE* f)
{
    USART_SendData(Debug_Usart,(uint8_t)ch);

    while(USART_GetFlagStatus(Debug_Usart,USART_FLAG_TXE) == RESET);

    return ch;
}

//输入重定向使用scanf函数
int fgetc(FILE* f)
{
    while(USART_GetFlagStatus(Debug_Usart,USART_FLAG_RXNE) == RESET);

    return (int)(USART_ReceiveData(Debug_Usart));
}
