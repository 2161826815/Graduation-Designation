#include "init.h"

extern float fir_pitch;
extern float fir_roll;
extern float fir_yaw;

data_buff_t all_data;
/*
priority config
mpu6050 > ds18b20_convert > ds18b20 > max30102 > esp8266 > 
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
extern Task_t m_oled_calcu_task;
extern Task_t m_oled_refresh_task;

extern list_item long_task;
extern list_item hit_task[HIT_LIST_TICK_MAX];

uint8_t MPU_IT_STATUS;

void peripheral_init(void)
{
    uint8_t i;
    sys_tick_init();
    LED_Init();
    Debug_USART_init();
    list_init(&long_task);
    for(i=0;i<HIT_LIST_TICK_MAX;i++){
        list_init(&hit_task[i]);
    }

    //通过不断闪烁LED代表任务正在不断调度切换
    /* led_task_init();
    task_add(&m_led_task,m_led_task.Period);
    led2_task_init();
    task_add(&m_led2_task,m_led2_task.Period); */

    Key_Init();

    BEEP_Init();
#if MPU6050_ON_OFF

    while(MPU_Init());                                                 //MPU6050 角速度，加速度传感器初始化
    DMA_Printf("MPU6050 Init Success\r\n");
    while(mpu_dmp_init());

    while(mpu_dmp_get_data(&fir_pitch,&fir_roll,&fir_yaw) != 0);
    DMA_Printf("DMP Init Success\r\n");

#if USE_MPU6050_IT
#else
    mpu6050_task_init();
    task_add(&m_mpu6050_task,m_mpu6050_task.Period);
#endif

#endif

#if MAX30102_ON_OFF
    Max30102_Init();                                                //MAX30102 心率血氧传感器初始化
    LED_ON(1);
    max30102_task_init();
    task_add(&m_max30102_task,m_max30102_task.Period);
    DMA_Printf("Max30102 Init Success\r\n");
#endif

#if OLED_ON_OFF
	OLED_Init();
	OLED_Clear(0);                                                  //清屏
    GUI_ShowString(0,0,(uint8_t*)"temp:",8,1);
    GUI_ShowString(0,10,(uint8_t*)"pitch:",8,1);
    GUI_ShowString(0,20,(uint8_t*)"roll:",8,1);
    GUI_ShowString(0,30,(uint8_t*)"yaw:",8,1);
    GUI_ShowString(0,40,(uint8_t*)"HR:",8,1);
    GUI_ShowString(0,50,(uint8_t*)"SPO2:",8,1);
    OLED_Display();

    oled_refresh_init();
    task_add(&m_oled_refresh_task,m_oled_refresh_task.Period);

    oled_calcu_Buff_task_init();
    task_add(&m_oled_calcu_task,m_oled_calcu_task.Period);
#endif

#if ESP_ON_OFF
    while(ESP8266_Init());                                           //ESP8266 WIFI模块初始化
    LED_ON(5);
    esp8266_task_init();
    task_add(&m_esp8266_task,m_esp8266_task.Period);
    DMA_Printf("ESP8266 Init Success\r\n");
#endif

#if DS18B20_ON_OFF
    while(DS18B20_Init());                                           //DS18B20 温度传感器初始化
    //DS18B20_Convert();
    DMA_Printf("DS18B20 Init Success\r\n");
    ds18b20_read_task_init();
    task_add(&m_ds18b20_read_task,m_ds18b20_read_task.Period);

    ds18b20_convert_task_init();
    task_add(&m_ds18b20_convert_task,m_ds18b20_convert_task.Period);
#endif

#if MPU6050_ON_OFF

#if USE_MPU6050_IT
    tim2_init(MPU6050_IT_TIME*10-1,7199); //mpu6050定时器
#endif

#endif
}

void task_looper(void)
{
    static uint32_t pre_tick;
    uint32_t cur_tick;

    tim3_init((TIM_IT_TIME*10-1),7199);   //启动任务定时器

    while(1){
        cur_tick = tim_get_tick();
        if(cur_tick != pre_tick){  
            pre_tick++;
            task_dispatch();
        }else{
            /*
                Wait Task
            */
        }
    } 
}
