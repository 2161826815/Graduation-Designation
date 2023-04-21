#include "oled.h"
#include "stdlib.h"
#include "string.h" 	 
#include "SysTick.h"
#include "oled_iic.h"
#include "soft_iic.h"
#include "i2c.h"
#include "init.h"

Task_t m_oled_calcu_task;
Task_t m_oled_refresh_task;

struct oled_buffer
{
	uint8_t temp_int;
	uint8_t temp_float;
	uint8_t pitch_int;
	uint8_t pitch_float;
	uint8_t roll_int;
	uint8_t roll_float;
	uint8_t yaw_int;
	uint8_t yaw_float;
	int32_t HR;
	int32_t SPO2;
}oled_data;


static unsigned char OLED_buffer[8][128];


void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd){
		I2C_write_OneByte(I2C2,IIC_SLAVE_ADDR,0x40,dat,1);
	}
	else{
		I2C_write_OneByte(I2C2,IIC_SLAVE_ADDR,0x00,dat,1);
	}
}

void OLED_WR_Bytes(unsigned char*dat,unsigned cmd,uint8_t num)
{
	if(cmd){
		I2C_write_Bytes(I2C2,IIC_SLAVE_ADDR,0x40,dat,num);
	}
	else{
		I2C_write_Bytes(I2C2,IIC_SLAVE_ADDR,0x00,dat,num);	
	}
}

void OLED_WR_large_Bytes(unsigned char*dat,unsigned cmd,uint16_t num)
{
	if(cmd){
		I2C_write_large_Bytes(I2C2,IIC_SLAVE_ADDR,0x40,dat,num);
	}
	else{
		I2C_write_large_Bytes(I2C2,IIC_SLAVE_ADDR,0x00,dat,num);	
	}
}

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{
 	OLED_WR_Byte(YLevel+y/PAGE_SIZE,OLED_CMD);
	OLED_WR_Byte((((x+2)&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte(((x+2)&0x0f),OLED_CMD); 
}  
	  
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  
	OLED_WR_Byte(0X14,OLED_CMD);  
	OLED_WR_Byte(0XAF,OLED_CMD);  
}

   
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  
	OLED_WR_Byte(0X10,OLED_CMD);  
	OLED_WR_Byte(0XAE,OLED_CMD);  
}


void OLED_Set_Pixel(unsigned char x, unsigned char y,unsigned char color)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=y/8;
	bx=y%8;
	temp=1<<bx;
	if(color)OLED_buffer[pos][x]|=temp;
	else OLED_buffer[pos][x]&=~temp;

}		   			 

uint8_t level[8][3] = {
	{0xb0,0x02,0x10},
	{0xb1,0x02,0x10},
	{0xb2,0x02,0x10},
	{0xb3,0x02,0x10},
	{0xb4,0x02,0x10},
	{0xb5,0x02,0x10},
	{0xb6,0x02,0x10},
	{0xb7,0x02,0x10},
};
void OLED_Display(void)
{
	uint8_t i;
	//uint8_t level[8][3];
	for(i=0;i<PAGE_SIZE;i++){
 	//	level[i][0] = YLevel+i;
	//	level[i][1] = XLevelL;
	//	level[i][2] = XLevelH; */
		OLED_WR_Bytes(&level[i][0],OLED_CMD,3);
		OLED_WR_Bytes(OLED_buffer[i],OLED_DATA,WIDTH);
	}
	//OLED_WR_large_Bytes(&OLED_buffer[0][0],OLED_DATA,WIDTH*PAGE_SIZE);

	/* for(i=0;i<PAGE_SIZE;i++)  {  
		OLED_WR_Byte (YLevel+i,OLED_CMD);    
		OLED_WR_Byte (XLevelL,OLED_CMD);     
		OLED_WR_Byte (XLevelH,OLED_CMD);     
		for(n=0;n<WIDTH;n++){
			OLED_WR_Byte(OLED_buffer[i*WIDTH+n],OLED_DATA); 
		} 
	} */
}


void OLED_Clear(unsigned dat)  
{  
	if(dat){
		memset(OLED_buffer,0xff,sizeof(OLED_buffer));
	}
	else{
		memset(OLED_buffer,0,sizeof(OLED_buffer));
	}
	OLED_Display();
}


void OLED_Init_GPIO(void)
{
	I2C_Config();
}


void OLED_Init(void)
{
	/* uint8_t	OLED_INIT_DATA[28] = {
		0xae,0x20,0x00,0x02,0x10,0x40,0xb0,0x81,0xff,0xa1,0xa6,0xa8,0x3f,0xad,0x8b,0x32,0xc8,
		0xd3,0x00,0xd5,0x80,0xd9,0x1f,0xda,0x12,0xdb,0x40,0xaf
	}; */
 	OLED_Init_GPIO();
	//OLED_WR_Bytes(OLED_INIT_DATA,OLED_CMD,28);
	OLED_WR_Byte(0x20,OLED_CMD);
	OLED_WR_Byte(0x00,OLED_CMD);

	OLED_WR_Byte(0xAE,OLED_CMD);    /*display off*/
	OLED_WR_Byte(0x01,OLED_CMD);    /*set lower column address*/       
	OLED_WR_Byte(0x10,OLED_CMD);    /*set higher column address*/     
	OLED_WR_Byte(0x40,OLED_CMD);    /*set display start line*/     
	OLED_WR_Byte(0xB0,OLED_CMD);    /*set page address*/     
	OLED_WR_Byte(0x81,OLED_CMD);    /*contract control*/
	OLED_WR_Byte(0xFF,OLED_CMD);    /*128*/     
	OLED_WR_Byte(0xA1,OLED_CMD);    /*set segment remap*/    
	OLED_WR_Byte(0xA6,OLED_CMD);    /*normal / reverse*/     
	OLED_WR_Byte(0xA8,OLED_CMD);    /*multiplex ratio*/
	OLED_WR_Byte(0x3F,OLED_CMD);    /*duty = 1/64*/     
	OLED_WR_Byte(0xAD,OLED_CMD);    /*set charge pump enable*/
	OLED_WR_Byte(0x8B,OLED_CMD);     /*    0x8B    �ڹ�VCC   */     
	OLED_WR_Byte(0x32,OLED_CMD);    /*0X30---0X33  set VPP   8V */     
	OLED_WR_Byte(0xC8,OLED_CMD);    /*Com scan direction*/     
	OLED_WR_Byte(0xD3,OLED_CMD);    /*set display offset*/
	OLED_WR_Byte(0x00,OLED_CMD);   /*   0x20  */     
	OLED_WR_Byte(0xD5,OLED_CMD);    /*set osc division*/
	OLED_WR_Byte(0x80,OLED_CMD);         
	OLED_WR_Byte(0xD9,OLED_CMD);    /*set pre-charge period*/
	OLED_WR_Byte(0x1F,OLED_CMD);    /*0x22*/     
	OLED_WR_Byte(0xDA,OLED_CMD);    /*set COM pins*/
	OLED_WR_Byte(0x12,OLED_CMD);     
	OLED_WR_Byte(0xDB,OLED_CMD);    /*set vcomh*/
	OLED_WR_Byte(0x40,OLED_CMD);                
	OLED_WR_Byte(0xAF,OLED_CMD);    /*display ON*/    
}

extern data_buff_t all_data;
void oled_refresh_task(void)
{
#if DS18B20_ON_OFF
	if(m_oled_refresh_task.pri_data & OLED_DS18B20_MASK){
		if(all_data.temperature < 0)
			GUI_ShowChar(30,0,'-',8,1);
		else
			GUI_ShowChar(30,0,'+',8,1);

		if(oled_data.temp_int<=9){
			GUI_ShowNum(36,0,0,1,8,1);
			GUI_ShowNum(42,0,oled_data.temp_int,1,8,1);
		}else{
			GUI_ShowNum(36,0,oled_data.temp_int,2,8,1);
		}
		GUI_ShowChar(48,0,'.',8,1);

		if(oled_data.temp_float<=9){
			GUI_ShowNum(54,0,0,1,8,1);
			GUI_ShowNum(60,0,oled_data.temp_float,1,8,1);
		}else{
			GUI_ShowNum(54,0,oled_data.temp_float,2,8,1);
		}
		m_oled_refresh_task.pri_data &= ~OLED_DS18B20_MASK;
	}
#endif

#if MPU6050_ON_OFF
		//显示pitch
	if(m_oled_refresh_task.pri_data & OLED_MPU6050_MASK){
		if(all_data.pitch < 0)
			GUI_ShowChar(36,10,'-',8,1);
		else
			GUI_ShowChar(36,10,'+',8,1);

		if(oled_data.pitch_int<=9){
			GUI_ShowNum(42,10,0,1,8,1);
			GUI_ShowNum(48,10,oled_data.pitch_int,1,8,1);
		}else{
			GUI_ShowNum(42,10,oled_data.pitch_int,2,8,1);
		}
		GUI_ShowChar(54,10,'.',8,1);
		if(oled_data.pitch_float<=9){
			GUI_ShowNum(60,10,0,1,8,1);
			GUI_ShowNum(66,10,oled_data.pitch_float,1,8,1);
		}else{
			GUI_ShowNum(60,10,oled_data.pitch_float,2,8,1);
		}
		
		//显示roll

		if(all_data.roll < 0)
			GUI_ShowChar(30,20,'-',8,1);
		else
			GUI_ShowChar(30,20,'+',8,1);

		if(oled_data.roll_int<=9){
			GUI_ShowNum(36,20,0,1,8,1);
			GUI_ShowNum(42,20,oled_data.roll_int,1,8,1);
		}else{
			GUI_ShowNum(36,20,oled_data.roll_int,2,8,1);
		}
		GUI_ShowChar(48,20,'.',8,1);
		if(oled_data.roll_float<=9){
			GUI_ShowNum(54,20,0,1,8,1);
			GUI_ShowNum(60,20,oled_data.roll_float,1,8,1);
		}else{
			GUI_ShowNum(54,20,oled_data.roll_float,2,8,1);
		}


		if(all_data.yaw < 0)
			GUI_ShowChar(24,30,'-',8,1);
		else
			GUI_ShowChar(24,30,'+',8,1);

		if(oled_data.yaw_int<=9){
			GUI_ShowNum(30,30,0,1,8,1);
			GUI_ShowNum(36,30,oled_data.yaw_int,1,8,1);
		}else{
			GUI_ShowNum(30,30,oled_data.yaw_int,2,8,1);
		}
		GUI_ShowChar(42,30,'.',8,1);
		if(oled_data.yaw_float<=9){
			GUI_ShowNum(48,30,0,1,8,1);
			GUI_ShowNum(54,30,oled_data.yaw_float,1,8,1);
		}else{
			GUI_ShowNum(48,30,oled_data.yaw_float,2,8,1);
		}
		m_oled_refresh_task.pri_data &= ~OLED_MPU6050_MASK;
	}
#endif

#if MAX30102_ON_OFF
	if(m_oled_refresh_task.pri_data & OLED_MAX30102_MASK){
		GUI_ShowNum(24,40,oled_data.HR,3,8,1);

		GUI_ShowNum(36,50,oled_data.SPO2,2,8,1);
		m_oled_refresh_task.pri_data &= ~OLED_MAX30102_MASK;
	}
#endif

	OLED_Display();
}

void oled_calcu_buffer_task(void)
{

#if DS18B20_ON_OFF
	oled_data.temp_int = (uint8_t)all_data.temperature;
	oled_data.temp_float = (uint8_t)((all_data.temperature-oled_data.temp_int)*100);
	m_oled_refresh_task.pri_data |= OLED_DS18B20_MASK;
#endif

#if MPU6050_ON_OFF
	oled_data.pitch_int = (uint8_t)all_data.pitch;
	oled_data.pitch_float = (uint8_t)((all_data.pitch-oled_data.pitch_int)*100);

	oled_data.roll_int = (uint8_t)all_data.roll;
	oled_data.roll_float = (uint8_t)((all_data.roll-oled_data.roll_int)*100);

	oled_data.yaw_int = (uint8_t)all_data.yaw;
	oled_data.yaw_float = (uint8_t)((all_data.yaw-oled_data.yaw_int)*100);

	m_oled_refresh_task.pri_data |= OLED_MPU6050_MASK;
#endif

#if MAX30102_ON_OFF
	oled_data.HR = all_data.HR;
	oled_data.SPO2 = all_data.SPO2;
	m_oled_refresh_task.pri_data |= OLED_MAX30102_MASK;
#endif
}
