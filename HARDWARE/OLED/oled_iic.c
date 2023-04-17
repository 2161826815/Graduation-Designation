#include "oled_iic.h"
#include "i2c.h"


void Write_IIC_Command(uint8_t IIC_Command)
{
	I2C_write_OneByte(I2C2,IIC_SLAVE_ADDR,0x00,IIC_Command,1);
}

void Write_IIC_Data(uint8_t IIC_Data)
{
	I2C_write_OneByte(I2C2,IIC_SLAVE_ADDR,0x40,IIC_Data,1);
}
