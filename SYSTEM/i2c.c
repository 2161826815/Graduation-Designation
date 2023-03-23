#include "i2c.h"

void I2C_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    I2C_InitTypeDef  I2C_InitStruct;

    RCC_APB2PeriphClockCmd(I2C_GPIO_CLK,ENABLE);
    RCC_APB1PeriphClockCmd(MAX30102_I2C_CLK | MPU6050_I2C_CLK,ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStruct.GPIO_Pin = MAX30102_SCL | MAX30102_SDA | MPU6050_SCL | MPU6050_SDA;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_GPIO_Port,&GPIO_InitStruct);
    
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStruct.I2C_ClockSpeed = I2C_Speed;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_OwnAddress1 = I2C_OwnDress;
    I2C_Init(MAX30102_I2C,&I2C_InitStruct);
    I2C_Init(MPU6050_I2C,&I2C_InitStruct);

    I2C_Cmd(MAX30102_I2C,ENABLE);
    I2C_Cmd(MPU6050_I2C,ENABLE);

}

void I2C_write_OneByte(I2C_TypeDef* I2Cx,uint8_t slave_addr,uint8_t reg_addr,uint8_t data,uint8_t num)
{
    I2C_GenerateSTART(I2Cx,ENABLE);  //Start Signal
    while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_SB)); //EV5

    I2C_Send7bitAddress(I2Cx,slave_addr,I2C_Direction_Transmitter); //send slave addr
    while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_ADDR)); //EV6

    I2C_SendData(I2Cx,reg_addr); //send register addr
    while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_BTF)); //EV8 

    I2C_SendData(I2Cx,data); //send register addr
    while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_BTF)); //EV8

    I2C_GenerateSTOP(I2Cx,ENABLE);
}

void I2C_write_Bytes(I2C_TypeDef* I2Cx,uint8_t slave_addr,uint8_t reg_addr,uint8_t *data,uint8_t num)
{
    I2C_GenerateSTART(I2Cx,ENABLE);  //Start Signal
    while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_SB)); //EV5

    I2C_Send7bitAddress(I2Cx,slave_addr,I2C_Direction_Transmitter); //send slave addr
    while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_ADDR)); //EV6

    I2C_SendData(I2Cx,reg_addr); //send register addr
    while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_BTF)); //EV8 

    while(num){
        I2C_SendData(I2Cx,*data); //send register addr
        while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_BTF)); //EV8
        num--;
        data++;
    }

    I2C_GenerateSTOP(I2Cx,ENABLE);
}

void I2C_read_Bytes(I2C_TypeDef* I2Cx,uint8_t slave_addr,uint8_t reg_addr,uint8_t *data,uint8_t num)
{
    I2C_GenerateSTART(I2Cx,ENABLE);  //Start Signal
    while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_SB)); //EV5

    I2C_Send7bitAddress(I2Cx,slave_addr,I2C_Direction_Transmitter); //send addr
    while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_ADDR)); //EV6

    I2C_SendData(I2Cx,reg_addr); //send send register addr
    while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_TXE)); //EV8




    I2C_GenerateSTART(I2Cx,ENABLE);  //Second Start Signal
    while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_SB)); //EV5

    I2C_Send7bitAddress(I2Cx,slave_addr,I2C_Direction_Receiver); //Second send slave addr
    while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_ADDR)); //EV6

    while(num){

        if(num==1){
            I2C_AcknowledgeConfig(I2Cx,DISABLE);//the last data
        }

        while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_RXNE)); //EV7
        *data = I2C_ReceiveData(I2Cx);

        data++;
        num--;
    }
    

    I2C_GenerateSTOP(I2Cx,ENABLE);
    I2C_AcknowledgeConfig(I2Cx,ENABLE);
}

void I2C_Wait(I2C_TypeDef* I2Cx,uint8_t slave_addr)
{
    do{
        I2C_GenerateSTART(I2Cx,ENABLE);  //Second Start Signal
        while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_SB)); //EV5

        I2C_Send7bitAddress(I2Cx,slave_addr,I2C_Direction_Transmitter); //send addr

    }while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_ADDR));

    I2C_GenerateSTOP(I2Cx,ENABLE);
}
