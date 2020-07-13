#include "stm32f10x.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "timer.h"
#include "esp8266.h"
u8 buff[100];
u32 TIM_UPDATA_CNT=0;
//AP+TCP服务器模式
char *ESP8266_AP_Server[]=
{
    "AT\r\n",
    "ATE0\r\n",
    "AT+CWMODE=2\r\n",
    "AT+RST\r\n",
    "ATE0\r\n",
    "AT+CWSAP=\"wbyq_Cortex_M3\",\"12345678\",1,4\r\n",
    "AT+CIPMUX=1\r\n",
    "AT+CIPSERVER=1,8089\r\n",
    "AT+CIFSR\r\n" 
	
};
//STA+TCP客户端模式
char *ESP8266_STA_Client[]=
{
    "AT\r\n",
    "ATE0\r\n",
    "AT+CWMODE=1\r\n",
    "AT+RST\r\n",
    "ATE0\r\n",
    //"AT+CWLAP\r\n", //查询可以连接WIFI
    "AT+CWJAP=\"wbyq_STM32\",\"12345678\"\r\n",
    //"AT+CIFSR\r\n" 
    "AT+CIPMUX=0\r\n",
    "AT+CIPMODE=1\r\n",
    "AT+CIPSTART=\"TCP\",\"192.168.2.186\",8088\r\n",
    "AT+CIPSEND\r\n",  
};
char Onenote1[]={    "POST /devices/610233384/datapoints?type=3 HTTP/1.1\r\n"
					"api-key:cDJsejENQXKilzeMkuX2yM1gOMs=\r\n"
    "Host:api.heclouds.com\r\n"
    "Content-Length:24\r\n\r\n"
    "{\"temp\":18.99,\"humi\":77}"
				};
char Onenote2[]={    "POST /devices/610233384/datapoints?type=3 HTTP/1.1\r\n"
					"api-key:cDJsejENQXKilzeMkuX2yM1gOMs=\r\n"
    "Host:api.heclouds.com\r\n"
    "Content-Length:24\r\n\r\n"
    "{\"temp\":52.52,\"humi\":61}"
				};
char Onenote3[]={    "POST /devices/610233384/datapoints?type=3 HTTP/1.1\r\n"
					"api-key:cDJsejENQXKilzeMkuX2yM1gOMs=\r\n"
    "Host:api.heclouds.com\r\n"
    "Content-Length:24\r\n\r\n"
    "{\"temp\":11.11,\"humi\":31}"
				};
char Onenote4[]={    "POST /devices/610233384/datapoints?type=3 HTTP/1.1\r\n"
					"api-key:cDJsejENQXKilzeMkuX2yM1gOMs=\r\n"
    "Host:api.heclouds.com\r\n"
    "Content-Length:24\r\n\r\n"
    "{\"temp\":99.99,\"humi\":80}"
				};
int main(void)
{
	//u32 time;
	u8 key=0;
	u32 cnt=0;
	u32 vvvv=0;
	LED_Init();
	KEY_Init();
	USART1_Init(115200);

	USART3_Init(115200);
	TIMER1_Init(72,20000);
	TIMER3_Init(72,20000);
	USARTx_Stringsend(USART3,(u8*)"+++");
	DELAY_Ms(50);
	USART1_Printf("正在初始化WIFI请稍等.\n");
   if(ESP8266_STA_CLIENT_Init())
   {
      USART1_Printf("ESP8266硬件检测错误.\n");  
   }
   else
   {
      USART1_Printf("WIFI:%d\n",ESP8266_STA_TCP_CLIENT_MODE("8lite","1936778055","183.230.40.33",80,0));
   }
	while(1)
	{
		 if(USART3_RX_FLAG)
        {
          USART3_RX_BUFFER[USART3_RX_CNT]='\0';
          //USART1_Printf("%s",USART3_RX_BUFFER);
          USART3_RX_CNT=0;
          USART3_RX_FLAG=0;
        }
		key=KEY_Check(0);
		if(key==2)
        {
            cnt=0;
            USART1_Printf("OneNet1=%s\n",Onenote1);
            USART1_Printf("发送状态1:%d\n",ESP8266_ClientSendData((u8*)Onenote1,strlen(Onenote1))); 
        }
        else if(key==3)
        {
            cnt=0;
            USART1_Printf("发送状态2:%d\n",ESP8266_ClientSendData((u8*)Onenote2,strlen(Onenote2))); 
        }
		else if(key==4)
        {
            cnt=0;
            USART1_Printf("发送状态3:%d\n",ESP8266_ClientSendData((u8*)Onenote3,strlen(Onenote3))); 
        }
		else if(key==5)
        {
            cnt=0;
            USART1_Printf("发送状态4:%d\n",ESP8266_ClientSendData((u8*)Onenote4,strlen(Onenote4))); 
        }
		//5秒发送一次数据
        if(cnt>=5000)
        {
           cnt=0;
           USART1_Printf("发送状态(自动发送):%d\n",ESP8266_ClientSendData((u8*)Onenote2,strlen(Onenote2))); 
			
        }
        
        DELAY_Ms(1);
        cnt++;
		
	}
}
