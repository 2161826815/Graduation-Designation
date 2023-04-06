#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "gui.h"
#include "LED.h"
#include "beep.h"
#include "DS18B20.h"
#include "MAX30102.h"
#include "key.h"
#include "ESP8266.h"
#include "USART.h"
#include "MPU6050.h"
#include "main.h"


#if ESP_ON_OFF
uint8_t ESP_Data[255];
#endif
uint8_t OLED_State=0;                                           //中断状态码开关OLED
int main(void)
{
  uint8_t ret;                                                    //ESP8266初始化返回值

#if DS18B20_ON_OFF
  float last_temperature = 0,cur_temperature = 0;                                           //温度值
#endif

#if MAX30102_ON_OFF
  uint32_t RED, IR;                                               //红光和红外光
  int32_t SPO2_Value,HR_Value;                                    //血氧值和心率值
#endif

#if MPU6050_ON_OFF
  short accel_x,accel_y,accel_z;                                 //x,y,z轴的加速度
  short gyro_x,gyro_y,gyro_z;                                     //x,y,z轴的角速度
#endif

  Debug_USART_init();                                             //调试串口初始化
  delay_init();                                                   //Systick初始化
	LED_Init();                                                     //LED初始化
  Key_Init();                                                     //独立按键初始化
#if BEEP_ON_OFF
  BEEP_Init();                                                    //蜂鸣器Init
#endif
#if MAX30102_ON_OFF
		Max30102_Init();                                                //MAX30102 心率血氧传感器初始化
    printf("Max30102 Init Success\r\n");
#endif

#if ESP_ON_OFF
	ret = ESP8266_Init();                                           //ESP8266 WIFI模块初始化
  if(ret != 0){
    printf("Error State:%d\r\n",ret);
  }else{
    printf("ESP8266 Init Success\r\n");
  }
#endif

#if DS18B20_ON_OFF
  ret = DS18B20_Init();                                            //DS18B20 温度传感器初始化
  if(ret==0){
    printf("DS18B20 Init Success\r\n");
  }else{
    printf("DS18B20 Init Fail\r\n");
  } 
#endif

#if OLED_ON_OFF
	OLED_Init();
	OLED_Clear(0);                                                  //清屏
#endif

#if MPU6050_ON_OFF
	ret = MPU6050_Init();                                                 //MPU6050 角速度，加速度传感器初始化
  if(!ret){
    printf("MPU6050 Init Success\r\n");
  }else{
    printf("MPU6050 Init Fail\r\n");
  }
  
  ret = mpu_dmp_init();
  if(ret == 0){
    printf("DMP Init Success\r\n");
  }else{
    printf("DMP Init Fail,ERR Code:%d\r\n",ret);
  }
#endif

		LED_ON(1);
		LED_ON(3);
		LED_ON(4);
		LED_ON(5);
#if MAX30102_ON_OFF
    Max30102_Get_First_Sample(&RED,&IR,&SPO2_Value,&HR_Value);
#endif    
  while (1)
  {
#if DS18B20_ON_OFF		
		cur_temperature = DS18B20_Read_Temp();                           //采集温度
    if(cur_temperature != last_temperature){
      printf("Temp:%f\r\n",cur_temperature);
#if ESP_ON_OFF
      ESP8266_Pub_Data(cur_temperature,Type_Temperature);
#endif
      last_temperature = cur_temperature;
    }else{

    }
#endif

#if MAX30102_ON_OFF
    /* Max30102_Read_FIFO(&RED,&IR);
    Max30102_Get_First_Sample(&RED,&IR,&SPO2_Value,&HR_Value); */
    Max30102_Calculate(&RED,&IR,&SPO2_Value,&HR_Value);          //采集心率血氧
    //printf("RED:%d ,IR:%d\r\n",RED,IR);
    if(SPO2_Value != 0 && HR_Value != 0){
      printf("SPO2:%d%%,HR:%d\r\n",SPO2_Value,HR_Value);
#if ESP_ON_OFF
			ESP8266_Pub_Data(SPO2_Value,Type_SPO2);
			ESP8266_Pub_Data(HR_Value,Type_HR);
#endif
    }
#endif

#if MPU6050_ON_OFF
    float pitch;
    float roll;
    float yaw;
    if(mpu_dmp_get_data(&pitch,&roll,&yaw) == 0){
      MPU6050_Get_Accelerometer(&accel_x,&accel_y,&accel_z);
      MPU6050_Get_Gyroscope(&gyro_x,&gyro_y,&gyro_z);
      printf("Pitch:  %f\r\n",(float)pitch);
			printf("Roll:  %f\r\n",(float)roll);
			printf("yaw:  %f\r\n",(float)yaw);
    }
#endif 

  //GUI_ShowNum(24,8,5,10,8,1);
/*                   
    if(temperature > 40){
      LED_ON(1);
      BEEP_ON();
    }else{
      LED_OFF(1);
      BEEP_OFF();
    }

    ESP8266_Pub_Data(temperature,Type_Temperature);               //发布温度数据到云端
    ESP8266_Pub_Data(SPO2_Value,Type_SPO2);                       //发布血氧浓度数据到云端
    ESP8266_Pub_Data(HR_Value,Type_HR);                           //发布心率数据到云端

    GUI_ShowString(0,0,(u8*)"temperature:",8,1);                       //OLED显示温度
    GUI_ShowNum(96,0,temperature,10,8,1);
      
    GUI_ShowString(0,8,(u8*)"HR:",8,1);                                //OLED显示心率
    GUI_ShowNum(24,8,HR_Value,10,8,1);

    GUI_ShowString(0,16,(u8*)"SPO2:",8,1);                             //OLED显示血氧浓度
    GUI_ShowNum(40,16,HR_Value,10,8,1);

    delay_ms(2000);  
    OLED_Clear(0);                                                //延时两秒刷新OLED
    
    GUI_ShowString(0,8,(u8*)"accel_x:",8,1);                           //OLED显示血氧浓度
    GUI_ShowNum(64,8,accel_x,10,8,1);
    GUI_ShowString(0,16,(u8*)"accel_y:",8,1);                          //OLED显示血氧浓度
    GUI_ShowNum(64,16,accel_y,10,8,1);
    GUI_ShowString(0,24,(u8*)"accel_z:",8,1);                          //OLED显示血氧浓度
    GUI_ShowNum(64,24,accel_z,10,8,1);
    GUI_ShowString(0,32,(u8*)"gyro_x:",8,1);                           //OLED显示血氧浓度
    GUI_ShowNum(64,32,gyro_x,10,8,1);
    GUI_ShowString(0,40,(u8*)"gyro_y:",8,1);                           //OLED显示血氧浓度
    GUI_ShowNum(64,40,gyro_y,10,8,1);
    GUI_ShowString(0,48,(u8*)"gyro_z:",8,1);                           //OLED显示血氧浓度
    GUI_ShowNum(64,48,gyro_z,10,8,1);

    delay_ms(2000);  
    OLED_Clear(0);                                                //延时两秒刷新OLED
		*/
  }
    
}


