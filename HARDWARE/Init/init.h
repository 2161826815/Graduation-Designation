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

#define BEEP_ON_OFF             0
#define ESP_ON_OFF              0
#define DS18B20_ON_OFF          0
#define MAX30102_ON_OFF         1
#define MPU6050_ON_OFF          0
#define OLED_ON_OFF             0
#define OLED_TEST_ON_OFF        0
#define TIM2_ON_OFF             0
#define TIM3_ON_OFF             0
#define TIM4_ON_OFF             0
#define FSM_ON_OFF              0

typedef enum fsm{
    fsm_init_end = 1,
    fsm_task_ds18b20,
    fsm_task_mpu6050,
    fsm_task_max30102,
    fsm_task_esp8266,
    fsm_task_oled,
}fsm_t;

#define LED_Period          20
#define DS18B20_Period      20
#define OLED_Period         50
#define OLED_TEST_Period    50
#define MAX30102_Period     300
#define MPU6050_Period      300
#define ESP8266_Period      50

void mpu6050_task(void);
void ds18b20_task(void);
void max30102_task(void);

void peripheral_init(list_item *task_head);

#endif
