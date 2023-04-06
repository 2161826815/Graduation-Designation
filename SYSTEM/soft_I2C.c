#include "soft_I2C.h"
#include "USART.h"

void soft_i2c_config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStruct.GPIO_Pin = soft_i2c_scl;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(soft_i2c_port,&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = soft_i2c_sda;
    GPIO_Init(soft_i2c_port,&GPIO_InitStruct);
    soft_i2c_stop();
}

void soft_i2c_io_change(io_mode_t io_mode)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = soft_i2c_sda;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    if(io_mode == INPUT){
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    }else if(io_mode == OPUPUT){
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    }
    GPIO_Init(soft_i2c_port,&GPIO_InitStruct);
}

void soft_i2c_start(void)
{
    SDA_1();
    SCL_1();
    I2C_Delay();
    SDA_0();
    I2C_Delay();
    SCL_0();
    I2C_Delay();
    
}

void soft_i2c_stop(void)
{
    SDA_0();
    SCL_1();
    I2C_Delay();
    SDA_1();
    I2C_Delay();
}

void soft_i2c_ack(void)
{
    SCL_0();
    SDA_1();
    I2C_Delay();
    SCL_1();
    I2C_Delay();
    SCL_0();
}

void soft_i2c_nack(void)
{
    SDA_1();
    I2C_Delay();
    SCL_1();
    I2C_Delay();
    SCL_0();
    I2C_Delay();
}

uint8_t soft_i2c_wait_ack()
{
    uint8_t timeout = 0;
    SDA_1();
    I2C_Delay();
    SCL_1();
    I2C_Delay();
    soft_i2c_io_change(INPUT);
    while(SDA_READ()){
        timeout++;
        if(timeout == 250){
            soft_i2c_io_change(OPUPUT);
            soft_i2c_stop();
            return 1;
        }
    }
    SCL_0();
    return 0;

}

uint8_t soft_i2c_read_byte(void)
{
    uint8_t i=0,value=0;
    for(i=0;i<8;i++){
        value <<= 1;
        SCL_1();
        I2C_Delay();
        soft_i2c_io_change(INPUT);
        value |= SDA_READ();
        soft_i2c_io_change(OPUPUT);
        SCL_0();
        I2C_Delay();
    }
    return value;
}

void soft_i2c_send_byte(uint8_t value)
{
    uint8_t i=0;
    for(i=0;i<8;i++){
        if(value & 0x80)
            SDA_1();
        else
            SDA_0();
        I2C_Delay();
        SCL_1();
        I2C_Delay();
        SCL_0();

        if(i==7){
            SDA_1();
            I2C_Delay();
        }
        value = value<<1;
    }
}


void soft_i2c_read_data(I2C_TypeDef* I2Cx,uint8_t addr,uint8_t position,uint8_t *data,uint8_t num)
{
    uint8_t i=0;
    soft_i2c_start();
    soft_i2c_send_byte((addr<<1) | 0x00); //send addr
    while(soft_i2c_wait_ack());
    soft_i2c_send_byte(position);
    while(soft_i2c_wait_ack()); //send position
    
    soft_i2c_start();
    soft_i2c_send_byte((addr<<1) | 0x01); //send read signal
    while(soft_i2c_wait_ack());

    for(i=0;i<num;i++){
        data[i] = soft_i2c_read_byte();
        if(i == (num-1))
            soft_i2c_nack();
        else
            soft_i2c_ack();      
    }
    soft_i2c_stop();
}

void soft_i2c_write_byte(I2C_TypeDef* I2Cx,uint8_t addr,uint8_t position,uint8_t byte,uint8_t num)
{
    soft_i2c_start();
    soft_i2c_send_byte((addr<<1) | 0x00); //send addr
    while(soft_i2c_wait_ack());
    soft_i2c_send_byte(position);
    while(soft_i2c_wait_ack()); //send position
    soft_i2c_send_byte(byte);
    while(soft_i2c_wait_ack()); 
    
    soft_i2c_stop();
}

void soft_i2c_write_bytes(I2C_TypeDef* I2Cx,uint8_t addr,uint8_t position,uint8_t const *array,uint8_t num)
{
    uint8_t i=0;
    soft_i2c_start();
    soft_i2c_send_byte((addr<<1) | 0x00); //send addr
    while(soft_i2c_wait_ack());
    soft_i2c_send_byte(position);
    while(soft_i2c_wait_ack()); //send position
    for(i=0;i<num;i++){
       soft_i2c_send_byte(array[i]);
       while(soft_i2c_wait_ack()); 
    }
    soft_i2c_stop();
}
