#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "usart.h"
#include <string.h>
#include "timer.h"
#include "bluetooth.h"
#include "esp8266.h"


//STA+TCP客户端模式
char *ESP8266_STA_Client[]=
{
    "AT\r\n",
    "ATE0\r\n",
    "AT+CWMODE=1\r\n",
    "AT+RST\r\n",
    "ATE0\r\n",
    "AT+CWLAP\r\n", //查询可以连接WIFI
    "AT+CWJAP=\"ChinaNet-wbyw\",\"12345678\"\r\n",
    "AT+CIPMUX=0\r\n",
    "AT+CIPMODE=1\r\n",
    "AT+CIPSTART=\"TCP\",\"192.168.101.6\",8088\r\n",
    "AT+CIPSEND\r\n",  
};

//"AT\r\n" "+++"
int main()
{
   u8 key,cnt=0;
   LED_Init();
   BEEP_Init();
   KEY_Init();
   USART1_Init(115200);
   TIMER1_Init(72,20000); //超时时间20ms
   USART2_Init(9600);//串口-蓝牙
   TIMER2_Init(72,20000); //超时时间20ms
   USART3_Init(115200);//串口-WIFI
   TIMER3_Init(72,20000); //超时时间20ms
   USART1_Printf("正在初始化WIFI请稍等.\n");

   while(1)
   {    
        if(USART3_RX_FLAG)
        {
          USART3_RX_BUFFER[USART3_RX_CNT]='\0';
          USART1_Printf("%s",USART3_RX_BUFFER);
          USART3_RX_CNT=0;
          USART3_RX_FLAG=0;
        }
        
        key=KEY_Scan(0);
        if(key==2)
        {
            USARTx_StringSend(USART3,ESP8266_STA_Client[cnt++]);
            if(cnt==12)
            {
                cnt=0;
                USART1_Printf("进入透传模式.\n");
            }
        }
        else if(key==3)
        {
            USARTx_StringSend(USART3,"12345ABCD"); 
        }
        else if(key==4) //退出透传模式
        {
            USARTx_StringSend(USART3,"+++");
        }
        else if(key==5) //发送AT
        {
            USARTx_StringSend(USART3,"AT\r\n");
        }
   }
}

