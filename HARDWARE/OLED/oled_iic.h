#ifndef _IIC_H_
#define _IIC_H_
#include "sys.h"
#include "SysTick.h"
#define IIC_SLAVE_ADDR 0x78

#define USE_MY_SOFT_I2C 0
#define USE_MY_HARD_I2C 1

#define OLED_IIC_Delay()    delay_us(2)

#define OLED_SDA        GPIO_Pin_3
#define OLED_SCL        GPIO_Pin_4
#define OLED_READ_SDA        PBin(3)

#define	OLED_SDA_SET()  	GPIO_SetBits(GPIOB,OLED_SDA) 
#define	OLED_SDA_CLR()  	GPIO_ResetBits(GPIOB,OLED_SDA) 
  
#define	OLED_SCL_SET()  	GPIO_SetBits(GPIOB,OLED_SCL)    
#define	OLED_SCL_CLR()  	GPIO_ResetBits(GPIOB,OLED_SCL)    


void IIC_Start(void);
void IIC_Stop(void);
uint8_t OLED_IIC_Wait_Ack(void);
void Write_IIC_Byte(uint8_t IIC_Byte);
uint8_t Write_IIC_Command(uint8_t IIC_Command);
uint8_t Write_IIC_Data(uint8_t IIC_Data);

#endif
