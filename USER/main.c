#include "main.h"

#if ESP_ON_OFF
uint8_t ESP_Data[255];
#endif

#if DS18B20_ON_OFF
  float last_temperature = 0,cur_temperature = 0;                  //温度值
#endif

#if MAX30102_ON_OFF
  uint32_t RED, IR;                                               //红光和红外光
  int32_t SPO2_Value,HR_Value;                                    //血氧值和心率值
#endif

uint8_t OLED_State=0;                                           //中断状态码开关OLED

#if MPU6050_ON_OFF
  short accel_x,accel_y,accel_z;                                 //x,y,z轴的加速度
  short gyro_x,gyro_y,gyro_z;                                     //x,y,z轴的角速度
#endif

#if MPU6050_ON_OFF
  float cur_pitch,pre_pitch;
  float cur_roll,pre_roll;
  float cur_yaw,pre_yaw;
#endif

int main(void)
{
  uint8_t ret;
  Debug_USART_init();
  delay_init();
	LED_Init();
  Key_Init();
#if BEEP_ON_OFF
  BEEP_Init();                                                    //蜂鸣器Init
#endif

#if MAX30102_ON_OFF
  Max30102_Init();                                                //MAX30102 心率血氧传感器初始化
  LED_ON(1);
  printf("Max30102 Init Success\r\n");
#endif

#if DS18B20_ON_OFF
  ret = DS18B20_Init();                                            //DS18B20 温度传感器初始化
  if(ret==0){
    LED_ON(3);
    printf("DS18B20 Init Success\r\n");
  }else{
    printf("DS18B20 Init Fail\r\n");
    return 0;
  } 
#endif

#if MPU6050_ON_OFF
	ret = MPU_Init();                                                 //MPU6050 角速度，加速度传感器初始化
  if(!ret){
    printf("MPU6050 Init Success\r\n");
  }else{
    printf("MPU6050 Init Fail\r\n");
    return 0;
  }

  ret = mpu_dmp_init();
  if(ret == 0){
    LED_ON(4);
    printf("DMP Init Success\r\n");
  }else{
    printf("DMP Init Fail,ERR Code:%d\r\n",ret);
    return 0;
  }
  if(mpu_dmp_get_data(&cur_pitch,&cur_roll,&cur_yaw) == 0){
    pre_pitch = cur_pitch;
    pre_roll = cur_roll;
    pre_yaw = cur_yaw;
  }
#endif

#if ESP_ON_OFF
	ret = ESP8266_Init();                                           //ESP8266 WIFI模块初始化
  if(ret != 0){
    printf("Error State:%d\r\n",ret);
    return 0;
  }else{
    LED_ON(5);
    printf("ESP8266 Init Success\r\n");
  }
#endif

#if OLED_ON_OFF
	OLED_Init();
	OLED_Clear(0);                                                  //清屏
#endif

#if TIM2_ON_OFF
  tim2_init(19999,7199); //温度 2S读一次
#endif

#if TIM3_ON_OFF
  tim3_init(1999,7199);   //MPU6050 200ms读一次
#endif

#if TIM4_ON_OFF
  tim4_init(9999,7199);  //Max30102 1s读一次
#endif

#if MAX30102_ON_OFF
  Max30102_Get_First_Sample(&RED,&IR,&SPO2_Value,&HR_Value);
#endif

#if MPU6050_ON_OFF  
  mpu_dmp_get_data(&cur_pitch,&cur_roll,&cur_yaw);
#endif

  delay_ms(3000); //延时3s等待外设读取初始数据
  

  
  while (1)
  {
  //ESP发送数据
#if ESP_ON_OFF

//发送温度
#if DS18B20_ON_OFF		

    if(cur_temperature != last_temperature){
      ESP8266_Pub_Data(cur_temperature,Type_Temperature);
      last_temperature = cur_temperature;
    }

#endif

//发送心率血氧
#if MAX30102_ON_OFF

    if(SPO2_Value != 0 && HR_Value != 0){
      printf("SPO2:%d%%,HR:%d\r\n",SPO2_Value,HR_Value);
			ESP8266_Pub_Data(SPO2_Value,Type_SPO2);
			ESP8266_Pub_Data(HR_Value,Type_HR);
    }

#endif

#if MPU6050_ON_OFF

    if(fabs((cur_pitch-pre_pitch)) >= 60){
      BEEP_ON();
    }
    if(fabs(cur_roll-pre_roll) >= 60){
      BEEP_ON();
    }
    /*
    //偏航角不需要警报
    if(fabs(cur_yaw-pre_yaw) >= 60){
      BEEP_ON();
    }
    */
    pre_pitch = cur_pitch;
    pre_roll = cur_roll;
    pre_yaw = cur_yaw;
    
#endif 


#endif

//测试OLED
#if OLED_TEST_ON_OFF  
		TEST_English();          //英文显示测试
		OLED_Clear(0); 
		TEST_Number_Character(); //数字和符号显示测试
		OLED_Clear(0); 
		TEST_Chinese();          //中文显示测试
		OLED_Clear(0);  
#endif

#if OLED_ON_OFF               

    GUI_ShowString(0,0,(uint8_t*)"temperature:",8,1);                       //OLED显示温度
    GUI_ShowNum(96,0,cur_temperature,10,8,1);
      
    GUI_ShowString(0,8,(uint8_t*)"HR:",8,1);                                //OLED显示心率
    GUI_ShowNum(24,8,HR_Value,10,8,1);

    GUI_ShowString(0,16,(uint8_t*)"SPO2:",8,1);                             //OLED显示血氧浓度
    GUI_ShowNum(40,16,HR_Value,10,8,1);

    delay_ms(1000);  
    OLED_Clear(0);                                                          //延时两秒刷新OLED


#endif

  }
    
}


