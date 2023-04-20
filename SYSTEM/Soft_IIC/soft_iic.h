#ifndef __MPUIIC_H
#define __MPUIIC_H
#include "sys.h"

//IO操作函数
#define Sofr_IIC_Port GPIOB
#define Soft_IIC_SCL GPIO_Pin_3
#define Soft_IIC_SDA GPIO_Pin_4

#define IIC_SCL    PBout(3) 		//SCL
#define IIC_SDA    PBout(4) 		//SDA	 
#define READ_SDA   PBin(4) 		//输入SDA 

//IIC所有操作函数
void IIC_Delay(void);				//IIC延时2ms函数
void Soft_IIC_Init(void);                //初始化IIC的IO口				 
void Soft_IIC_Start(void);				//发送IIC开始信号
void Soft_IIC_Stop(void);	  			//发送IIC停止信号
void Soft_IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t Soft_IIC_Rcv_Byte(unsigned char ack);//IIC读取一个字节
uint8_t Soft_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void Soft_IIC_Ack(void);					//IIC发送ACK信号
void Soft_IIC_NAck(void);				//IIC不发送ACK信号
uint8_t Soft_IIC_Write_One_Byte(uint8_t addr, uint8_t reg,uint8_t data);
uint8_t Soft_IIC_Write_Bytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
uint8_t Soft_IIC_Read_Bytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);


#endif
