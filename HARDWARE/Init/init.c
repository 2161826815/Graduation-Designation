#include "init.h"

extern float fir_pitch;
extern float fir_roll;
extern float fir_yaw;

uint8_t MAX30102_RDY;

//priority oled > mpu6050 > max30102 > ds18b20 > esp8266 >led > oled_test
extern Task_t m_led_task;
extern Task_t m_led2_task;          
extern Task_t m_ds18b20_read_task;
extern Task_t m_ds18b20_convert_task;
extern Task_t m_max30102_task;
extern Task_t m_mpu6050_task;
extern Task_t m_oled_task;
extern Task_t m_oled_test_task;
extern Task_t m_esp8266_task;

data_buff_t all_data,pre_data;

void peripheral_init(list_item *task_head)
{
    Debug_USART_init();

    list_init(task_head);

    sys_tick_init();
    LED_Init();

    //通过不断闪烁LED代表任务正在不断调度切换
    led_task_init();
    task_add(task_head,&m_led_task);
    led2_task_init();
    task_add(task_head,&m_led2_task);

    Key_Init();

    BEEP_Init();


#if MAX30102_ON_OFF
    Max30102_Init();                                                //MAX30102 心率血氧传感器初始化
    LED_ON(1);
    max30102_task_init();
    task_add(task_head,&m_max30102_task);
    printf("Max30102 Init Success\r\n");
#endif

#if OLED_ON_OFF
	OLED_Init();
	OLED_Clear(0);                                                  //清屏
    GUI_ShowString(0,0,(uint8_t*)"temp:",8,1);
    GUI_ShowString(0,10,(uint8_t*)"pitch:",8,1);
    GUI_ShowString(0,20,(uint8_t*)"roll:",8,1);
    GUI_ShowString(0,30,(uint8_t*)"yaw:",8,1);
    oled_task_init();
    task_add(task_head,&m_oled_task);
#endif

#if OLED_TEST_ON_OFF
    OLED_Init();
	OLED_Clear(0);
    oled_test_task_init();
    task_add(task_head,&m_oled_test_task);
#endif

#if MPU6050_ON_OFF
    while(MPU_Init());                                                 //MPU6050 角速度，加速度传感器初始化
    //printf("MPU6050 Init Success\r\n");
    while(mpu_dmp_init());

    while(mpu_dmp_get_data(&fir_pitch,&fir_roll,&fir_yaw) != 0);
    //printf("DMP Init Success\r\n");
    LED_ON(1);
    mpu6050_task_init();
    task_add(task_head,&m_mpu6050_task);
#endif

#if ESP_ON_OFF
    while(ESP8266_Init());                                           //ESP8266 WIFI模块初始化
    LED_ON(5);
    esp8266_task_init();
    task_add(task_head,&m_esp8266_task);
    printf("ESP8266 Init Success\r\n");
#endif
    
#if DS18B20_ON_OFF
    while(DS18B20_Init());                                           //DS18B20 温度传感器初始化
    //DS18B20_Convert();

    ds18b20_read_task_init();
    task_add(task_head,&m_ds18b20_read_task);

    ds18b20_convert_task_init();
    task_add(task_head,&m_ds18b20_convert_task);
    //printf("DS18B20 Init Success\r\n");
#endif

    tim3_init((TIM_IT_TIME*10-1),7199);   //任务定时器
    //tim2_init(99,7199);   //mpu6050    10ms
}
