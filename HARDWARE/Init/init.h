#ifndef _INIT_H
#define _INIT_H

#include "stm32f10x.h"
#include "SysTick.h"
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
#include "test.h"
#include <math.h>
#include <stdlib.h>
#include "TIM.h"
#include "Task_Dispatch.h"
#include "DS18B20.h"
#include "Link_List.h"

#define BEEP_ON_OFF             0
#define ESP_ON_OFF              0
#define DS18B20_ON_OFF          1
#define MAX30102_ON_OFF         0
#define MPU6050_ON_OFF          1
#define OLED_ON_OFF             1
#define TIM2_ON_OFF             0
#define TIM3_ON_OFF             0
#define TIM4_ON_OFF             0
#define FSM_ON_OFF              0

typedef enum fsm{
    fsm_idle_state = 0,
    fsm_ds18b20_convert_state,
    fsm_ds18b20_read_state,
    fsm_mpu6050_state,
    fsm_max30102_state,
    fsm_oled_calculate_state,
    fsm_oled_refresh_state,
    fsm_esp8266_state,
}fsm_state_t;

typedef enum event{
    fsm_start_event = 0,
    fsm_ds18b20_event,
    fsm_mpu6050_event,
    fsm_max30102_event,
    fsm_oled_event,
    fsm_esp8266_event,
    fsm_idle_sevent,
}fsm_event_t;

typedef struct {
    fsm_state_t pre_state;
    fsm_state_t cur_state;
}fsm_t;

typedef struct data_buff
{
    float temperature;
    float pitch;
    float roll;
    float yaw;
    int32_t SPO2;
    int32_t HR;
    uint32_t RED;
    uint32_t IR;
}data_buff_t;

#define LED1_Period             (25U)      //√
#define LED2_Period             (25U)      //√
#define DS18B20_READ_Period     (400U)    //√
#define DS18B20_CONVERT_Period  (5U)      //√
#define OLED_REFRESH_Period     (50U)     //√
#define OLED_CALCU_Period       (20U)     //√
#define MAX30102_Period         (1000U)
#define MPU6050_Period          (30U)     //√
#define ESP8266_Period          (50U)

#define LED1_ID                 (1)
#define LED2_ID                 (2)
#define DS18B20_READ_ID         (3)
#define DS18B20_CONVERT_ID      (4)
#define MPU6050_ID              (5)
#define OLED_REFRESH_ID         (6)
#define OLED_CALCU_ID           (7)
#define MAX30102_ID             (8)
#define ESP8266_ID              (9)

void peripheral_init(void);
void task_fsm_looper(fsm_t *m_fsm);
void fsm_init(fsm_t *fsm);
#endif
