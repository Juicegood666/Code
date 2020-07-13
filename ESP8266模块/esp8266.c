#include "esp8266.h"
/*
��������:ESP8266 AP������ģʽӲ�����
��������ֵ:0�ɹ���1ʧ��
*/
u8 ESP8266_AP_SERVER_Init(void)
{
	return ESP8266_SENDCMD("AT\r\n");
}



/*
��������:ָ���
��������ֵ:0�ɹ���1ʧ��
*/
u8 ESP8266_SENDCMD(char *cmd)
{
	u8 i,j;
	for(i=0;i<10;i++)
	{
		USARTx_Stringsend(USART3,(u8*)cmd);
		USART1_Printf(">");
		for(j=0;j<100;j++)
		{
			DELAY_Ms(50);
			if(USART3_RX_FLAG)
			{
				USART3_RX_BUFFER[USART3_RX_CNT]='\0';
				USART3_RX_CNT=0;
				USART3_RX_FLAG=0;
				if(strstr((char *)USART3_RX_BUFFER,"OK"))
				{
					return 0;
				}
				
			}
		}
	}
	return 1;
}



/*
��������:ESP8266 AP���������ú���
�����β�:�������� ssid,���������� pass,�˿ں�: port 
*/
u8 ESP8266_IP_ADDR[16]; //255.255.255.255
u8 ESP8266_MAC_ADDR[18]; //Ӳ����ַ
u8 ESP8266_AP_TCP_SERVER_MODE(char *ssid,char *pass,u16 port)
{
	char i=0;
	char *ptr=NULL;
	char tmp[100];
	if(ESP8266_SENDCMD("AT\r\n"))return 1;
	if(ESP8266_SENDCMD("ATE0\r\n"))return 2;
	if(ESP8266_SENDCMD("AT+CWMODE=2\r\n"))return 3;
	ESP8266_SENDCMD("AT+RST\r\n");
	DELAY_Ms(1000);
	DELAY_Ms(1000);
	DELAY_Ms(1000);
	if(ESP8266_SENDCMD("ATE0\r\n"))return 5;
	sprintf(tmp,"AT+CWSAP=\"%s\",\"%s\",1,4\r\n",ssid,pass);
	if(ESP8266_SENDCMD(tmp))return 6;
	if(ESP8266_SENDCMD("AT+CIPMUX=1\r\n"))return 7;
	sprintf(tmp,"AT+CIPSERVER=1,%d\r\n",port);
	if(ESP8266_SENDCMD(tmp))return 8;
	if(ESP8266_SENDCMD("AT+CIFSR\r\n"))return 9;
	ptr=strstr((char *)USART3_RX_BUFFER,"APIP");
	if(ptr)
	{
		ptr+=6;
		for(i=0;*ptr!='"';i++)
		{
			ESP8266_IP_ADDR[i]=*ptr++;
		}
		ESP8266_IP_ADDR[i]='\0';
	}

	ptr=strstr((char *)USART3_RX_BUFFER,"APMAC");
	if(ptr)
	{
		ptr+=7;
		for(i=0;*ptr!='"';i++)
		{
			ESP8266_MAC_ADDR[i]=*ptr++;
		}
	}

	ESP8266_MAC_ADDR[i]='\0';
	USART1_Printf("\n��ǰWIFIģʽ:AP+TCP������\n");
	USART1_Printf("��ǰWIFI����:%s\n",ssid);
	USART1_Printf("��ǰWIFI����:%s\n",pass);
	USART1_Printf("��ǰTCP�������˿ں�:%d\n",port);
	USART1_Printf("��ǰWiFi��IP��ַ:%s\n",ESP8266_IP_ADDR);
	USART1_Printf("��ǰWiFi��MAC��ַ:%s\n",ESP8266_MAC_ADDR);
	return 0;
}




u8 ESP8266_ServerSendData(u8 id,u8 *data,u16 len)
{
    u8 i,j,n;
    char ESP8266_SendCMD[100]; //��Ϸ��͹����е�����
    for(i=0;i<10;i++)
    {
        sprintf(ESP8266_SendCMD,"AT+CIPSEND=%d,%d\r\n",id,len);
        USARTx_Stringsend(USART3,(u8*)ESP8266_SendCMD);
        for(j=0;j<10;j++)
        {
            DELAY_Ms(50);
            if(USART3_RX_FLAG)
            {
                USART3_RX_BUFFER[USART3_RX_CNT]='\0';
                USART3_RX_FLAG=0;
                USART3_RX_CNT=0;
                if(strstr((char*)USART3_RX_BUFFER,">"))
                {
                    //������������
                    USARTx_DataSend(USART3,data,len);
                    //�ȴ����ݷ��ͳɹ�
                    for(n=0;n<200;n++)
                    {
                        DELAY_Ms(50);
                        if(USART3_RX_FLAG)
                        {
                            USART3_RX_BUFFER[USART3_RX_CNT]='\0';
                            USART3_RX_FLAG=0;
                            USART3_RX_CNT=0;
                            if(strstr((char*)USART3_RX_BUFFER,"SEND OK"))
                            {
                                return 0;
                            }
                         }            
                    }   
                }
            }
        }
    }
    return 1;
}




/*
��������:ESP8266 STA �ͻ���ģʽӲ�����
��������ֵ:0�ɹ���1ʧ��
*/
u8 ESP8266_STA_CLIENT_Init(void)
{
	USARTx_Stringsend(USART3,(u8*)"+++");
	DELAY_Ms(50);
	return ESP8266_SENDCMD("AT\r\n");
}
/*
��������:ESP8266 STA�ͻ������ú���
�����β�:WIFI��:ssid��WIFI����:pass��WIFI IP:IP���˿ں�:port
*/
u8 ESP8266_STA_TCP_CLIENT_MODE(char *ssid,char *pass,char *ip,u16 port,u8 flag)
{
    char ESP8266_SendCMD[100]; //��Ϸ��͹����е�����
    //�˳�͸��ģʽ
    //USARTx_StringSend(USART3,"+++");
    //delay_ms(50);
    /*1. ����Ӳ��*/
    if(ESP8266_SENDCMD("AT\r\n"))return 1;
    /*2. �رջ���*/
    if(ESP8266_SENDCMD("ATE0\r\n"))return 2;
    /*3. ����WIFIģʽ*/
    if(ESP8266_SENDCMD("AT+CWMODE=1\r\n"))return 3;
    /*4. ��λ*/
    ESP8266_SENDCMD("AT+RST\r\n");
    DELAY_Ms(1000);
    DELAY_Ms(1000);
    DELAY_Ms(1000);
    /*5. �رջ���*/
    if(ESP8266_SENDCMD("ATE0\r\n"))return 5;
    /*6. ���ý�Ҫ���ӵ�WIFI�ȵ���Ϣ*/
    sprintf(ESP8266_SendCMD,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,pass);
    if(ESP8266_SENDCMD(ESP8266_SendCMD))return 6;
    /*7. ���õ�����*/
    if(ESP8266_SENDCMD("AT+CIPMUX=0\r\n"))return 7;
    /*8. ����Ҫ���ӵ�TCP��������Ϣ*/
    sprintf(ESP8266_SendCMD,"AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",ip,port);
    if(ESP8266_SENDCMD(ESP8266_SendCMD))return 8;
    /*9. ����͸��ģʽ*/
    if(flag)
    {
       if(ESP8266_SENDCMD("AT+CIPMODE=1\r\n"))return 9; //����
       if(ESP8266_SENDCMD("AT+CIPSEND\r\n"))return 10;  //��ʼ͸��
       if(!(strstr((char*)USART3_RX_BUFFER,">")))
       {
            return 11;
       }
        //�����Ҫ�˳�����:  "+++"
    }
    
     //��ӡ������Ϣ
    USART1_Printf("\n��ǰWIFIģʽ:STA+TCP�ͻ���\n");
    USART1_Printf("��ǰ���ӵ�WIFI�ȵ�����:%s\n",ssid);
    USART1_Printf("��ǰ���ӵ�WIFI�ȵ�����:%s\n",pass);
    USART1_Printf("��ǰ���ӵ�TCP�������˿ں�:%d\n",port);
    USART1_Printf("��ǰ���ӵ�TCP������IP��ַ:%s\n",ip);
    return 0;
}





/*
��������: TCP�ͻ���ģʽ�µķ��ͺ���
����ָ��: 
*/
u8 ESP8266_ClientSendData(u8 *data,u16 len)
{
    u8 i,j,n;
    char ESP8266_SendCMD[100]; //��Ϸ��͹����е�����
    for(i=0;i<10;i++)
    {
        sprintf(ESP8266_SendCMD,"AT+CIPSEND=%d\r\n",len);
        USARTx_Stringsend(USART3,(u8*)ESP8266_SendCMD);
        for(j=0;j<10;j++)
        {
            DELAY_Ms(50);
            if(USART3_RX_FLAG)
            {
                USART3_RX_BUFFER[USART3_RX_CNT]='\0';
                USART3_RX_FLAG=0;
                USART3_RX_CNT=0;
				USART1_Printf("111%s\n",USART3_RX_BUFFER);
                if(strstr((char*)USART3_RX_BUFFER,">"))
                {
                    //������������
                    USARTx_DataSend(USART3,data,len);
                    //�ȴ����ݷ��ͳɹ�
                    for(n=0;n<200;n++)
                    {
                        DELAY_Ms(50);
                        if(USART3_RX_FLAG)
                        {
                            USART3_RX_BUFFER[USART3_RX_CNT]='\0';
                            USART3_RX_FLAG=0;
                            USART3_RX_CNT=0;
							USART1_Printf("::%s\n",USART3_RX_BUFFER);
                            if(strstr((char*)USART3_RX_BUFFER,"OK"))
                            {
                                return 0;
                            }
                         }            
                    }   
                }
            }
        }
    }
    return 1;
}

