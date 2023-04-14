#ifndef __ESP8266_H
#define __ESP8266_H

#include "stm32f10x.h"
#include "ESP_USART.h"
#include <stdio.h>
#include "SysTick.h"

/*
#define WIFI_ID     "(G)-IDLE"
#define WIFI_PWD    "2161826815www"
*/


enum Data_Type{
    Type_Temperature = 10,
    Type_HR = 11,
    Type_SPO2 = 12
};

#define ProductKey          "i5z42JpfDlV"
#define DeviceName          "ESP8266"
#define DeviceSecret        "a681a66a7bd767cd9563f16de543c875"
#define IP                  "i5z42JpfDlV.iot-as-mqtt.cn-shanghai.aliyuncs.com:1883"
#define Deviece_Clint_ID    "2161826815"

#define MQTT_Client_ID      "2161826815|securemode=2,signmethod=hmacsha1,timestamp=1676045469270|"
#define MQTT_Username       "ESP8266&i5z42JpfDlV"
#define MQTT_Password       "F153B43A65CC96889F0CEBF96984EE1993682453"
/*
#define AT_CIPSNTPCFG       "AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"\r\n"
#define AT_WIFI_INFO	        "AT+CWJAP=\"(G)-IDLE\",\"2161826815www\"\r\n"
#define AT_MQTTUSRCFG       "AT+MQTTUSERCFG=0,1,\"NULL\",\"ESP8266&i5z42JpfDlV\",\"F153B43A65CC96889F0CEBF96984EE1993682453\",0,0,\"\"\r\n"
#define AT_MQTTCLIENTID     "AT+MQTTCLIENTID=0,\"2161826815|securemode=2\\,signmethod=hmacsha1\\,timestamp=1676045469270|\"\r\n"
#define AT_MQTTCONN         "AT+MQTTCONN=0,\"i5z42JpfDlV.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883,1\r\n"
#define AF_MQTTPUB_TEMP     "AT+MQTTPUB=0,\"/i5z42JpfDlV/ESP8266/user/temperature\",\"xxx\",1,0\r\n"  //发送温度值到云端，xxx需要格式化
#define AF_MQTTPUB_TEMP     "AT+MQTTPUB=0,\"/i5z42JpfDlV/ESP8266/user/HR_SPO2\",\"xxx\",1,0\r\n"  //发送心率血氧值到云端，xxx需要格式化
*/

void ESP8266_RCV_Clear(void);
uint8_t ESP8266_Wait(void);
uint8_t ESP8266_Send_Cmd(uint8_t* cmd,const char* ret);
uint8_t ESP8266_Init(void);
uint8_t ESP8266_Pub_Data(float data,int type);
void esp8266_task_init(void);

#endif  
