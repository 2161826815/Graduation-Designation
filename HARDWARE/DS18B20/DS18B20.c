#include "DS18B20.h"
#include "delay.h"

void DS18B20_Init(void)
{
    RCC_APB2PeriphClockCmd(DS18B20_GPIO_RCC,ENABLE);

    GPIO_InitTypeDef ds18b20_struct;
    ds18b20_struct.GPIO_Mode = GPIO_Mode_Out_PP;
    ds18b20_struct.GPIO_Pin = DS18B20_Pin;
    ds18b20_struct.GPIO_Speed = DS18B20_GPIO_Speed;
    GPIO_Init(DS18B20_PORT,&ds18b20_struct);
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

uint8_t DS18B20_Start(void)
{
    uint8_t ret;

    DS18B20_Mode_Change(OUTPUT);
    DS18B20_LOW;
    delay_us(480);
    DS18B20_HIGH;
    delay_us(30);
    DS18B20_Mode_Change(INPUT);
    ret = GPIO_ReadInputData(DS18B20_PORT);
    delay_us(200);

    return ret;
}

void DS18B20_Write_Byte(uint8_t data)
{
    uint8_t i;                                           //Problem 3
    DS18B20_Mode_Change(OUTPUT);
    for(i=0;i<8;i++){

        DS18B20_LOW;
        delay_us(60);
        DS18B20_HIGH;
        delay_us(2);

        DS18B20_LOW;
        delay_us(2);
        (data&0x01) ? DS18B20_HIGH : DS18B20_LOW;
        delay_us(60);
        DS18B20_HIGH;
        data >>= 1;
    }
    
}

uint8_t DS18B20_Read_Byte(void)
{
    uint8_t i,ret = 0;

    for(i=0;i<8;i++){                                   //Problem 3
        ret >>= 1;

        DS18B20_Mode_Change(OUTPUT);
        DS18B20_LOW;
        delay_us(2);

        DS18B20_Mode_Change(INPUT);
        delay_us(60);

        DS18B20_Mode_Change(OUTPUT);
        DS18B20_HIGH;
        delay_us(2);

        DS18B20_LOW;
        delay_us(2);
        DS18B20_Mode_Change(INPUT);

        if(GPIO_ReadInputData(DS18B20_PORT))
            ret |= 0x80;
        delay_us(45);
    }

    return ret;
}

void DS18B20_Read_Temp(uint16_t* data)
{
    uint8_t LOW=0,HIGH=0;
    uint16_t temp = 0;

    DS18B20_Start();
    DS18B20_Write_Byte(0xcc);               //Skip Rom
    DS18B20_Write_Byte(0x44); 
    delay_ms(750);                          //默认分辨率12bit

    DS18B20_Start();
    DS18B20_Write_Byte(0xcc);               //read data
    DS18B20_Write_Byte(0xbe); 

    LOW = DS18B20_Read_Byte();
    HIGH = DS18B20_Read_Byte();
    temp = (HIGH << 8) | LOW;
                                            //Problem 4
    if((temp &= 0xf800) == 0xf800)          //负数
        *data = (~temp + 0x01)*-0.0625;
    else                                    //正数
        *data = temp*0.0625;
    
}
