#include "USART.h"

uint8_t DBG_Buffer[128];
void Debug_USART_init(void)
{
    GPIO_InitTypeDef gpio_struct;
    USART_InitTypeDef usart_struct;

    RCC_APB2PeriphClockCmd(Debug_Usart_RCC_GPIO_Port,ENABLE);
    RCC_APB2PeriphClockCmd(Debug_Usart_RCC_Port,ENABLE);

    gpio_struct.GPIO_Pin = Debug_Usart_GPIO_Rx_Pin;
    gpio_struct.GPIO_Mode = Debug_Usart_GPIO_Rx_Mode;
    gpio_struct.GPIO_Speed = Debug_Usart_GPIO_Speed;
    GPIO_Init(Debug_Usart_GPIO_Port,&gpio_struct);

    gpio_struct.GPIO_Pin = Debug_Usart_GPIO_Tx_Pin;
    gpio_struct.GPIO_Mode = Debug_Usart_GPIO_Tx_Mode;
    GPIO_Init(Debug_Usart_GPIO_Port,&gpio_struct);

    usart_struct.USART_BaudRate = Debug_Usart_BaudRate;
    usart_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_struct.USART_Mode = Debug_Usart_Mode;
    usart_struct.USART_Parity = Debug_Usart_Parity;
    usart_struct.USART_StopBits = Debug_Usart_StopBits;
    usart_struct.USART_WordLength = Debug_Usart_WordLenth;
    USART_Init(Debug_Usart,&usart_struct);

    NVIC_InitTypeDef NVIC_Struct;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_Struct.NVIC_IRQChannel = Debug_Usart_IRQ_Channel;
    NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_Struct.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&NVIC_Struct);

    USART_ITConfig(Debug_Usart,USART_IT_RXNE,ENABLE);
    USART_Cmd(Debug_Usart,ENABLE);

#if USE_DMA
    DMA_InitTypeDef DNA_Struct;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    DMA_DeInit(DMA1_Channel4);

    DNA_Struct.DMA_BufferSize = 256;
    DNA_Struct.DMA_DIR = DMA_DIR_PeripheralDST;
    DNA_Struct.DMA_M2M = DMA_M2M_Disable;
    DNA_Struct.DMA_MemoryBaseAddr = (uint32_t)DBG_Buffer;
    DNA_Struct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DNA_Struct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DNA_Struct.DMA_Mode = DMA_Mode_Normal;
    DNA_Struct.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
    DNA_Struct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DNA_Struct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DNA_Struct.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_Init(DMA1_Channel4,&DNA_Struct);
    USART_DMACmd(Debug_Usart,USART_DMAReq_Tx,ENABLE);

    NVIC_Struct.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_Struct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_Struct);
    DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);
#endif 
}

void Debug_USART_Send_Byte(USART_TypeDef* USARTX,uint8_t data)
{
    USART_SendData(USARTX,data);
    while(USART_GetFlagStatus(USARTX,USART_FLAG_TXE) == RESET);
}

void Debug_USART_Send_Array(USART_TypeDef* USARTX,uint8_t* array,uint8_t num)
{
    for(uint8_t i=0;i<num;i++){
        Debug_USART_Send_Byte(USARTX,*array++);
    }
    while(USART_GetFlagStatus(USARTX,USART_FLAG_TC) == RESET);
}

void Debug_USART_Send_Str(USART_TypeDef* USARTX,uint8_t* str)
{
    uint8_t i=0;
    while(*(str + i) != '\0'){
        Debug_USART_Send_Byte(USARTX,*(str + i));
        i++;
    }
    while(USART_GetFlagStatus(USARTX,USART_FLAG_TC) == RESET);
}

int fputc(int ch,FILE* f)
{
    USART_SendData(Debug_Usart,(uint8_t)ch);

    while(USART_GetFlagStatus(Debug_Usart,USART_FLAG_TXE) == RESET);

    return ch;
}


#if USE_DMA
static uint8_t DMA_STATUS;
uint16_t USART1_SendBuffer(const char* buffer, uint16_t length)
{
	if( (buffer == NULL) || (length == 0) ){
		return 0;
	}
 
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel4, length);
	DMA_Cmd(DMA1_Channel4, ENABLE);
	while(DMA_STATUS);
    DMA_STATUS = 1;
	return length;
}


void DMA_Printf(const char *format,...)
{
    uint32_t length;
	va_list args;
 
	va_start(args, format);
	length = vsnprintf((char*)DBG_Buffer, sizeof(DBG_Buffer), (char*)format, args);
	va_end(args);
	USART1_SendBuffer((const char*)DBG_Buffer,length);
}

void DMA1_Channel4_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC4)!=RESET){
		DMA_STATUS = 0;
		DMA_ClearITPendingBit(DMA1_IT_TC4);
    }
}
#endif
