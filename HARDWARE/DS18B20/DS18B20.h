#ifndef __DS18B20_H

#define __DS18B20_H

#include "stm32f10x.h"

#define DS18B20_PORT                    GPIOA
#define DS18B20_Pin                     GPIO_Pin_7
#define DS18B20_GPIO_RCC                RCC_APB2Periph_GPIOA
#define DS18B20_GPIO_Speed              GPIO_Speed_50MHz
#define DS18B20_GPIO_Output_Mode        GPIO_Mode_Out_PP
#define DS18B20_GPIO_Input_Mode         GPIO_Mode_IN_FLOATING

#define DS18B20_LOW                     (GPIO_ResetBits(DS18B20_PORT,DS18B20_Pin))
#define DS18B20_HIGH                    (GPIO_SetBits(DS18B20_PORT,DS18B20_Pin))
#define DS18B20_IN_Data                 (GPIO_ReadInputDataBit(DS18B20_PORT,DS18B20_Pin))
enum mode_cmd
{
    INPUT  = 0,
    OUTPUT
};

#endif

uint8_t DS18B20_Init(void);
void DS18B20_Mode_Change(uint8_t mode_cmd);
void DS18B20_Start(void);
void DS18B20_Write_Byte(uint8_t data);
uint8_t DS18B20_Read_Byte(void);
float DS18B20_Read_Temp(void);
void DS18B20_RST(void);
uint8_t DS18B20_Check(void);
void ds18b20_read_task_init(void);
void ds18b20_convert_task_init(void);
void DS18B20_Convert(void);
void ds18b20_read_task(void);
