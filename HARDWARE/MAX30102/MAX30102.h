#ifndef __MAX30102_H

#define __MAX30102_H

#include "stm32f10x.h"
#include "i2c.h"
#include "algorithm.h"
#include "USART.h"
#define MAX_BRIGHTNESS 255

#define MAX30102_IT_Port                            GPIOB
#define MAX30102_IT_Pin                             GPIO_Pin_5
#define MAX30102_IT_STATUS()                        GPIO_ReadInputDataBit(MAX30102_IT_Port,MAX30102_IT_Pin)

#define write_slave_addr                            0xAE
#define read_slave_addr                             0xAF

#define interrupt_status_1_rigister                 0x00
#define interrupt_enable_1_rigister                 0x02
#define interrupt_enable_2_rigister                 0x03
#define fifo_wr_ptr_rigister                        0x04
#define over_flow_cnt_rigister                      0x05
#define fifo_rd_ptr_rigister                        0x06
#define fifo_data_rigister                          0x07
#define fifo_config_rigister                        0x08
#define mode_config_rigister                        0x09
#define spO2_config_rigister                        0x0A
 

#define led1_pulse_amplitude_rigister               0x0C
#define led2_pulse_amplitude_rigister               0x0D
#define proximity_mode_led_pulse_amplitude_rigister 0x10
//#define Multi_LED_Mode_Control_Registers_slot1_2    0x11
//#define Multi_LED_Mode_Control_Registers_slot1_2    0x12
//#define Die_Temp_Integer                            0x1F
//#define Die_Temp_Fraction                           0x20
//#define Die_Temp_config                             0x20
//#define Proximity_Interrupt_Threshold               0x30

void Max30102_Reset(void);
void Max30102_Init(void);
void Max30102_Read_FIFO(uint32_t *RED,uint32_t *IR);
void Max30102_Calculate(uint32_t *RED,uint32_t *IR,int32_t *SPO2_Value,int32_t *HR_Value);
void Max30102_Get_First_Sample(uint32_t *RED,uint32_t *IR,int32_t *SPO2_Value,int32_t *HR_Value);
void max30102_task(void);
#endif
