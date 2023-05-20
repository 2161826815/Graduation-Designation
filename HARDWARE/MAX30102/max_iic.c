#include "max_iic.h"
#include "SysTick.h"

void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	MAX_IIC_SCL=1;
	MAX_IIC_SDA=1;

}

void IIC_Start(void)
{
	SDA_OUT();
	MAX_IIC_SDA=1;  	  
	MAX_IIC_SCL=1;
	delay_us(4);
 	MAX_IIC_SDA=0;
	delay_us(4);
	MAX_IIC_SCL=0;
}	  

void IIC_Stop(void)
{
	SDA_OUT();
	MAX_IIC_SCL=0;
	MAX_IIC_SDA=0;
 	delay_us(4);
	MAX_IIC_SCL=1; 
	MAX_IIC_SDA=1;
	delay_us(4);							   	
}

u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();
	MAX_IIC_SDA=1;delay_us(1);	   
	MAX_IIC_SCL=1;delay_us(1);	 
	while(MAX_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	MAX_IIC_SCL=0;	   
	return 0;  
} 

void IIC_Ack(void)
{
	MAX_IIC_SCL=0;
	SDA_OUT();
	MAX_IIC_SDA=0;
	delay_us(2);
	MAX_IIC_SCL=1;
	delay_us(2);
	MAX_IIC_SCL=0;
}
	    
void IIC_NAck(void)
{
	MAX_IIC_SCL=0;
	SDA_OUT();
	MAX_IIC_SDA=1;
	delay_us(2);
	MAX_IIC_SCL=1;
	delay_us(2);
	MAX_IIC_SCL=0;
}					 				     
		  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    MAX_IIC_SCL=0;
    for(t=0;t<8;t++)
    {              
        MAX_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);
		MAX_IIC_SCL=1;
		delay_us(2); 
		MAX_IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
 
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();
    for(i=0;i<8;i++ )
	{
        MAX_IIC_SCL=0; 
        delay_us(2);
		MAX_IIC_SCL=1;
        receive<<=1;
        if(MAX_READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();
    else
        IIC_Ack();
    return receive;
}


void IIC_WriteBytes(u8 WriteAddr,u8* data,u8 dataLength)
{		
	u8 i;	
    IIC_Start();  

	IIC_Send_Byte(WriteAddr);
	IIC_Wait_Ack();
	
	for(i=0;i<dataLength;i++)
	{
		IIC_Send_Byte(data[i]);
		IIC_Wait_Ack();
	}				    	   
    IIC_Stop();
	delay_ms(10);	 
}

void IIC_ReadBytes(u8 deviceAddr, u8 writeAddr,u8* data,u8 dataLength)
{		
	u8 i;	
    IIC_Start();  

	IIC_Send_Byte(deviceAddr);
	IIC_Wait_Ack();
	IIC_Send_Byte(writeAddr);
	IIC_Wait_Ack();
	IIC_Send_Byte(deviceAddr|0X01);
	IIC_Wait_Ack();
	
	for(i=0;i<dataLength-1;i++)
	{
		data[i] = IIC_Read_Byte(1);
	}		
	data[dataLength-1] = IIC_Read_Byte(0);	
    IIC_Stop();
	delay_ms(10);	 
}

void IIC_Read_One_Byte(u8 daddr,u8 addr,u8* data)
{				  	  	    																 
    IIC_Start();  
	
	IIC_Send_Byte(daddr);
	IIC_Wait_Ack();
	IIC_Send_Byte(addr);
	IIC_Wait_Ack();
	IIC_Start();   
	IIC_Send_Byte(daddr|0X01);
	IIC_Wait_Ack();
    *data = IIC_Read_Byte(0);
    IIC_Stop();
}

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data)
{				   	  	    																 
    IIC_Start();  
	
	IIC_Send_Byte(daddr);
	IIC_Wait_Ack();
	IIC_Send_Byte(addr);
	IIC_Wait_Ack();	   	 										  		   
	IIC_Send_Byte(data);
	IIC_Wait_Ack();  		    	   
    IIC_Stop();
	delay_ms(10);
}

