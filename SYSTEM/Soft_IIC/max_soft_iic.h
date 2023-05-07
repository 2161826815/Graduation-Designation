#ifndef __MAX_IIC_H
#define __MAX_IIC_H
#include "sys.h"

//IO操作函数
#define Max_Soft_IIC_Port GPIOB
#define Max_Soft_IIC_SCL GPIO_Pin_6
#define Max_Soft_IIC_SDA GPIO_Pin_7

#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(4 << 28);}	
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(3 << 28);}

#define I2C_WR	0
#define I2C_RD	1

#define Max_IIC_SCL    PBout(6) 		//SCL
#define Max_IIC_SDA    PBout(7) 		//SDA	 
#define Max_READ_SDA   PBin(7) 		    //输入SDA 

//IIC所有操作函数
void Max_Soft_IIC_Init(void);               //初始化IIC的IO口				 
void Max_Soft_IIC_Start(void);				//发送IIC开始信号
void Max_Soft_IIC_Stop(void);	  			//发送IIC停止信号
void Max_Soft_IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t Max_Soft_IIC_Rcv_Byte(unsigned char ack);   //IIC读取一个字节
uint8_t Max_Soft_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void Max_Soft_IIC_Ack(void);					    //IIC发送ACK信号
void Max_Soft_IIC_NAck(void);				        //IIC不发送ACK信号
void Max_Soft_IIC_Write_One_Byte(uint8_t addr, uint8_t reg,uint8_t data);
void Max_Soft_IIC_Read_One_Byte(uint8_t addr, uint8_t reg, uint8_t *data);
void Max_Soft_IIC_Write_Bytes(uint8_t addr,uint8_t *buf ,uint8_t len);
void Max_Soft_IIC_Read_Bytes(uint8_t addr, uint8_t reg, uint8_t *buf , uint8_t len);


#endif
