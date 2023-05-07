#include "max_soft_iic.h"
#include "SysTick.h"

void Max_Soft_IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = Max_Soft_IIC_SCL | Max_Soft_IIC_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Max_Soft_IIC_Port, &GPIO_InitStructure);

	Max_IIC_SDA = 1;
	Max_IIC_SCL = 1;
}

void Max_Soft_IIC_Start(void)
{
	SDA_OUT();
	Max_IIC_SDA = 1;
	Max_IIC_SCL = 1;
	delay_us(4);
	Max_IIC_SDA = 0;
	delay_us(4);
	Max_IIC_SCL = 0;
}

void Max_Soft_IIC_Stop(void)
{
	SDA_OUT();
	Max_IIC_SCL = 0;
	Max_IIC_SDA = 0;
	delay_us(4);
	Max_IIC_SCL = 1;
	Max_IIC_SDA = 1;
	delay_us(4);
}

uint8_t Max_Soft_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime = 0;
	SDA_IN();
	Max_IIC_SDA = 1;
	delay_us(1);
	Max_IIC_SCL = 1;
	delay_us(1);
	while (Max_READ_SDA){
		ucErrTime++;
		if (ucErrTime > 250){
			Max_Soft_IIC_Stop();
			return 1;
		}
	}
	Max_IIC_SCL = 0;
	return 0;
}

void Max_Soft_IIC_Ack(void)
{
	Max_IIC_SCL = 0;
	SDA_OUT();
	Max_IIC_SDA = 0;
	delay_us(2);
	Max_IIC_SCL = 1;
	delay_us(2);
	Max_IIC_SCL = 0;
}

void Max_Soft_IIC_NAck(void)
{
	Max_IIC_SCL = 0;
	SDA_OUT();
	Max_IIC_SDA = 1;
	delay_us(2);
	Max_IIC_SCL = 1;
	delay_us(2);
	Max_IIC_SCL = 0;
}

void Max_Soft_IIC_Send_Byte(uint8_t byte)
{
	uint8_t t;
	SDA_OUT();
	Max_IIC_SCL = 0;
	for (t = 0; t < 8; t++){
		Max_IIC_SDA = (byte & 0x80) >> 7;
		byte <<= 1;
		delay_us(2);
		Max_IIC_SCL = 1;
		delay_us(2);
		Max_IIC_SCL = 0;
		delay_us(2);
	}
}

uint8_t Max_Soft_IIC_Rcv_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	SDA_IN();
	for (i = 0; i < 8; i++){
		Max_IIC_SCL = 0;
		delay_us(2);
		Max_IIC_SCL = 1;
		receive <<= 1;
		if (Max_READ_SDA)
			receive++;
		delay_us(1);
	}
	if (!ack)
		Max_Soft_IIC_NAck();
	else
		Max_Soft_IIC_Ack();
	return receive;
}

void Max_Soft_IIC_Write_One_Byte(uint8_t addr, uint8_t reg, uint8_t data)
{
	Max_Soft_IIC_Start();

	Max_Soft_IIC_Send_Byte(addr);
	Max_Soft_IIC_Wait_Ack();

	Max_Soft_IIC_Send_Byte(reg);
	Max_Soft_IIC_Wait_Ack();

	Max_Soft_IIC_Send_Byte(data);
	Max_Soft_IIC_Wait_Ack();

	Max_Soft_IIC_Stop();
	delay_ms(10);
}

void Max_Soft_IIC_Read_One_Byte(uint8_t addr, uint8_t reg, uint8_t *data)
{
	Max_Soft_IIC_Start();

	Max_Soft_IIC_Send_Byte(addr);
	Max_Soft_IIC_Wait_Ack();

	Max_Soft_IIC_Send_Byte(reg);
	Max_Soft_IIC_Wait_Ack();

	Max_Soft_IIC_Start();
	Max_Soft_IIC_Send_Byte(addr | 0x01);
	Max_Soft_IIC_Wait_Ack();

	*data = Max_Soft_IIC_Rcv_Byte(0);
	Max_Soft_IIC_Stop();
}

void Max_Soft_IIC_Write_Bytes(uint8_t addr,uint8_t *buf ,uint8_t len)
{
	uint8_t i;
	Max_Soft_IIC_Start();
	Max_Soft_IIC_Send_Byte(addr);
	Max_Soft_IIC_Wait_Ack();

	for (i = 0; i < len; i++){
		Max_Soft_IIC_Send_Byte(buf[i]);
		Max_Soft_IIC_Wait_Ack();
	}
	Max_Soft_IIC_Stop();
	delay_ms(10);
}
void Max_Soft_IIC_Read_Bytes(uint8_t addr, uint8_t reg, uint8_t *buf , uint8_t len)
{
	uint8_t i = 1;
	Max_Soft_IIC_Start();

	Max_Soft_IIC_Send_Byte(addr);
	Max_Soft_IIC_Wait_Ack();

	Max_Soft_IIC_Send_Byte(reg);
	Max_Soft_IIC_Wait_Ack();

	Max_Soft_IIC_Send_Byte(addr | 0x01);
	Max_Soft_IIC_Wait_Ack();

	for(i=0;i<len-1;i++){
		buf[i] = Max_Soft_IIC_Rcv_Byte(1);
	}
	buf[len-1] = Max_Soft_IIC_Rcv_Byte(0);
	Max_Soft_IIC_Stop();
	delay_ms(10);
}
