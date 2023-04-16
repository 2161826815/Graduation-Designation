#include "oled_iic.h"
#include "soft_iic.h"
#include "i2c.h"
#include "LED.h"
void IIC_Start(void)
{
#if USE_MY_SOFT_I2C
	Soft_IIC_Start();
#else
	OLED_SDA_SET();
	OLED_SCL_SET();
	OLED_IIC_Delay();
	OLED_SDA_CLR();
	OLED_IIC_Delay();
	OLED_SCL_CLR();
#endif
}

void IIC_Stop(void)
{
#if USE_MY_SOFT_I2C
	Soft_IIC_Stop();
#else	
	OLED_SCL_CLR();
	OLED_SDA_CLR();
	OLED_IIC_Delay();
	OLED_SCL_SET();
	OLED_SDA_SET();
	OLED_IIC_Delay();
#endif
}


uint8_t OLED_IIC_Wait_Ack(void)
{
#if USE_MY_SOFT_I2C
	Soft_IIC_Wait_Ack();
	return 0;
#else
	uint8_t ucErrTime = 0;
	OLED_SDA_SET();
	OLED_IIC_Delay();
	OLED_SCL_SET();
	OLED_IIC_Delay();
	while(OLED_READ_SDA){
		ucErrTime++;
		if (ucErrTime > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	OLED_SCL_CLR();
	return 0;
#endif
}

void OLED_IIC_Ack(void)
{
	OLED_SCL_CLR();
	OLED_SDA_CLR();
	OLED_IIC_Delay();
	OLED_SCL_SET();
	OLED_IIC_Delay();
	OLED_SCL_CLR();
}

void OLED_IIC_NAck(void)
{
	OLED_SCL_CLR();
	OLED_SDA_SET();
	OLED_IIC_Delay();
	OLED_SCL_SET();
	OLED_IIC_Delay();
	OLED_SCL_CLR();
}

void Write_IIC_Byte(uint8_t IIC_Byte)
{
#if USE_MY_SOFT_I2C
	Soft_IIC_Send_Byte(IIC_Byte);

#else
	uint8_t i;
	uint8_t m,da;
	da=IIC_Byte;
	OLED_SCL_CLR();
	for(i=0;i<8;i++){
		m=da;
		m=m&0x80;
		if(m==0x80){
			OLED_SDA_SET();
		}
		else{
			OLED_SDA_CLR();
		}
		da=da<<1;
		OLED_SCL_SET();
		OLED_IIC_Delay();
		OLED_SCL_CLR();
		OLED_IIC_Delay();
	}
#endif
}

uint8_t Write_IIC_Command(uint8_t IIC_Command)
{
#if USE_MY_SOFT_I2C
	return Soft_IIC_Write_One_Byte(IIC_SLAVE_ADDR, 0x00, IIC_Command);
#elif USE_MY_HARD_I2C
	I2C_write_OneByte(I2C2,IIC_SLAVE_ADDR,0x00,IIC_Command,1);
	return 0;
#else
	IIC_Start();
	//Write_IIC_Byte(IIC_SLAVE_ADDR);
	Write_IIC_Byte(IIC_SLAVE_ADDR & 0xfffe);            //Slave address,SA0=0
	if(OLED_IIC_Wait_Ack()){
		IIC_Stop();
		return 1;
	}
	LED_ON(1);
	Write_IIC_Byte(0x00);			//write command
	OLED_IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Command); 
	if(OLED_IIC_Wait_Ack()){
		IIC_Stop();
		return 1;
	}		
	IIC_Stop();
	return 0;
#endif
}

uint8_t Write_IIC_Data(uint8_t IIC_Data)
{
#if USE_MY_SOFT_I2C
	return Soft_IIC_Write_One_Byte(IIC_SLAVE_ADDR, 0x40, IIC_Data);

#elif USE_MY_HARD_I2C
	I2C_write_OneByte(I2C2,IIC_SLAVE_ADDR,0x40,IIC_Data,1);
	return 0;
#else
	IIC_Start();
	//Write_IIC_Byte(IIC_SLAVE_ADDR);
	Write_IIC_Byte(IIC_SLAVE_ADDR & 0xfffe);			//D/C#=0; R/W#=0
	if(OLED_IIC_Wait_Ack()){
		IIC_Stop();
		return 1;
	}		
	Write_IIC_Byte(0x40);			//write data
	OLED_IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Data);
	if(OLED_IIC_Wait_Ack()){
		IIC_Stop();
		return 1;
	}		
	IIC_Stop();
	return 0;
#endif
}
