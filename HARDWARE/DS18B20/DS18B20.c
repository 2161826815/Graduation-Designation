#include "DS18B20.h"
#include "delay.h"
#include "USART.h"
//0:success 1:fail
uint8_t DS18B20_Init(void)
{
    RCC_APB2PeriphClockCmd(DS18B20_GPIO_RCC,ENABLE);

    GPIO_InitTypeDef ds18b20_struct;
    ds18b20_struct.GPIO_Mode = GPIO_Mode_Out_PP;
    ds18b20_struct.GPIO_Pin = DS18B20_Pin;
    ds18b20_struct.GPIO_Speed = DS18B20_GPIO_Speed;
    GPIO_Init(DS18B20_PORT,&ds18b20_struct);

    GPIO_SetBits(DS18B20_PORT,DS18B20_Pin);
    DS18B20_Rst();
    return  DS18B20_Check();
}

void DS18B20_Mode_Change(uint8_t mode_cmd)
{
    GPIO_InitTypeDef ds18b20_struct;

    ds18b20_struct.GPIO_Pin = DS18B20_Pin;
    ds18b20_struct.GPIO_Speed = DS18B20_GPIO_Speed;

    if(OUTPUT)
        ds18b20_struct.GPIO_Mode = DS18B20_GPIO_Output_Mode;
    else
        ds18b20_struct.GPIO_Mode = DS18B20_GPIO_Input_Mode; //Problem 2

    GPIO_Init(DS18B20_PORT,&ds18b20_struct);
}

void DS18B20_Rst(void)
{
    DS18B20_Mode_Change(OUTPUT);
    DS18B20_LOW;
    delay_us(750);
    DS18B20_HIGH;
    delay_us(15);
}

//0:success 1:fail
uint8_t DS18B20_Check(void)
{
    uint16_t retry = 0;
    DS18B20_Mode_Change(INPUT);
    while(DS18B20_IN_STATUS && retry<200){
        retry++;
        delay_ms(1);
        printf("IN:%d\r\n",DS18B20_IN_STATUS);
    }
    if(retry>=200){
        return 1;
    }else{
        retry = 0;
    }
    while(!DS18B20_IN_STATUS&&retry<240){
        retry++;
        delay_us(1);
    }
    if(retry>=240){
        return 1;
    }
    return 0;
}

uint8_t DS18B20_Read_Bit(void)
{
    uint8_t data;
    DS18B20_Mode_Change(OUTPUT);
    DS18B20_LOW;
    delay_us(2);
    DS18B20_HIGH;
    DS18B20_Mode_Change(INPUT);
    delay_us(12);
    if(DS18B20_IN_STATUS){
        data = 1;
    }else{
        data = 0;
    }
    delay_us(50);
    return data;
}

uint8_t DS18B20_Read_Byte(void)
{
    uint8_t i,j,byte = 0;

    for(i=1;i<=8;i++){
        j = DS18B20_Read_Bit();
        byte = (j<<7) | (byte >> 1);
    }

    return byte;
}

void DS18B20_Write_Byte(uint8_t data)
{
    uint8_t i,temp;                                           //Problem 3
    DS18B20_Mode_Change(OUTPUT);
    for(i=1;i<=8;i++){
        temp = data & 0x01;
        temp = temp >> 1;
        if(temp){
            DS18B20_LOW;
            delay_us(2);
            DS18B20_HIGH;
            delay_us(60);
        }else{
            DS18B20_LOW;
            delay_us(60);
            DS18B20_HIGH;
            delay_us(2);
        }
    }
}

void DS18B20_Start(void)
{
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);   //skip rom
    DS18B20_Write_Byte(0x44);   //convert
}

short DS18B20_Get_Temp(void)
{
    uint8_t LOW=0,HIGH=0,temp;
    short value = 0;

    DS18B20_Start();
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);   //skip rom
    DS18B20_Write_Byte(0x44);   //convert
    LOW = DS18B20_Read_Byte();
    HIGH = DS18B20_Read_Byte();
    if(HIGH > 7){
        HIGH = ~HIGH;
        LOW = ~LOW;
        temp = 0;
    }else{
        temp = 1;
    }
    value = HIGH;
    value <<= 8;
    value += LOW;
    value = (float)value * 0.625;
    if(temp){
        return value;
    }else{
        return -value;
    }
        
}
