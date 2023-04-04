#include "ESP_USART.h"
#include "USART.h"
uint8_t DMA_RCV_Buffer[DMA_SIZE];

void ESP8266_GPIO_Config(void)
{
    GPIO_InitTypeDef  GPIO_Struct;
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_Struct.GPIO_Mode = GPIO_Mode_AF_PP;  //TX
    GPIO_Struct.GPIO_Pin = ESP8266_Pin_TX;
    GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ESP8266_Port,&GPIO_Struct);

    GPIO_Struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //RX
    GPIO_Struct.GPIO_Pin = ESP8266_Pin_RX;
    GPIO_Init(ESP8266_Port,&GPIO_Struct);

    USART_InitTypeDef USART_Struct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    USART_Struct.USART_BaudRate = ESP8266_Baudrate;
    USART_Struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Struct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //TX
    USART_Struct.USART_Parity = USART_Parity_No;
    USART_Struct.USART_StopBits = USART_StopBits_1;
    USART_Struct.USART_WordLength = USART_WordLength_8b;
    USART_Init(ESP8266_USARTX,&USART_Struct);
    USART_Cmd(ESP8266_USARTX,ENABLE);

    NVIC_InitTypeDef NVIC_Struct;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_Struct.NVIC_IRQChannel = ESP8266_IRQn;
    NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_Struct.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_Struct);

#ifdef IDLE  
    USART_ITConfig(ESP8266_USARTX,USART_IT_IDLE,ENABLE); //空闲中断
#endif

#ifdef RXNE
    USART_ITConfig(ESP8266_USARTX,USART_IT_RXNE,ENABLE); //接收中断
#endif

#ifdef IDLE  
    USART_DMACmd(ESP8266_USARTX,USART_DMAReq_Rx,ENABLE); //使能串口DMA接受
    

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//DMAs时钟
    DMA_InitTypeDef DMA_Struct;
    DMA_Struct.DMA_BufferSize = DMA_SIZE;//
    DMA_Struct.DMA_DIR = DMA_DIR_PeripheralSRC; //DMA接受方向
    DMA_Struct.DMA_M2M = DMA_M2M_Disable;
    DMA_Struct.DMA_MemoryBaseAddr = (uint32_t)DMA_RCV_Buffer;
    DMA_Struct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_Struct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //DMA_Struct.DMA_Mode = DMA_Mode_Normal;
    DMA_Struct.DMA_Mode = DMA_Mode_Circular;
    DMA_Struct.DMA_PeripheralBaseAddr = (uint32_t)&ESP8266_USARTX->DR;
    DMA_Struct.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
    DMA_Struct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_Struct.DMA_Priority = DMA_Priority_High;
    DMA_Init(ESP8266_RX_DMA_CHANNEL,&DMA_Struct); //DMA1 通道6 串口2接收
    DMA_ITConfig(ESP8266_RX_DMA_CHANNEL,DMA_IT_TC,ENABLE);
    DMA_ClearFlag(DMA1_IT_TC6);
    DMA_Cmd(ESP8266_RX_DMA_CHANNEL,ENABLE); //使能
#endif
}

void USART_Send_One_Byte(USART_TypeDef* USARTx,uint8_t byte)
{
    USART_SendData(ESP8266_USARTX,byte);
    while(!USART_GetFlagStatus(ESP8266_USARTX,USART_FLAG_TXE));
}

void USART_Send_str(USART_TypeDef* USARTx,uint8_t *bytes)
{
    while(*bytes != '\0'){
        USART_Send_One_Byte(USARTx,*bytes);
        bytes++;
    }
    while(!USART_GetFlagStatus(ESP8266_USARTX,USART_FLAG_TC));
}

void DMA_Reuse(DMA_Channel_TypeDef* DMA_CHx)
{ 
    DMA_Cmd(DMA_CHx, DISABLE );  //关闭USART1 TX DMA1所指示的通道    
    DMA_SetCurrDataCounter(DMA_CHx,DMA_SIZE);//DMA通道的DMA缓存的大小
    DMA_Cmd(DMA_CHx, ENABLE);  //打开USART1 TX DMA1所指示的通道  
}


void USART2_IRQHandler(void)     //ESP8266串口DMA空闲中断    
{
#ifdef RXNE
  uint8_t temp;
  extern uint8_t DMA_RCV_Buffer[DMA_SIZE];
  extern uint8_t RCV_CNT;
  if(USART_GetFlagStatus(ESP8266_USARTX,USART_IT_RXNE) != RESET){
    temp = USART_ReceiveData(ESP8266_USARTX);
    DMA_RCV_Buffer[RCV_CNT++] = temp;
    if(temp == '\0'){
      RCV_CNT = 0;
    }
    USART_ClearFlag(ESP8266_USARTX,USART_FLAG_RXNE);
  }
#endif

#ifdef IDLE
    
  if(USART_GetFlagStatus(ESP8266_USARTX,USART_FLAG_IDLE) == SET){
    extern uint8_t DMA_TC_STATUS;
    extern uint8_t DMA_RCV_Buffer[DMA_SIZE];
    extern uint8_t RCV_CNT;
    uint8_t temp;
    temp = USART1->SR;
    temp = USART1->DR; //清USART_IT_IDLE标志
    RCV_CNT = DMA_SIZE-DMA_GetCurrDataCounter(ESP8266_RX_DMA_CHANNEL);
    DMA_TC_STATUS = 1;
    DMA_TC_STATUS = 1;
    DMA_Cmd(ESP8266_RX_DMA_CHANNEL,DISABLE);
    DMA_SetCurrDataCounter(ESP8266_RX_DMA_CHANNEL,DMA_SIZE);
    DMA_Cmd(ESP8266_RX_DMA_CHANNEL,ENABLE);
    USART_ClearFlag(ESP8266_USARTX,USART_IT_IDLE);
  }
#endif
}
#ifdef IDLE
void DMA1_Channel6_IRQHandler(void)
{
    printf("1dasca\r\n");
    extern uint8_t DMA_TC_STATUS;
    extern uint8_t DMA_RCV_Buffer[DMA_SIZE];
    extern uint8_t RCV_CNT;
    if(DMA_GetITStatus(DMA1_IT_TC6)){
        RCV_CNT = DMA_SIZE-DMA_GetCurrDataCounter(ESP8266_RX_DMA_CHANNEL);
        DMA_TC_STATUS = 1;
        DMA_SetCurrDataCounter(ESP8266_RX_DMA_CHANNEL,DMA_SIZE);
        DMA_ClearITPendingBit(DMA1_IT_TC6);
    }
}
#endif