#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "usart.h"
#include <string.h>
#include "timer.h"
#include "bluetooth.h"
#include "esp8266.h"


//STA+TCP�ͻ���ģʽ
char *ESP8266_STA_Client[]=
{
    "AT\r\n",
    "ATE0\r\n",
    "AT+CWMODE=1\r\n",
    "AT+RST\r\n",
    "ATE0\r\n",
    "AT+CWLAP\r\n", //��ѯ��������WIFI
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
   TIMER1_Init(72,20000); //��ʱʱ��20ms
   USART2_Init(9600);//����-����
   TIMER2_Init(72,20000); //��ʱʱ��20ms
   USART3_Init(115200);//����-WIFI
   TIMER3_Init(72,20000); //��ʱʱ��20ms
   USART1_Printf("���ڳ�ʼ��WIFI���Ե�.\n");

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
                USART1_Printf("����͸��ģʽ.\n");
            }
        }
        else if(key==3)
        {
            USARTx_StringSend(USART3,"12345ABCD"); 
        }
        else if(key==4) //�˳�͸��ģʽ
        {
            USARTx_StringSend(USART3,"+++");
        }
        else if(key==5) //����AT
        {
            USARTx_StringSend(USART3,"AT\r\n");
        }
   }
}

