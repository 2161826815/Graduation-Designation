#include "ESP8266.h"
#include "Task_Dispatch.h"
#include "init.h"
Task_t m_esp8266_task;
extern uint8_t DMA_RCV_Buffer[DMA_SIZE];
char Pub_Data[PUB_SIZE];
uint16_t RCV_CNT=0;
uint8_t ESP_STATE;

uint8_t AT_CIPSNTPCFG[]="AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"\r\n";    //设置地区时间连接阿里云

uint8_t AT_WIFI_INFO[]="AT+CWJAP=\"(G)I-DLE\",\"2161826815\"\r\n";   //连接WIFI

//设置MQTT用户
uint8_t AT_MQTTUSRCFG[]="AT+MQTTUSERCFG=0,1,\"NULL\",\"ESP8266&i5z42JpfDlV\",\"F153B43A65CC96889F0CEBF96984EE1993682453\",0,0,\"\"\r\n";

//设置MQTT客户端
uint8_t AT_MQTTCLIENTID[]="AT+MQTTCLIENTID=0,\"2161826815|securemode=2\\,signmethod=hmacsha1\\,timestamp=1676045469270|\"\r\n";

//以MQTT协议连接阿里云
uint8_t AT_MQTTCONN[]="AT+MQTTCONN=0,\"i5z42JpfDlV.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883,1\r\n";

uint8_t AF_MQTTPUB_TEMP[]="AT+MQTTPUB=0,\"/i5z42JpfDlV/ESP8266/user/temperature\",\"xxx\",1,0\r\n";  //发送温度值到云端，xxx需要格式化
uint8_t AF_MQTTPUB_HR_SPO2[]="AT+MQTTPUB=0,\"/i5z42JpfDlV/ESP8266/user/HR_SPO2\",\"xxx\",1,0\r\n";  //发送心率血氧值到云端，xxx需要格式化


//清空接收数组
void ESP8266_RCV_Clear(void)
{
    memset(DMA_RCV_Buffer,0,DMA_SIZE);
    RCV_CNT = 0;
}

//成功返回0.失败返回1
uint8_t ESP8266_Wait(void)
{  
    if(RCV_CNT == 0)
        return 1;
    else
        return 0;
}

uint8_t DMA_TC_STATUS = 0;
//发送AT指令，成功返回0，失败返回1
uint8_t ESP8266_Send_Cmd(uint8_t* cmd,const char* ret)
{
//串口接收中断
#ifdef RXNE
    uint16_t timeout = 50;
    while(timeout--){
        USART_Send_str(ESP8266_USARTX,cmd);
        if(strstr((const char*)DMA_RCV_Buffer,ret) != NULL){
            ESP8266_RCV_Clear();
            return 0;
        }else{

        }
    }
#endif

//串口空闲中断
#ifdef IDLE
    uint16_t timeout = 5;
    while(timeout--){
        USART_Send_str(ESP8266_USARTX,cmd);
        while(!DMA_TC_STATUS);
        if(strstr((const char*)DMA_RCV_Buffer,ret) != NULL){
            DMA_Printf("DMA_RCV_Buffer:  %s,times:%d\r\n",DMA_RCV_Buffer,timeout);
            
            DMA_Printf("Recieve OK\r\n");
            return 0;
        }else{
            DMA_Printf("Recieve Fail\r\n");
        }
        
        ESP8266_RCV_Clear();
        DMA_TC_STATUS = 0;
        DMA_Printf("2adasdas\r\n");
        delay_ms(1000);
    }
#endif

    return 1;
}

//ESP8266初始化 成功返回0，失败返回对应值
uint8_t ESP8266_Init()
{
    uint8_t ret;

    ESP8266_GPIO_Config();

    ret = ESP8266_Send_Cmd("AT\r\n","OK");      //测试是否正常工作
    if(ret != 0)
       return 1;


    ret = ESP8266_Send_Cmd("AT+RST\r\n","OK");  //复位
    if(ret != 0)
       return 2;


    ret = ESP8266_Send_Cmd("AT+CWMODE=3\r\n","OK");  //Station+AP模式
    if(ret != 0)
       return 4;


    ret = ESP8266_Send_Cmd(AT_CIPSNTPCFG,"OK");  //设置地区时间连接阿里云
    if(ret != 0)
       return 5;

    ret = ESP8266_Send_Cmd(AT_WIFI_INFO,"OK");  //连接WIFI
    if(ret != 0)
       return 6;

    ret = ESP8266_Send_Cmd(AT_MQTTUSRCFG,"OK");  //设置MQTT用户
    if(ret != 0)
       return 7;

    ret = ESP8266_Send_Cmd(AT_MQTTCLIENTID,"OK");  //设置MQTT客户端
    if(ret != 0)
       return 8;

    ret = ESP8266_Send_Cmd(AT_MQTTCONN,"OK");  //连接阿里云
    if(ret != 0)
       return 9;

    
    return 0;
}

//Success:0 Fail:1
uint8_t ESP8266_Pub_Data(float data,int type)
{
    switch(type){
    case Type_Temperature:
        sprintf(Pub_Data,"AT+MQTTPUB=0,\
\"/sys/i5z42JpfDlV/ESP8266/thing/event/property/post\",\
\"{\\\"method\\\":\\\"thing.service.property.set\\\"\\,\
\\\"id\\\":\\\"0\\\"\\,\\\"params\\\":{\\\"Temperature\\\":%f}\\,\
\\\"version\\\":\\\"1.0\\\"}\",\
1,0\r\n",data);

        if(ESP8266_Send_Cmd((uint8_t*)Pub_Data,"OK") == 0){
            DMA_Printf("Pub Temperature Success\r\n");
        }else{
            return 1;
        }
        break;
    case Type_HR:
        sprintf(Pub_Data,"AT+MQTTPUB=0,\
\"/sys/i5z42JpfDlV/ESP8266/thing/event/property/post\",\
\"{\\\"method\\\":\\\"thing.service.property.set\\\"\\,\
\\\"id\\\":\\\"1\\\"\\,\
\\\"params\\\":{\\\"HeartRate\\\":%d}\\,\
\\\"version\\\":\\\"1.0\\\"}\",\
1,0\r\n",(uint32_t)data);

        if(ESP8266_Send_Cmd((uint8_t*)Pub_Data,"OK") == 0){
            DMA_Printf("Pub HR Success\r\n");
        }else{
            return 1;
        }
        break;
       
    case Type_SPO2:
        sprintf(Pub_Data,"AT+MQTTPUB=0,\
\"/sys/i5z42JpfDlV/ESP8266/thing/event/property/post\",\
\"{\\\"method\\\":\\\"thing.service.property.set\\\"\\,\
\\\"id\\\":\\\"2\\\"\\,\\\"params\\\":{\\\"SPO2\\\":%d}\\,\
\\\"version\\\":\\\"1.0\\\"}\",\
1,0\r\n",(uint32_t)data);

        if(ESP8266_Send_Cmd((uint8_t*)Pub_Data,"OK") == 0){
            DMA_Printf("Pub SPO2 Success\r\n");
        }else{
            return 1;
        }  
        break;

    default:
        break;
    }
    memset(Pub_Data,0,sizeof(uint8_t)*PUB_SIZE);
    return 0;
}

extern data_buff_t all_data;
void esp8266_task(void)
{
    static float last_temp = 0;
    if(all_data.temperature != last_temp){
        while(ESP8266_Pub_Data(all_data.temperature,Type_Temperature));
        last_temp = all_data.temperature;
    }
    if(all_data.SPO2>0){
        while(ESP8266_Pub_Data(all_data.SPO2,Type_SPO2));
    }
    if(all_data.HR>0){
        while(ESP8266_Pub_Data(all_data.HR,Type_HR));
    }
}

/*
AT+MQTTPUB=0,"/sys/i5z42JpfDlV/ESP8266/thing/event/property/post","{\"method\":\"thing.service.property.set\"\,\"id\":\"2\"\,\"params\":{\"SPO2\":75}\,\"version\":\"1.0\"}",1,0
*/

/*
AT+MQTTPUB=0,"/sys/i5z42JpfDlV/ESP8266/thing/event/property/post","{\"method\":\"thing.service.property.set\"\,\"id\":\"1\"\,\"params\":{\"HeartRate\":66}\,\"version\":\"1.0\"}",1,0
*/

/*
AT+MQTTPUB=0,"/sys/i5z42JpfDlV/ESP8266/thing/event/property/post","{\"method\":\"thing.service.property.set\"\,\"id\":\"0\"\,\"params\":{\"Temperature\":78.7}\,\"version\":\"1.0\"}",1,0
*/

