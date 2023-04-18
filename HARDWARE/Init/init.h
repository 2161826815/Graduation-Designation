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
#include "Task_List.h"
#include "DS18B20.h"

#define BEEP_ON_OFF             0
#define ESP_ON_OFF              0
#define DS18B20_ON_OFF          0
#define MAX30102_ON_OFF         0
#define MPU6050_ON_OFF          1
#define OLED_ON_OFF             1
#define TIM2_ON_OFF             0
#define TIM3_ON_OFF             0
#define TIM4_ON_OFF             0
#define FSM_ON_OFF              0

#define MPU6050_IT_TIME         5

typedef enum fsm{
    fsm_init_end = 1,
    fsm_task_ds18b20,
    fsm_task_mpu6050,
    fsm_task_max30102,
    fsm_task_esp8266,
    fsm_task_oled,
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

#define LED_Period              20U
#define LED2_Period             20U
#define DS18B20_READ_Period     750U
#define DS18B20_convert_Period  5U
#define OLED_DS18B20_Period     100U
#define OLED_MPU6050_Period     100U
#define OLED_MAX30102_Period    100U
#define OLED_Period             10U
#define MAX30102_Period         20U
#define MPU6050_Period          10U
#define ESP8266_Period          50U


void peripheral_init(void);
#endif
