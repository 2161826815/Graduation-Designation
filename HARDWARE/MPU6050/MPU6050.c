#include "mpu6050.h"
#include "sys.h"
#include "SysTick.h"
#include "USART.h"
#include "Task_Dispatch.h"
#include "init.h"
Task_t m_mpu6050_task;
float fir_pitch;
float fir_roll;
float fir_yaw;

uint8_t MPU_Init(void)
{
	uint8_t ret_addr,res;

	Soft_IIC_Init();

	Soft_IIC_Write_One_Byte(MPU_ADDR, MPU_PWR_MGMT1_REG, 0X80); // 复位MPU6050
	delay_ms(100);
	
	Soft_IIC_Write_One_Byte(MPU_ADDR, MPU_PWR_MGMT1_REG, 0X00); // 唤醒MPU6050
	MPU_Set_Gyro_Fsr(3);										// 陀螺仪传感器2000dps
	MPU_Set_Accel_Fsr(0);										// 加速度传感器±2g
	MPU_Set_Rate(50);											// 设置采样率50Hz
	Soft_IIC_Write_One_Byte(MPU_ADDR, MPU_INT_EN_REG, 0X00);	// 关闭所有中断
	Soft_IIC_Write_One_Byte(MPU_ADDR, MPU_USER_CTRL_REG, 0X00); // I2C主模式关闭
	Soft_IIC_Write_One_Byte(MPU_ADDR, MPU_FIFO_EN_REG, 0X00);	// 关闭FIFO
	Soft_IIC_Write_One_Byte(MPU_ADDR, MPU_INTBP_CFG_REG, 0X80); // INT低电平有效

	res = Soft_IIC_Read_Bytes(MPU_ADDR,MPU_DEVICE_ID_REG,1,&ret_addr);
	if (res == 0 && ret_addr == MPU_ADDR){ // ret_addr = MPU_ADDR = 0x68
	
		Soft_IIC_Write_One_Byte(MPU_ADDR, MPU_PWR_MGMT1_REG, 0X01); // 设置CLKSEL,PLL X轴为参考
		Soft_IIC_Write_One_Byte(MPU_ADDR, MPU_PWR_MGMT2_REG, 0X00); // 加速度与陀螺仪都工作
		MPU_Set_Rate(50);											// 设置采样率为50Hz
	} else
		return 1;
	return 0;	  // 读地址错误返回0
}
// 设置MPU6050陀螺仪传感器满量程范围
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
	return Soft_IIC_Write_One_Byte(MPU_ADDR, MPU_GYRO_CFG_REG, (fsr << 3)); // 设置陀螺仪满量程范围
}

// 设置MPU6050加速度传感器满量程范围
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return Soft_IIC_Write_One_Byte(MPU_ADDR, MPU_ACCEL_CFG_REG, fsr << 3); // 设置加速度传感器满量程范围
}

// 设置MPU6050的数字低通滤波器
uint8_t MPU_Set_LPF(u16 lpf)
{
	uint8_t data = 0;

	if (lpf >= 188)
		data = 1;
	else if (lpf >= 98)
		data = 2;
	else if (lpf >= 42)
		data = 3;
	else if (lpf >= 20)
		data = 4;
	else if (lpf >= 10)
		data = 5;
	else
		data = 6;
	return Soft_IIC_Write_One_Byte(MPU_ADDR, MPU_CFG_REG, data); // 设置数字低通滤波器
}

// 设置MPU6050的采样率(假定Fs=1KHz)
uint8_t MPU_Set_Rate(u16 rate)
{
	uint8_t data;
	if (rate > 1000)
		rate = 1000;
	if (rate < 4)
		rate = 4;
	data = 1000 / rate - 1;
	data = Soft_IIC_Write_One_Byte(MPU_ADDR, MPU_SAMPLE_RATE_REG, data); // 设置数字低通滤波器
	return MPU_Set_LPF(rate / 2);										 // 自动设置LPF为采样率的一半
}

// 得到温度传感器值
short MPU_Get_Temperature(void)
{
	uint8_t buf[2];
	short raw;
	float temp;

	Soft_IIC_Read_Bytes(MPU_ADDR, MPU_TEMP_OUTH_REG, 2, buf);
	raw = ((u16)buf[0] << 8) | buf[1];
	temp = 36.53 + ((double)raw) / 340;
	return temp * 100;
}

// 得到陀螺仪值(原始值)
uint8_t MPU_Get_Gyroscope(short *gx, short *gy, short *gz)
{
	uint8_t buf[6], res;

	res = Soft_IIC_Read_Bytes(MPU_ADDR, MPU_GYRO_XOUTH_REG, 6, buf);
	if (res == 0)
	{
		*gx = ((u16)buf[0] << 8) | buf[1];
		*gy = ((u16)buf[2] << 8) | buf[3];
		*gz = ((u16)buf[4] << 8) | buf[5];
	}
	return res;
}

// 得到加速度值(原始值)
uint8_t MPU_Get_Accelerometer(short *ax, short *ay, short *az)
{
	uint8_t buf[6], res;
	res = Soft_IIC_Read_Bytes(MPU_ADDR, MPU_ACCEL_XOUTH_REG, 6, buf);
	if (res == 0)
	{
		*ax = ((u16)buf[0] << 8) | buf[1];
		*ay = ((u16)buf[2] << 8) | buf[3];
		*az = ((u16)buf[4] << 8) | buf[5];
	}
	return res;
}

extern data_buff_t all_data;

void mpu6050_task(void)
{	
	while(mpu_dmp_get_data(&all_data.pitch,&all_data.roll,&all_data.yaw));
	printf("pitch:%.2f,roll:%.2f,yaw:%.2f\r\n",all_data.pitch,all_data.roll,all_data.yaw);
	if(all_data.pitch !=0 && all_data.roll!= 0 && all_data.yaw != 0){
		if(all_data.pitch-fir_pitch > 30 || all_data.pitch-fir_pitch < -30 ||	\
		 all_data.roll-fir_roll > 30 || all_data.roll-fir_roll < -30){
			BEEP_ON();
		}else{
			BEEP_OFF();
		}
	}else{
		LED_OFF(3);
	}
}

