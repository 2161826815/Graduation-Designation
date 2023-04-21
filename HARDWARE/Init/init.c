#include "init.h"

extern float fir_pitch;
extern float fir_roll;
extern float fir_yaw;

extern list_item long_task;
extern list_item hit_task[HIT_LIST_TICK_MAX];

data_buff_t all_data;

static Task_t m_led1_task = TASK_CTOR(led1_task,Period_to_Tick(LED1_Period),LED1_ID);
static Task_t m_led2_task = TASK_CTOR(led2_task,Period_to_Tick(LED2_Period),LED2_ID);
static Task_t m_ds18b20_read_task = TASK_CTOR(ds18b20_read_task,Period_to_Tick(DS18B20_READ_Period),DS18B20_READ_ID);
static Task_t m_ds18b20_convert_task = TASK_CTOR(ds18b20_convert_task,Period_to_Tick(DS18B20_CONVERT_Period),DS18B20_CONVERT_ID);
static Task_t m_max30102_task = TASK_CTOR(max30102_task,Period_to_Tick(MAX30102_Period),MAX30102_ID);
static Task_t m_mpu6050_task = TASK_CTOR(mpu6050_task,Period_to_Tick(MPU6050_Period),MPU6050_ID);
static Task_t m_esp8266_task = TASK_CTOR(esp8266_task,Period_to_Tick(ESP8266_Period),ESP8266_ID);
static Task_t m_oled_calcu_task = TASK_CTOR(oled_calcu_buffer_task,Period_to_Tick(OLED_CALCU_Period),OLED_CALCU_ID);
static Task_t m_oled_refresh_task = TASK_CTOR(oled_refresh_task,Period_to_Tick(OLED_REFRESH_Period),OLED_REFRESH_ID);

void peripheral_init(void)
{
    sys_tick_init();
    LED_Init();
    Debug_USART_init();

    list_init(&long_task);
    for(uint8_t i=0;i<HIT_LIST_TICK_MAX;i++){
        list_init(&hit_task[i]);
    }

    task_add(&m_led1_task,m_led1_task.period);
    task_add(&m_led2_task,m_led2_task.period);

    Key_Init();

    BEEP_Init();
    
#if MAX30102_ON_OFF
    Max30102_Init();
    task_add(&m_max30102_task,m_max30102_task.period);

    DMA_Printf("Max30102 Init Success\r\n");
#endif

#if MPU6050_ON_OFF

    while(MPU_Init())
    DMA_Printf("MPU6050 Init Success\r\n");
    while(mpu_dmp_init());

    while(mpu_dmp_get_data(&fir_pitch,&fir_roll,&fir_yaw) != 0);
    DMA_Printf("DMP Init Success\r\n");

    task_add(&m_mpu6050_task,m_mpu6050_task.period);

#endif

#if OLED_ON_OFF
	OLED_Init();
	OLED_Clear(0);
    GUI_ShowString(0,0,(uint8_t*)"temp:",8,1);
    GUI_ShowString(0,10,(uint8_t*)"pitch:",8,1);
    GUI_ShowString(0,20,(uint8_t*)"roll:",8,1);
    GUI_ShowString(0,30,(uint8_t*)"yaw:",8,1);
    GUI_ShowString(0,40,(uint8_t*)"HR:",8,1);
    GUI_ShowString(0,50,(uint8_t*)"SPO2:",8,1);
    OLED_Display();

    task_add(&m_oled_refresh_task,m_oled_refresh_task.period);
    task_add(&m_oled_calcu_task,m_oled_calcu_task.period);
#endif

#if ESP_ON_OFF
    while(ESP8266_Init());
    LED_ON(5);
    task_add(&m_esp8266_task,m_esp8266_task.period);
    DMA_Printf("ESP8266 Init Success\r\n");
#endif

#if DS18B20_ON_OFF
    while(DS18B20_Init());
    DMA_Printf("DS18B20 Init Success\r\n");

    task_add(&m_ds18b20_read_task,m_ds18b20_read_task.period);
    task_add(&m_ds18b20_convert_task,m_ds18b20_convert_task.period);
#endif
}

void task_dispatch_looper(void)
{
    static uint32_t pre_tick;
    uint32_t cur_tick;

    tim3_init((TIM_IT_TIME*10-1),7199);

    for(;;){
        cur_tick = tim_get_tick();
        if(cur_tick != pre_tick){  
            pre_tick++;
            task_dispatch();
        }
    } 
}

void fsm_init(fsm_t *fsm)
{
    fsm->pre_state = fsm_idle_state;
    fsm->cur_state = fsm_idle_state;
}

static uint8_t ds18b20_read_status = 1;
void task_fsm_looper(fsm_t *m_fsm)   
{
    uint8_t i = 0;
    static uint32_t pre_tick;
    uint32_t cur_tick;

    switch(m_fsm->cur_state){        //执行任务
    case fsm_idle_state:
        break;
    case fsm_ds18b20_convert_state:
        if(ds18b20_read_status){
            ds18b20_convert_task();
            ds18b20_read_status = 0;
            pre_tick = get_tick();
        }   
        break;
    case fsm_ds18b20_read_state:
        cur_tick = get_tick();
        if(cur_tick-pre_tick >= 480000){
            ds18b20_read_task();
            ds18b20_read_status = 1;
        }         
        break;
    case fsm_mpu6050_state:
        mpu6050_task();
        break;
    case fsm_max30102_state:
        max30102_task();   
        break;
    case fsm_oled_calculate_state:
        oled_calcu_buffer_task();
        break;
    case fsm_oled_refresh_state:
        oled_refresh_task();
        break;
    case fsm_esp8266_state:
        esp8266_task();
        DMA_Printf("esp8266\r\n");
        break;
    default:
        break;
    }

    switch(m_fsm->cur_state){        //切换任务
    case fsm_idle_state:
        m_fsm->cur_state = fsm_ds18b20_convert_state;
        break;
    case fsm_ds18b20_convert_state:
        m_fsm->cur_state = fsm_ds18b20_read_state;
        break;
    case fsm_ds18b20_read_state:
        m_fsm->cur_state = fsm_mpu6050_state;
        break;
    case fsm_mpu6050_state:
        m_fsm->cur_state = fsm_max30102_state;
        break;
    case fsm_max30102_state:
        m_fsm->cur_state = fsm_oled_calculate_state;
        break;
    case fsm_oled_calculate_state:
        m_fsm->cur_state = fsm_oled_refresh_state;
        break;
    case fsm_oled_refresh_state:
        m_fsm->cur_state = fsm_idle_state;
    //case fsm_esp8266_state:
    //    i = 0;
    //    m_fsm->cur_state = fsm_idle_state + i;
        break;
    default:
        break;
    }
}
