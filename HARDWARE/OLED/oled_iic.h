#ifndef _IIC_H_
#define _IIC_H_
#include "sys.h"

#define IIC_SLAVE_ADDR 0x78

#define USE_MY_SOFT_I2C 0
#define USE_MY_HARD_I2C 1

#define OLED_SDA       GPIO_Pin_3
#define OLED_SCL       GPIO_Pin_4


#define	OLED_SDA_SET()  	GPIO_SetBits(GPIOB,OLED_SDA) 
#define	OLED_SDA_CLR()  	GPIO_ResetBits(GPIOB,OLED_SDA) 
  
#define	OLED_SCL_SET()  	GPIO_SetBits(GPIOB,OLED_SCL)    
#define	OLED_SCL_CLR()  	GPIO_ResetBits(GPIOB,OLED_SCL)    


void IIC_Start(void);
void IIC_Stop(void);
void IIC_Wait_Ack(void);
void Write_IIC_Byte(u8 IIC_Byte);
void Write_IIC_Command(u8 IIC_Command);
void Write_IIC_Data(u8 IIC_Data);

#endif
