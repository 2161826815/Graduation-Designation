		#include "oled_iic.h"
#include "soft_I2C.h"
#include "i2c.h"

void IIC_Start(void)
{
#if USE_MY_SOFT_I2C
	soft_i2c_start();
#else
	OLED_SCL_SET();
	OLED_SDA_SET();
	OLED_SDA_CLR();
	OLED_SCL_CLR();
#endif
}

void IIC_Stop(void)
{
#if USE_MY_SOFT_I2C
	soft_i2c_stop();
#else	
	OLED_SCL_SET();
	OLED_SDA_CLR();
	OLED_SDA_SET();
#endif
}


void IIC_Wait_Ack(void)
{
#if USE_MY_SOFT_I2C
	soft_i2c_wait_ack();
#else
	OLED_SCL_SET();
	OLED_SCL_CLR();
#endif
}


void Write_IIC_Byte(u8 IIC_Byte)
{
#if USE_MY_SOFT_I2C
	soft_i2c_send_byte(IIC_Byte);

#else
	u8 i;
	u8 m,da;
	da=IIC_Byte;
	OLED_SCL_CLR();
	for(i=0;i<8;i++)		
	{
		m=da;
		m=m&0x80;
		if(m==0x80)
		{
			OLED_SDA_SET();
		}
		else
		{
			OLED_SDA_CLR();
		}
		da=da<<1;
		OLED_SCL_SET();
		OLED_SCL_CLR();
		}
#endif
}

void Write_IIC_Command(u8 IIC_Command)
{
#if USE_MY_SOFT_I2C
	soft_i2c_write_byte(I2C2,IIC_SLAVE_ADDR,0x00,IIC_Command,1);
#elif USE_MY_HARD_I2C
	I2C_write_OneByte(I2C2,IIC_SLAVE_ADDR,0x00,IIC_Command,1);
#else
	IIC_Start();
	Write_IIC_Byte(IIC_SLAVE_ADDR);            //Slave address,SA0=0
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
	IIC_Stop();
#endif
}

void Write_IIC_Data(u8 IIC_Data)
{
#if USE_MY_SOFT_I2C
	soft_i2c_write_byte(I2C2,IIC_SLAVE_ADDR,0x40,IIC_Data,1);

#elif USE_MY_HARD_I2C
	I2C_write_OneByte(I2C2,IIC_SLAVE_ADDR,0x40,IIC_Data,1);
#else
	IIC_Start();
	Write_IIC_Byte(IIC_SLAVE_ADDR);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
	Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Ack();	
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
	IIC_Stop();
#endif
}
