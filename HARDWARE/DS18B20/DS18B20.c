#include "DS18B20.h"
#include "SysTick.h"
#include "Task_List.h"
#include "init.h"

float last_temperature = 0,cur_temperature = 0;                  //温度值
Task_t m_ds18b20_task;

uint8_t DS18B20_Init(void)
{
    RCC_APB2PeriphClockCmd(DS18B20_GPIO_RCC,ENABLE);

    GPIO_InitTypeDef ds18b20_struct;
    ds18b20_struct.GPIO_Mode = GPIO_Mode_Out_PP;
    ds18b20_struct.GPIO_Pin = DS18B20_Pin;
    ds18b20_struct.GPIO_Speed = DS18B20_GPIO_Speed;
    GPIO_Init(DS18B20_PORT,&ds18b20_struct);

    DS18B20_RST();
    return DS18B20_Check();
}

void DS18B20_Mode_Change(uint8_t mode_cmd)
{
    GPIO_InitTypeDef ds18b20_struct;

    ds18b20_struct.GPIO_Pin = DS18B20_Pin;
    ds18b20_struct.GPIO_Speed = DS18B20_GPIO_Speed;

    if(mode_cmd == OUTPUT){
        ds18b20_struct.GPIO_Mode = DS18B20_GPIO_Output_Mode;
    }
    else if(mode_cmd == INPUT){
        ds18b20_struct.GPIO_Mode = DS18B20_GPIO_Input_Mode; //Problem 2
    }
    GPIO_Init(DS18B20_PORT,&ds18b20_struct);
}

void DS18B20_RST(void)
{
    DS18B20_Mode_Change(OUTPUT);
    DS18B20_LOW;
    delay_us(750);
    DS18B20_HIGH;
    delay_us(30);
}



uint8_t DS18B20_Check(void) 
{   
	uint8_t retry=0;
	DS18B20_Mode_Change(INPUT);	 
	
	while (DS18B20_IN_Data&&retry<200){
		retry++;
		delay_us(1);
	};
	
	if(retry>=200)
		return 1;
	else 
		retry=0;
	
	while (!DS18B20_IN_Data&&retry<240){
		retry++;
		delay_us(1);
	};
	if(retry>=240)
		return 1;	 
	
	return 0;
}

uint8_t DS18B20_Read_Bit(void)
{
	uint8_t data;
	DS18B20_Mode_Change(OUTPUT);
	DS18B20_LOW; 
	delay_us(1);
	DS18B20_HIGH; 
	DS18B20_Mode_Change(INPUT);
	delay_us(14);
	
	if(DS18B20_IN_Data){
        data=1;
    }else{
        data=0;	
    }
	delay_us(50);  
	
	return data;
}


uint8_t DS18B20_Read_Byte(void)
{        
	uint8_t i,j,dat;
	dat=0;
	for (i=1;i<=8;i++) {
		j=DS18B20_Read_Bit();
		dat=(j<<7)|(dat>>1);
	}						    
	return dat;
}
 
void DS18B20_Write_Byte(uint8_t dat)     
{             
	uint8_t j;
	uint8_t testb;
	DS18B20_Mode_Change(OUTPUT);
	for (j=1;j<=8;j++) 
	{
		testb=dat&0x01;
		dat=dat>>1;
		if (testb) {
			DS18B20_LOW;
			delay_us(1);                            
			DS18B20_HIGH;
			delay_us(60);             
		}else {
			DS18B20_LOW;
			delay_us(60);             
			DS18B20_HIGH;
			delay_us(1);                          
		}
	}
}

void DS18B20_Start(void)
{   						               
	DS18B20_RST();	   
    if(!DS18B20_Check()){

    }else{
        return;
    }
} 

float DS18B20_Read_Temp(void)
{
    uint8_t LOW=0,HIGH=0;
    uint16_t temp = 0;
    DS18B20_Start();
    DS18B20_Write_Byte(0xcc);
    DS18B20_Write_Byte(0x44);               //convert
    delay_ms(750);
        
    DS18B20_Start();
    DS18B20_Write_Byte(0xcc);
    DS18B20_Write_Byte(0xbe);               //read

    LOW = DS18B20_Read_Byte();
    HIGH = DS18B20_Read_Byte();
    temp = (HIGH << 8) + LOW;
                                            //Problem 4
    if((temp & 0xf800) == 0xf800)          //负数
        return (~temp + 0x01)*-0.0625;
    else                                    //正数
        return temp*0.0625;
    
}

void ds18b20_task(void)
{
    cur_temperature = DS18B20_Read_Temp();      //采集温度
    printf("Temp:%.2f\r\n",cur_temperature);
    if(cur_temperature != last_temperature){
      last_temperature = cur_temperature;
    }
    if(cur_temperature > 38){
      BEEP_ON();
    }

}

void ds18b20_task_init(void)
{
    m_ds18b20_task.Period = DS18B20_Period;
    m_ds18b20_task.remain = 0;
    m_ds18b20_task.task = &ds18b20_task;
}
