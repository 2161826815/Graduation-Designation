#ifndef __I2C_H

#define __I2C_H

#include "stm32f10x.h"
#include "USART.h"
#define I2C_GPIO_Port 			    GPIOB
#define I2C_GPIO_CLK                RCC_APB2Periph_GPIOB
#define I2C_Speed                   400000
#define I2C_OwnDress                0x5f
#define MAX30102_I2C                I2C1
#define MPU6050_I2C                    I2C2


#define MAX30102_SCL 				GPIO_Pin_6
#define MAX30102_SDA 				GPIO_Pin_7
#define MAX30102_I2C_CLK            RCC_APB1Periph_I2C1

#define MPU6050_SCL 					GPIO_Pin_10
#define MPU6050_SDA 					GPIO_Pin_11
#define MPU6050_I2C_CLK                RCC_APB1Periph_I2C2

void I2C_Config(void);
void I2C_write_Bytes(I2C_TypeDef* I2Cx,uint8_t slave_addr,uint8_t reg_addr,const uint8_t *data,uint8_t num);
void I2C_write_OneByte(I2C_TypeDef* I2Cx,uint8_t slave_addr,uint8_t reg_addr,uint8_t data,uint8_t num);
void I2C_read_Bytes(I2C_TypeDef* I2Cx,uint8_t slave_addr,uint8_t reg_addr,uint8_t *data,uint8_t num);
void I2C_Wait(I2C_TypeDef* I2Cx,uint8_t slave_addr);

#endif
