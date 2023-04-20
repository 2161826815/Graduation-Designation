#include "soft_iic.h"
#include "SysTick.h"

void IIC_Delay(void)
{
	delay_us(2);
}

void Soft_IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	

	GPIO_InitStructure.GPIO_Pin = Soft_IIC_SCL | Soft_IIC_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Sofr_IIC_Port, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB, Soft_IIC_SCL | Soft_IIC_SDA);
}

void Soft_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = Soft_IIC_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Soft_SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = Soft_IIC_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Soft_IIC_Start(void)
{
	Soft_SDA_OUT();
	IIC_SDA = 1;
	IIC_SCL = 1;
	IIC_Delay();
	IIC_SDA = 0;
	IIC_Delay();
	IIC_SCL = 0;
}

void Soft_IIC_Stop(void)
{
	Soft_SDA_OUT();
	IIC_SCL = 0;
	IIC_SDA = 0;
	IIC_Delay();
	IIC_SCL = 1;
	IIC_SDA = 1;
	IIC_Delay();
}

uint8_t Soft_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime = 0;
	Soft_SDA_IN();
	IIC_SDA = 1;
	IIC_Delay();
	IIC_SCL = 1;
	IIC_Delay();
	while (READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			Soft_IIC_Stop();
			return 1;
		}
	}
	IIC_SCL = 0;
	return 0;
}

void Soft_IIC_Ack(void)
{
	IIC_SCL = 0;
	Soft_SDA_OUT();
	IIC_SDA = 0;
	IIC_Delay();
	IIC_SCL = 1;
	IIC_Delay();
	IIC_SCL = 0;
}

void Soft_IIC_NAck(void)
{
	IIC_SCL = 0;
	Soft_SDA_OUT();
	IIC_SDA = 1;
	IIC_Delay();
	IIC_SCL = 1;
	IIC_Delay();
	IIC_SCL = 0;
}

void Soft_IIC_Send_Byte(uint8_t txd)
{
	uint8_t t;
	Soft_SDA_OUT();
	IIC_SCL = 0;
	for (t = 0; t < 8; t++)
	{
		IIC_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		IIC_SCL = 1;
		IIC_Delay();
		IIC_SCL = 0;
		IIC_Delay();
	}
}

uint8_t Soft_IIC_Rcv_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	Soft_SDA_IN();
	for (i = 0; i < 8; i++)
	{
		IIC_SCL = 0;
		IIC_Delay();
		IIC_SCL = 1;
		receive <<= 1;
		if (READ_SDA)
			receive++;
		IIC_Delay();
	}
	if (!ack)
		Soft_IIC_NAck();
	else
		Soft_IIC_Ack();
	return receive;
}

uint8_t Soft_IIC_Write_One_Byte(uint8_t addr, uint8_t reg, uint8_t data)
{
	Soft_IIC_Start();
	Soft_IIC_Send_Byte((addr << 1) | 0);
	if (Soft_IIC_Wait_Ack())
	{
		Soft_IIC_Stop();
		return 1;
	}
	Soft_IIC_Send_Byte(reg);
	Soft_IIC_Wait_Ack();
	Soft_IIC_Send_Byte(data);
	if (Soft_IIC_Wait_Ack())
	{
		Soft_IIC_Stop();
		return 1;
	}
	Soft_IIC_Stop();
	return 0;
}

uint8_t Soft_IIC_Write_Bytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	uint8_t i;
	Soft_IIC_Start();
	Soft_IIC_Send_Byte((addr << 1) | 0);
	if (Soft_IIC_Wait_Ack())
	{
		Soft_IIC_Stop();
		return 1;
	}
	Soft_IIC_Send_Byte(reg);
	Soft_IIC_Wait_Ack();
	for (i = 0; i < len; i++)
	{
		Soft_IIC_Send_Byte(buf[i]);
		if (Soft_IIC_Wait_Ack())
		{
			Soft_IIC_Stop();
			return 1;
		}
	}
	Soft_IIC_Stop();
	return 0;
}
uint8_t Soft_IIC_Read_Bytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	Soft_IIC_Start();
	Soft_IIC_Send_Byte((addr << 1) | 0);
	if (Soft_IIC_Wait_Ack())
	{
		Soft_IIC_Stop();
		return 1;
	}
	Soft_IIC_Send_Byte(reg);
	Soft_IIC_Wait_Ack();
	Soft_IIC_Start();
	Soft_IIC_Send_Byte((addr << 1) | 1);
	Soft_IIC_Wait_Ack();
	while (len)
	{
		if (len == 1)
			*buf = Soft_IIC_Rcv_Byte(0);
		else
			*buf = Soft_IIC_Rcv_Byte(1);
		len--;
		buf++;
	}
	Soft_IIC_Stop();
	return 0;
}
