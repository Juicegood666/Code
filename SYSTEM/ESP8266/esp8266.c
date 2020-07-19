#include "esp8266.h"
u8 ESP8266_IP_ADDR[16]; //255.255.255.255
u8 ESP8266_MAC_ADDR[18]; //Ӳ����ַ
/*
��������: ESP8266����ͺ���
��������ֵ:0��ʾ�ɹ�  1��ʾʧ��
*/
u8 ESP8266_SendCmd(char *cmd)
{
    u8 i,j;
    for(i=0;i<10;i++) //���Ĵ���--����ָ��Ĵ���
    {
        USARTx_StringSend(USART3,cmd);
        for(j=0;j<100;j++) //�ȴ���ʱ��
        {
            delay_ms(50);
            if(USART3_RX_FLAG)
            {
                USART3_RX_BUFFER[USART3_RX_CNT]='\0';
                USART3_RX_FLAG=0;
                USART3_RX_CNT=0;
                if(strstr((char*)USART3_RX_BUFFER,"OK"))
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

/*
��������: ESP8266Ӳ����ʼ����⺯��
��������ֵ:0��ʾ�ɹ�  1��ʾʧ��
*/
u8 ESP8266_Init(void)
{
    //�˳�͸��ģʽ
    USARTx_StringSend(USART3,"+++");
    delay_ms(50);
    return ESP8266_SendCmd("AT\r\n");
}

/*
��������: һ������WIFIΪAP+TCP������ģʽ
��������:
char *ssid  �������ȵ�����
char *pass  �������ȵ����� ������8λ��
u16 port    �����ķ������˿ں�
��������ֵ: 0��ʾ�ɹ� ����ֵ��ʾ��Ӧ����ֵ
*/
u8 ESP8266_AP_TCP_Server_Mode(char *ssid,char *pass,u16 port)
{
    char *p;
    u8 i;
    char ESP8266_SendCMD[100]; //��Ϸ��͹����е�����
    /*1. ����Ӳ��*/
    if(ESP8266_SendCmd("AT\r\n"))return 1;
    /*2. �رջ���*/
    if(ESP8266_SendCmd("ATE0\r\n"))return 2;
    /*3. ����WIFIģʽ*/
    if(ESP8266_SendCmd("AT+CWMODE=2\r\n"))return 3;
    /*4. ��λ*/
    ESP8266_SendCmd("AT+RST\r\n");
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    /*5. �رջ���*/
    if(ESP8266_SendCmd("ATE0\r\n"))return 5;
    /*6. ����WIFI��APģʽ����*/
    sprintf(ESP8266_SendCMD,"AT+CWSAP=\"%s\",\"%s\",1,4\r\n",ssid,pass);
    if(ESP8266_SendCmd(ESP8266_SendCMD))return 6;
    /*7. ����������*/
    if(ESP8266_SendCmd("AT+CIPMUX=1\r\n"))return 7;
    /*8. ���÷������˿ں�*/
    sprintf(ESP8266_SendCMD,"AT+CIPSERVER=1,%d\r\n",port);
    if(ESP8266_SendCmd(ESP8266_SendCMD))return 8;
    /*9. ��ѯ����IP��ַ*/
    if(ESP8266_SendCmd("AT+CIFSR\r\n"))return 9;
    //��ȡIP��ַ
    p=strstr((char*)USART3_RX_BUFFER,"APIP");
    if(p)
    {
        p+=6;
        for(i=0;*p!='"';i++)
        {
            ESP8266_IP_ADDR[i]=*p++;
        }
        ESP8266_IP_ADDR[i]='\0';
    }
    //��ȡMAC��ַ
    p=strstr((char*)USART3_RX_BUFFER,"APMAC");
    if(p)
    {
        p+=7;
        for(i=0;*p!='"';i++)
        {
            ESP8266_MAC_ADDR[i]=*p++;
        }
        ESP8266_MAC_ADDR[i]='\0';
    }
    
    //��ӡ������Ϣ
    USART1_Printf("��ǰWIFIģʽ:AP+TCP������\n");
    USART1_Printf("��ǰWIFI�ȵ�����:%s\n",ssid);
    USART1_Printf("��ǰWIFI�ȵ�����:%s\n",pass);
    USART1_Printf("��ǰTCP�������˿ں�:%d\n",port);
    USART1_Printf("��ǰTCP������IP��ַ:%s\n",ESP8266_IP_ADDR);
    USART1_Printf("��ǰTCP������MAC��ַ:%s\n",ESP8266_MAC_ADDR);
    return 0;
}

/*
��������: TCP������ģʽ�µķ��ͺ���
����ָ��: 
*/
u8 ESP8266_ServerSendData(u8 id,u8 *data,u16 len)
{
    u8 i,j,n;
    char ESP8266_SendCMD[100]; //��Ϸ��͹����е�����
    for(i=0;i<10;i++)
    {
        sprintf(ESP8266_SendCMD,"AT+CIPSEND=%d,%d\r\n",id,len);
        USARTx_StringSend(USART3,ESP8266_SendCMD);
        for(j=0;j<10;j++)
        {
            delay_ms(50);
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
                        delay_ms(50);
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
��������: ����WIFIΪSTAģʽ+TCP�ͻ���ģʽ
��������:
char *ssid  �������ȵ�����
char *pass  �������ȵ����� ������8λ��
char *p     ��Ҫ���ӵķ�����IP��ַ
u16 port    ��Ҫ���ӵķ������˿ں�
u8 flag     1��ʾ����͸��ģʽ 0��ʾ�ر�͸��ģʽ
��������ֵ:0��ʾ�ɹ�  ����ֵ��ʾ��Ӧ�Ĵ���
*/
u8 ESP8266_STA_TCP_Client_Mode(char *ssid,char *pass,char *ip,u16 port,u8 flag)
{
    char ESP8266_SendCMD[100]; //��Ϸ��͹����е�����
    //�˳�͸��ģʽ
    //USARTx_StringSend(USART3,"+++");
    //delay_ms(50);
    /*1. ����Ӳ��*/
    if(ESP8266_SendCmd("AT\r\n"))return 1;
    /*2. �رջ���*/
    if(ESP8266_SendCmd("ATE0\r\n"))return 2;
    /*3. ����WIFIģʽ*/
    if(ESP8266_SendCmd("AT+CWMODE=1\r\n"))return 3;
    /*4. ��λ*/
    ESP8266_SendCmd("AT+RST\r\n");
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    /*5. �رջ���*/
    if(ESP8266_SendCmd("ATE0\r\n"))return 5;
    /*6. ���ý�Ҫ���ӵ�WIFI�ȵ���Ϣ*/
    sprintf(ESP8266_SendCMD,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,pass);
    if(ESP8266_SendCmd(ESP8266_SendCMD))return 6;
    /*7. ���õ�����*/
    if(ESP8266_SendCmd("AT+CIPMUX=0\r\n"))return 7;
    /*8. ����Ҫ���ӵ�TCP��������Ϣ*/
    sprintf(ESP8266_SendCMD,"AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",ip,port);
    if(ESP8266_SendCmd(ESP8266_SendCMD))return 8;
    /*9. ����͸��ģʽ*/
    if(flag)
    {
       if(ESP8266_SendCmd("AT+CIPMODE=1\r\n"))return 9; //����
       if(ESP8266_SendCmd("AT+CIPSEND\r\n"))return 10;  //��ʼ͸��
       if(!(strstr((char*)USART3_RX_BUFFER,">")))
       {
            return 11;
       }
        //�����Ҫ�˳�����:  "+++"
    }
    
     //��ӡ������Ϣ
    USART1_Printf("��ǰWIFIģʽ:STA+TCP�ͻ���\n");
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
        USARTx_StringSend(USART3,ESP8266_SendCMD);
        for(j=0;j<10;j++)
        {
            delay_ms(50);
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
                        delay_ms(50);
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
