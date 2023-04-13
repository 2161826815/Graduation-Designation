#ifndef _MAIN_H
#define _MAIN_H

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
#include "test.h"
#include <math.h>
#include <stdlib.h>
#include "TIM.h"

#define BEEP_ON_OFF             0
#define ESP_ON_OFF              0
#define DS18B20_ON_OFF          1
#define MAX30102_ON_OFF         1
#define MPU6050_ON_OFF          1
#define OLED_ON_OFF             0
#define OLED_TEST_ON_OFF        0
#define TIM2_ON_OFF             1
#define TIM3_ON_OFF             1
#define TIM4_ON_OFF             0
#endif

void mpu6050_task(void);
void ds18b20_task(void);
void max30102_task(void);
