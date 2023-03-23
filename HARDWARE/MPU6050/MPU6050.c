#include "MPU6050.h"

uint8_t MPU6050_Write_Bytes(uint8_t slave_addr,uint8_t reg_addr,uint8_t num,uint8_t* data)
{
    I2C_write_Bytes(MPU6050_I2C,slave_addr,reg_addr,data,num);
    return 0;
}

uint8_t MPU6050_Write_One_Byte(uint8_t slave_addr,uint8_t reg_addr,uint8_t num,uint8_t data)
{
    I2C_write_OneByte(MPU6050_I2C,slave_addr,reg_addr,data,num);
    return 0;
}

uint8_t MPU6050_Read_Bytes(uint8_t slave_addr,uint8_t reg_addr,uint8_t num,uint8_t* data)
{
    I2C_read_Bytes(MPU6050_I2C,slave_addr,reg_addr,data,num);
    return 0;
}

void MPU6050_Init(void)
{
    uint8_t res;
    I2C_Config();
    MPU6050_Write_One_Byte(MPU6050_ADDR,MPU6050_PWR_MGMT1_REG,1,0x80);    //复位
    delay_ms(100);
    MPU6050_Write_One_Byte(MPU6050_ADDR,MPU6050_PWR_MGMT1_REG,1,0x00);    //唤醒
    MPU6050_Write_One_Byte(MPU6050_ADDR,MPU6050_GYRO_CFG_REG,1,(3<<3));   //角速度量程为±2000°/s
    MPU6050_Write_One_Byte(MPU6050_ADDR,MPU6050_ACCEL_CFG_REG,1,(0<<3));  //加速度量程为±2g
    MPU6050_Write_One_Byte(MPU6050_ADDR,MPU6050_INT_EN_REG,1,0X00);       //关闭所有中断
    MPU6050_Write_One_Byte(MPU6050_ADDR,MPU6050_USER_CTRL_REG,1,0X00);    //关闭主模式
    MPU6050_Write_One_Byte(MPU6050_ADDR,MPU6050_FIFO_EN_REG,1,0X00);      //关闭FIFO
    MPU6050_Write_One_Byte(MPU6050_ADDR,MPU6050_INTBP_CFG_REG,1,0X80);    //INT引脚低电平有效

    MPU6050_Read_Bytes(MPU6050_ADDR,MPU6050_DEVICE_ID_REG,1,&res);   //判断是否成功连接MPU6050
    if(res == MPU6050_ADDR){
        MPU6050_Write_One_Byte(MPU6050_ADDR,MPU6050_PWR_MGMT1_REG,1,0X01);//设置CLKSEL,PLL X轴为参考
        MPU6050_Write_One_Byte(MPU6050_ADDR,MPU6050_PWR_MGMT2_REG,1,0X00);//加速度和陀螺仪都工作
        MPU6050_Set_Rate(50);
    }
}

void MPU6050_Set_LPF(uint16_t lpf)
{
	uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6;
    MPU6050_Write_One_Byte(MPU6050_ADDR,MPU6050_CFG_REG,1,data);    //设置数字低通滤波器 
}

void MPU6050_Set_Rate(uint32_t rate)
{
    if(rate>100)
        rate = 1000;
    if(rate<4)
        rate = 4;
    MPU6050_Write_One_Byte(MPU6050_ADDR,MPU6050_SAMPLE_RATE_REG,1,(1000/rate-1));
    MPU6050_Set_LPF(rate/2);
}

void MPU6050_Get_Accelerometer(short *accel_x,short *accel_y,short *accel_z)
{
    uint8_t buf[6]; 
    MPU6050_Read_Bytes(MPU6050_ADDR,MPU6050_ACCEL_XOUTH_REG,6,buf); 
	*accel_x=((uint16_t)buf[0]<<8)|buf[1];  
	*accel_y=((uint16_t)buf[2]<<8)|buf[3];  
	*accel_z=((uint16_t)buf[4]<<8)|buf[5];
}

void MPU6050_Get_Gyroscope(short *gyro_x,short *gyro_y,short *gyro_z)
{
    uint8_t buf[6]; 
    MPU6050_Read_Bytes(MPU6050_ADDR,MPU6050_GYRO_XOUTH_REG,6,buf); 
	*gyro_x=((uint16_t)buf[0]<<8)|buf[1];  
	*gyro_y=((uint16_t)buf[2]<<8)|buf[3];  
	*gyro_z=((uint16_t)buf[4]<<8)|buf[5];
}
