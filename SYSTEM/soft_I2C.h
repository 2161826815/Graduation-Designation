#ifndef __soft_I2C_H
#define __soft_I2C_H

#include "stm32f10x.h"
#include "delay.h"

#define soft_i2c_port       GPIOB
#define soft_i2c_sda        GPIO_Pin_3
#define soft_i2c_scl        GPIO_Pin_4  

typedef enum {
    INPUT = 0,
    OPUPUT
}io_mode_t;

#define I2C_Delay()         delay_us(1)

#define SDA_1()             GPIO_SetBits(soft_i2c_port,soft_i2c_sda)
#define SDA_0()             GPIO_ResetBits(soft_i2c_port,soft_i2c_sda)

#define SCL_1()             GPIO_SetBits(soft_i2c_port,soft_i2c_scl)
#define SCL_0()             GPIO_ResetBits(soft_i2c_port,soft_i2c_scl)

#define SDA_READ()          GPIO_ReadInputDataBit(soft_i2c_port,soft_i2c_sda)
#define SCL_READ()          GPIO_ReadInputDataBit(soft_i2c_port,soft_i2c_scl)

void soft_i2c_config(void);
void soft_i2c_start(void);
void soft_i2c_stop(void);
uint8_t soft_i2c_read_byte(void);
void soft_i2c_send_byte(uint8_t value);
void soft_i2c_ack(void);
void soft_i2c_nack(void);
uint8_t soft_i2c_wait_ack(void);
void soft_i2c_read_data(I2C_TypeDef* I2Cx,uint8_t addr,uint8_t position,uint8_t *data,uint8_t num);
void soft_i2c_write_byte(I2C_TypeDef* I2Cx,uint8_t addr,uint8_t position,uint8_t byte,uint8_t num);
void soft_i2c_write_bytes(I2C_TypeDef* I2Cx,uint8_t addr,uint8_t position,uint8_t const *array,uint8_t num);
void soft_i2c_io_change(io_mode_t io_mode);
#endif
