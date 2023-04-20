#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
 
#define PAGE_SIZE    8
#define XLevelL		 0x02
#define XLevelH		 0x10
#define YLevel       0xB0
#define	Brightness	 0xFF 
#define WIDTH 	     128
#define HEIGHT 	     64	

#define OLED_CMD     0	
#define OLED_DATA    1	

#define OLED_DS18B20_MASK   (1 << 0)					  						
#define OLED_MPU6050_MASK   (1 << 1)
#define OLED_MAX30102_MASK  (1 << 2)


void OLED_WR_Byte(unsigned dat,unsigned cmd);     							   		    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Init_GPIO(void);	   							   		    
void OLED_Init(void);
void OLED_Set_Pixel(unsigned char x, unsigned char y,unsigned char color);
void OLED_Display(void);
void OLED_Clear(unsigned dat);
void OLED_WR_large_Bytes(unsigned char*dat,unsigned cmd,uint16_t num);
void oled_calcu_buffer_task(void);
void oled_refresh_task(void);
#endif
