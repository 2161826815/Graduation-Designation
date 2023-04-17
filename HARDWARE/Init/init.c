#include "init.h"

extern float fir_pitch;
extern float fir_roll;
extern float fir_yaw;

extern list_item long_task;
extern list_item hit_task;

data_buff_t all_data;
/*
priority config
mpu6050 > ds18b20_convert > ds18b20 > max30102 > esp8266 > oled_mpu6050 > oled_ds18b20 > oled_max30102 > 
led1 > led2 
*/
extern Task_t m_led_task;
extern Task_t m_led2_task;          
extern Task_t m_ds18b20_read_task;
extern Task_t m_ds18b20_convert_task;
extern Task_t m_max30102_task;
extern Task_t m_mpu6050_task;
extern Task_t m_oled_task;
extern Task_t m_esp8266_task;
extern Task_t m_oled_ds18b20_task;
extern Task_t m_oled_mpu6050_task;
extern Task_t m_oled_max30102_task;



void peripheral_init(void)
{
    Debug_USART_init();

    sys_tick_init();
    LED_Init();

    //通过不断闪烁LED代表任务正在不断调度切换
    led_task_init();
    task_add(&m_led_task);
    led2_task_init();
    task_add(&m_led2_task);

    Key_Init();

    BEEP_Init();


#if MAX30102_ON_OFF
    Max30102_Init();                                                //MAX30102 心率血氧传感器初始化
    LED_ON(1);
    max30102_task_init();
    task_add(&m_max30102_task);
    printf("Max30102 Init Success\r\n");
#endif

#if OLED_ON_OFF
	OLED_Init();
	OLED_Clear(0);                                                  //清屏
    GUI_ShowString(0,0,(uint8_t*)"temp:",8,1);
    GUI_ShowString(0,10,(uint8_t*)"pitch:",8,1);
    GUI_ShowString(0,20,(uint8_t*)"roll:",8,1);
    GUI_ShowString(0,30,(uint8_t*)"yaw:",8,1);

#if DS18B20_ON_OFF
    oled_ds18b20_task_init();
    task_add(&m_oled_ds18b20_task);
#endif

#if MPU6050_ON_OFF
    oled_mpu6050_task_init();
    task_add(&m_oled_mpu6050_task);
#endif

#endif

#if MPU6050_ON_OFF
    while(MPU_Init());                                                 //MPU6050 角速度，加速度传感器初始化
    printf("MPU6050 Init Success\r\n");
    while(mpu_dmp_init());

    while(mpu_dmp_get_data(&fir_pitch,&fir_roll,&fir_yaw) != 0);
    printf("DMP Init Success\r\n");
    mpu6050_task_init();
    task_add(&m_mpu6050_task);

#endif

#if ESP_ON_OFF
    while(ESP8266_Init());                                           //ESP8266 WIFI模块初始化
    LED_ON(5);
    esp8266_task_init();
    task_add(&m_esp8266_task);
    printf("ESP8266 Init Success\r\n");
#endif
    
#if DS18B20_ON_OFF
    while(DS18B20_Init());                                           //DS18B20 温度传感器初始化
    //DS18B20_Convert();
    printf("DS18B20 Init Success\r\n");
    ds18b20_read_task_init();
    task_add(&m_ds18b20_read_task);

    ds18b20_convert_task_init();
    task_add(&m_ds18b20_convert_task);

#endif

    tim3_init((HIT_TIME*10-1),7199);   //任务定时器
    //tim2_init(99,7199);
}
