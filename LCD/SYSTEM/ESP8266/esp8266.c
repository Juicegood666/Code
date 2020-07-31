#include "esp8266.h"
struct ESP8266_WIFI esp8266_wifi;

/*
��������: ����ESP8266WIFIΪAPģʽ+TCP������ģʽ
��������:
        char *ssid  :��Ҫ������WIFI�ȵ�����(Ӣ����ĸ)
        char *password :��Ҫ������WIFI�ȵ�����(��̳���8λ)
        u16 port :TCP�������Ķ˿ں�(0~65535)
�� �� ֵ:0��ʾ�ɹ�,����ֵ��ʾʧ��
*/
u8 ESP8266_AP_TCP_ServerMode(const char *ssid,const char *password,u16 port)
{
    u8 i;
    char *find_str=NULL;
    char cmd_buffer[100];
    /*1. ���Ͳ���ָ��,���WIFI�Ƿ�����*/
    printf("���Ͳ���ָ��,���WIFI�Ƿ�����...\n");
    if(ESP8266_SendCmd("AT\r\n"))return 1;
    /*2. �رջ���*/
    printf("�رջ���...\n");
    if(ESP8266_SendCmd("ATE0\r\n"))return 2;
    /*3. ���õ�ǰWIFI��ģʽΪAPģʽ*/
    printf("���õ�ǰWIFI��ģʽΪAPģʽ...\n");
    if(ESP8266_SendCmd("AT+CWMODE=2\r\n"))return 3;
    /*4. ��λģ��,���õ�ģʽ���븴λ֮��Ż���Ч*/
    printf("��λģ��...\n");
    if(ESP8266_SendCmd("AT+RST\r\n"))return 4;
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    /*5. �رջ���*/
    printf("�رջ���...\n");
    if(ESP8266_SendCmd("ATE0\r\n"))return 5;
    /*6. ���ô������ȵ���Ϣ*/
    printf("���ô������ȵ���Ϣ...\n");
    snprintf(cmd_buffer,100,"AT+CWSAP=\"%s\",\"%s\",1,4\r\n",ssid,password);
    if(ESP8266_SendCmd(cmd_buffer))return 6;
    /*7. ����������,�ڷ�����ģʽ�²ſɿ���*/
    printf("����������...\n");
    if(ESP8266_SendCmd("AT+CIPMUX=1\r\n"))return 7;
    /*8. ���ö˿ں�(0~65535)*/
    printf("���ö˿ں�...\n");
    snprintf(cmd_buffer,100,"AT+CIPSERVER=1,%d\r\n",port);
    if(ESP8266_SendCmd(cmd_buffer))return 8;
    /*9.��ѯ����IP��ַ*/
    printf("��ѯ����IP��ַ...\n");
    if(ESP8266_SendCmd("AT+CIFSR\r\n"))return 9;
    
    /*10. ����WIFI����Ϣ*/
    esp8266_wifi.port=port;
    strcpy(esp8266_wifi.mode,"AP");
    
    //��ȡIP��ַ
    /*
    IP��ַ=+CIFSR:APIP,"192.168.4.1"
           +CIFSR:APMAC,"86:f3:eb:17:e6:86"
    */
    find_str=strstr((char*)USART3_RX_BUFF,"APIP");
    if(find_str) //�ж��Ƿ���ҳɹ�
    {
        find_str+=6;
        for(i=0;*find_str!='"';i++)
        {
            esp8266_wifi.ip[i]=*find_str;
            find_str++;
        }
        esp8266_wifi.ip[i]='\0';
    }
    USART3_RX_FLAG=0;
    USART3_RX_CNT=0;
    
    printf("��ǰWIFIģʽ:%s\n",esp8266_wifi.mode);
    printf("��ǰ����Э������:%s\n","TCP");
    printf("��ǰ����ͨ��ģʽ:%s\n","server");
    printf("��ǰ����˿ں�:%d\n",esp8266_wifi.port);
    printf("��������IP��ַ:%s\n",esp8266_wifi.ip);
    return 0;
}

/*
��������:��ESP8266wifi����ָ��
˵��:�ú���ֻ�������ڳɹ��󷵻�OK��ָ��
����ֵ: 0��ʾ�ɹ� 1��ʾʧ��
*/
u8 ESP8266_SendCmd(char *cmd)
{
   u8 i,j;
   for(i=0;i<5;i++) //���Ե��ܴ���
   {
      USART3_RX_FLAG=0;
      USART3_RX_CNT=0;
      USART_X_SendString(USART3,cmd);
      for(j=0;j<200;j++) //�ȴ���ʱ��(ms��λ)
      {
          if(USART3_RX_FLAG)
          {
              USART3_RX_BUFF[USART3_RX_CNT]='\0';
              if(strstr((char*)USART3_RX_BUFF,"OK\r\n"))
              {
                  return 0;
              }
              else break;
          }
          delay_ms(2); //һ�ε�ʱ��
      }
   }
   return 1;
}

/*
��������:ESP8266wifi����TCP������ģʽ�µ����ݷ��ͺ���
��������:
        u8 id      :�Ѿ����ӷ������Ŀͻ���ID��
        char *data :��Ҫ��������(�ַ���)
����ֵ: 0��ʾ�ɹ� 1��ʾʧ��
˵��:ֻ����TCP������ģʽ�²ſ�ʹ�øú���

OK
> 
Recv 12 bytes
SEND OK
*/
u8 ESP8266_ServerSendData(u8 id,char *data)
{
   u32 i,j,n;
   char cmd[100];
   snprintf(cmd,1024,"AT+CIPSEND=%d,%d\r\n",id,strlen(data));
   for(i=0;i<5;i++) //���Ե��ܴ���
   {
      USART3_RX_FLAG=0;
      USART3_RX_CNT=0;
      USART_X_SendString(USART3,cmd);
      for(j=0;j<200;j++) //�ȴ���ʱ��(ms��λ)
      {
          if(USART3_RX_FLAG)
          {
              USART3_RX_BUFF[USART3_RX_CNT]='\0';
              if(strstr((char*)USART3_RX_BUFF,">")) //��ʼ��������
              {
                  USART3_RX_FLAG=0;
                  USART3_RX_CNT=0;
                  USART_X_SendString(USART3,data); //��������
                  for(n=0;n<10000;n++)
                  {
                      if(USART3_RX_FLAG)
                      {
                          USART3_RX_BUFF[USART3_RX_CNT]='\0';
                          if(strstr((char*)USART3_RX_BUFF,"SEND OK")) //��ʼ��������
                          {
                              USART3_RX_FLAG=0;
                              USART3_RX_CNT=0;
                              return 0;
                          }
                          else return 1;
                      }
                      delay_ms(1); //һ�ε�ʱ�� 
                  }
              }
              else break;
          }
          delay_ms(5); //һ�ε�ʱ��
      }
   }
   return 1;
}

/*
��������: ����STAģʽ�����ӵ��ȵ���Ϣ
�� �� ֵ: 0��ʾ�ɹ�,1��ʾʧ��

���ӳɹ�����ʾ��:
WIFI DISCONNECT
WIFI CONNECTED
WIFI GOT IP
OK
*/
u8 ESP8266_STA_ModeConnectWIFI(char *cmd)
{
   u32 i,j;
   for(i=0;i<5;i++) //���Ե��ܴ���
   {
      USART3_RX_FLAG=0;
      USART3_RX_CNT=0;
      USART_X_SendString(USART3,cmd);
      for(j=0;j<10000;j++) //�ȴ���ʱ��(ms��λ)
      {
          if(USART3_RX_FLAG)
          {
              USART3_RX_BUFF[USART3_RX_CNT]='\0';
              if(strstr((char*)USART3_RX_BUFF,"OK"))
              {
                  return 0;
              }
              else
              {
                  memset(USART3_RX_BUFF,0,sizeof(USART3_RX_BUFF));
                  USART3_RX_FLAG=0;
                  USART3_RX_CNT=0;
              }              
          }
          delay_ms(1); //һ�ε�ʱ��
      }
   }
   return 1;
}


/*
��������: �����ڿͻ���ģʽ��,���ӷ�����
�� �� ֵ: 0��ʾ�ɹ�,1��ʾʧ��
���ӳɹ�����ʾ��:CONNECT
*/
u8 ESP8266_ConnectServer(char *cmd)
{
   u32 i,j;
   for(i=0;i<5;i++) //���Ե��ܴ���
   {
      USART3_RX_FLAG=0;
      USART3_RX_CNT=0;
      USART_X_SendString(USART3,cmd);
      for(j=0;j<10000;j++) //�ȴ���ʱ��(ms��λ)
      {
          if(USART3_RX_FLAG)
          {
              USART3_RX_BUFF[USART3_RX_CNT]='\0';
              if(strstr((char*)USART3_RX_BUFF,"CONNECT"))
              {
                  return 0;
              }
              else
              {
                  memset(USART3_RX_BUFF,0,sizeof(USART3_RX_BUFF));
                  USART3_RX_FLAG=0;
                  USART3_RX_CNT=0;
              }              
          }
          delay_ms(1); //һ�ε�ʱ��
      }
   }
   return 1;
}


/*
��������: ����ESP8266WIFIΪSTAģʽ+TCP�ͻ���ģʽ
��������:
        char *ssid  :��Ҫ���ӵ�WIFI�ȵ�����(Ӣ����ĸ)
        char *password :��Ҫ���ӵ�WIFI�ȵ�����(��̳���8λ)
        u16 port :TCP�������Ķ˿ں�(0~65535)
�� �� ֵ:0��ʾ�ɹ�,����ֵ��ʾʧ��
*/
u8 ESP8266_STA_TCP_ClientMode(const char *ssid,const char *password,char *server_ip,u16 port)
{
    u8 i;
    char *find_str=NULL;
    char cmd_buffer[100];
    /*1. ���Ͳ���ָ��,���WIFI�Ƿ�����*/
    printf("���Ͳ���ָ��,���WIFI�Ƿ�����...\n");    
    for(i=0;i<10;i++)
    {
        if(ESP8266_SendCmd("AT\r\n")) //����WIFI������͸��ģʽ���޷�����ָ��
        {
           USART_X_SendString(USART3,"+++"); //�˳�͸��ģʽ
           delay_ms(40); //�ȴ��˳�͸��ģʽ
        }
        else break;
    }
    if(i==10)return 1; //���ʧ��
   
    /*2. �رջ���*/
    printf("�رջ���...\n");
    if(ESP8266_SendCmd("ATE0\r\n"))return 2;
    /*3. ���õ�ǰWIFI��ģʽΪSTAģʽ*/
    printf("���õ�ǰWIFI��ģʽΪSTAģʽ...\n");
    if(ESP8266_SendCmd("AT+CWMODE=1\r\n"))return 3;
    /*4. ��λģ��,���õ�ģʽ���븴λ֮��Ż���Ч*/
    printf("��λģ��...\n");
    if(ESP8266_SendCmd("AT+RST\r\n"))return 4;
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    /*5. �رջ���*/
    printf("�رջ���...\n");
    if(ESP8266_SendCmd("ATE0\r\n"))return 5;
    /*6. �������ӵ��ȵ���Ϣ*/
    printf("�������ӵ��ȵ���Ϣ...\n");
    snprintf(cmd_buffer,100,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,password);
    if(ESP8266_STA_ModeConnectWIFI(cmd_buffer))return 6;
    
    /*7. ����������,�ڿͻ���ģʽ�²ſɿ���*/
    printf("����������...\n");
    if(ESP8266_SendCmd("AT+CIPMUX=0\r\n"))return 8;
    
    /*8.��ѯ����IP��ַ*/
    printf("��ѯ����IP��ַ...\n");
    if(ESP8266_SendCmd("AT+CIFSR\r\n"))return 7;
    
    /*9. ����WIFI����Ϣ*/
    esp8266_wifi.port=port;
    strcpy(esp8266_wifi.mode,"STA");
    
    //��ȡIP��ַ
    /*
    +CIFSR:STAIP,"192.168.1.115"
    +CIFSR:STAMAC,"84:f3:eb:17:e6:86"
    */
    find_str=strstr((char*)USART3_RX_BUFF,"STAIP");
    if(find_str) //�ж��Ƿ���ҳɹ�
    {
        find_str+=7;
        for(i=0;*find_str!='"';i++)
        {
            esp8266_wifi.ip[i]=*find_str;
            find_str++;
        }
        esp8266_wifi.ip[i]='\0';
    }
    
    /*10. ���ӷ�����*/
    printf("��ʼ���ӷ�����...\n");
    snprintf(cmd_buffer,100,"AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",server_ip,port);
    if(ESP8266_ConnectServer(cmd_buffer))return 9;
    
    /*11. ����͸��ģʽ*/
    printf("����͸��ģʽ...\n");
    if(ESP8266_SendCmd("AT+CIPMODE=1\r\n"))return 10;
    
    /*12. ��ʼ͸��*/
    printf("��ʼ͸��...\n");
    if(ESP8266_SendCmd("AT+CIPSEND\r\n"))return 11;
    
    printf("��ǰWIFIģʽ:%s\n",esp8266_wifi.mode);
    printf("��ǰ����Э������:%s\n","TCP");
    printf("��ǰ����ͨ��ģʽ:%s\n","Client");
    printf("���ӵķ������˿ں�:%d\n",esp8266_wifi.port);
    printf("���ӵķ�����IP��ַ:%s\n",server_ip);
    printf("���ص�IP:%s\n",esp8266_wifi.ip);
    return 0;
}


/*
��������: ����ESP8266WIFIΪSTAģʽ+TCP�ͻ���ģʽ
��������:
        char *ssid  :��Ҫ���ӵ�WIFI�ȵ�����(Ӣ����ĸ)
        char *password :��Ҫ���ӵ�WIFI�ȵ�����(��̳���8λ)
�� �� ֵ:0��ʾ�ɹ�,����ֵ��ʾʧ��
*/
u8 ESP8266_STA_TCP_ClientMode_HTTP(const char *ssid,const char *password)
{
    u8 i;
    char *find_str=NULL;
    char cmd_buffer[100];
    /*1. ���Ͳ���ָ��,���WIFI�Ƿ�����*/
    printf("���Ͳ���ָ��,���WIFI�Ƿ�����...\n");    
    for(i=0;i<10;i++)
    {
        if(ESP8266_SendCmd("AT\r\n")) //����WIFI������͸��ģʽ���޷�����ָ��
        {
           USART_X_SendString(USART3,"+++"); //�˳�͸��ģʽ
           delay_ms(40); //�ȴ��˳�͸��ģʽ
        }
        else break;
    }
    if(i==10)return 1; //���ʧ��
   
    /*2. �رջ���*/
    printf("�رջ���...\n");
    if(ESP8266_SendCmd("ATE0\r\n"))return 2;
    /*3. ���õ�ǰWIFI��ģʽΪSTAģʽ*/
    printf("���õ�ǰWIFI��ģʽΪSTAģʽ...\n");
    if(ESP8266_SendCmd("AT+CWMODE=1\r\n"))return 3;
    /*4. ��λģ��,���õ�ģʽ���븴λ֮��Ż���Ч*/
    printf("��λģ��...\n");
    if(ESP8266_SendCmd("AT+RST\r\n"))return 4;
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    /*5. �رջ���*/
    printf("�رջ���...\n");
    if(ESP8266_SendCmd("ATE0\r\n"))return 5;
    /*6. �������ӵ��ȵ���Ϣ*/
    printf("�������ӵ��ȵ���Ϣ...\n");
    snprintf(cmd_buffer,100,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,password);
    if(ESP8266_STA_ModeConnectWIFI(cmd_buffer))return 6;
    
    /*7. ����������,�ڿͻ���ģʽ�²ſɿ���*/
    printf("����������...\n");
    if(ESP8266_SendCmd("AT+CIPMUX=0\r\n"))return 8;
    
    /*8.��ѯ����IP��ַ*/
    printf("��ѯ����IP��ַ...\n");
    if(ESP8266_SendCmd("AT+CIFSR\r\n"))return 7;
    
    /*9. ����WIFI����Ϣ*/
    strcpy(esp8266_wifi.mode,"STA");
    
    //��ȡIP��ַ
    /*
    +CIFSR:STAIP,"192.168.1.115"
    +CIFSR:STAMAC,"84:f3:eb:17:e6:86"
    */
    find_str=strstr((char*)USART3_RX_BUFF,"STAIP");
    if(find_str) //�ж��Ƿ���ҳɹ�
    {
        find_str+=7;
        for(i=0;*find_str!='"';i++)
        {
            esp8266_wifi.ip[i]=*find_str;
            find_str++;
        }
        esp8266_wifi.ip[i]='\0';
    }
    
    printf("��ǰWIFIģʽ:%s\n",esp8266_wifi.mode);
    printf("��ǰ����Э������:%s\n","TCP");
    printf("��ǰ����ͨ��ģʽ:%s\n","Client");
    printf("���ص�IP��ַ:%s\n",esp8266_wifi.ip);
    return 0;
}

/*
��������: (�ͻ��������)���ӷ�����
��������:
        char *ssid  :��Ҫ���ӵ�WIFI�ȵ�����(Ӣ����ĸ)
        char *password :��Ҫ���ӵ�WIFI�ȵ�����(��̳���8λ)
�� �� ֵ:0��ʾ�ɹ�,����ֵ��ʾʧ��
*/
u8 ESP8266_ClientHttpServer(char *server_ip,u16 port)
{

    char cmd_buffer[100];
    snprintf(cmd_buffer,100,"AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",server_ip,port);
    /*1. ���ӷ�����*/
    printf("��ʼ���ӷ�����...\n");
    if(ESP8266_ConnectServer(cmd_buffer))return 1; //���ʧ��
      
    /*2. ����͸��ģʽ*/
    printf("����͸��ģʽ...\n");
    if(ESP8266_SendCmd("AT+CIPMODE=1\r\n"))return 2;
    
    /*3. ��ʼ͸��*/
    printf("��ʼ͸��...\n");
    if(ESP8266_SendCmd("AT+CIPSEND\r\n"))return 3;    
    return 0;
}

