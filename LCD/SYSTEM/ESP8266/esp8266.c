#include "esp8266.h"
struct ESP8266_WIFI esp8266_wifi;

/*
函数功能: 配置ESP8266WIFI为AP模式+TCP服务器模式
函数参数:
        char *ssid  :将要创建的WIFI热点名称(英文字母)
        char *password :将要创建的WIFI热点密码(最短长度8位)
        u16 port :TCP服务器的端口号(0~65535)
返 回 值:0表示成功,其他值表示失败
*/
u8 ESP8266_AP_TCP_ServerMode(const char *ssid,const char *password,u16 port)
{
    u8 i;
    char *find_str=NULL;
    char cmd_buffer[100];
    /*1. 发送测试指令,检测WIFI是否正常*/
    printf("发送测试指令,检测WIFI是否正常...\n");
    if(ESP8266_SendCmd("AT\r\n"))return 1;
    /*2. 关闭回显*/
    printf("关闭回显...\n");
    if(ESP8266_SendCmd("ATE0\r\n"))return 2;
    /*3. 设置当前WIFI的模式为AP模式*/
    printf("设置当前WIFI的模式为AP模式...\n");
    if(ESP8266_SendCmd("AT+CWMODE=2\r\n"))return 3;
    /*4. 复位模块,设置的模式必须复位之后才会生效*/
    printf("复位模块...\n");
    if(ESP8266_SendCmd("AT+RST\r\n"))return 4;
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    /*5. 关闭回显*/
    printf("关闭回显...\n");
    if(ESP8266_SendCmd("ATE0\r\n"))return 5;
    /*6. 设置创建的热点信息*/
    printf("设置创建的热点信息...\n");
    snprintf(cmd_buffer,100,"AT+CWSAP=\"%s\",\"%s\",1,4\r\n",ssid,password);
    if(ESP8266_SendCmd(cmd_buffer))return 6;
    /*7. 开启多连接,在服务器模式下才可开启*/
    printf("开启多连接...\n");
    if(ESP8266_SendCmd("AT+CIPMUX=1\r\n"))return 7;
    /*8. 设置端口号(0~65535)*/
    printf("设置端口号...\n");
    snprintf(cmd_buffer,100,"AT+CIPSERVER=1,%d\r\n",port);
    if(ESP8266_SendCmd(cmd_buffer))return 8;
    /*9.查询本地IP地址*/
    printf("查询本地IP地址...\n");
    if(ESP8266_SendCmd("AT+CIFSR\r\n"))return 9;
    
    /*10. 保存WIFI的信息*/
    esp8266_wifi.port=port;
    strcpy(esp8266_wifi.mode,"AP");
    
    //提取IP地址
    /*
    IP地址=+CIFSR:APIP,"192.168.4.1"
           +CIFSR:APMAC,"86:f3:eb:17:e6:86"
    */
    find_str=strstr((char*)USART3_RX_BUFF,"APIP");
    if(find_str) //判断是否查找成功
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
    
    printf("当前WIFI模式:%s\n",esp8266_wifi.mode);
    printf("当前网络协议类型:%s\n","TCP");
    printf("当前网络通信模式:%s\n","server");
    printf("当前网络端口号:%d\n",esp8266_wifi.port);
    printf("本地网络IP地址:%s\n",esp8266_wifi.ip);
    return 0;
}

/*
函数功能:向ESP8266wifi发送指令
说明:该函数只是适用于成功后返回OK的指令
返回值: 0表示成功 1表示失败
*/
u8 ESP8266_SendCmd(char *cmd)
{
   u8 i,j;
   for(i=0;i<5;i++) //测试的总次数
   {
      USART3_RX_FLAG=0;
      USART3_RX_CNT=0;
      USART_X_SendString(USART3,cmd);
      for(j=0;j<200;j++) //等待的时间(ms单位)
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
          delay_ms(2); //一次的时间
      }
   }
   return 1;
}

/*
函数功能:ESP8266wifi处于TCP服务器模式下的数据发送函数
函数参数:
        u8 id      :已经连接服务器的客户端ID号
        char *data :将要发送数据(字符串)
返回值: 0表示成功 1表示失败
说明:只有在TCP服务器模式下才可使用该函数

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
   for(i=0;i<5;i++) //测试的总次数
   {
      USART3_RX_FLAG=0;
      USART3_RX_CNT=0;
      USART_X_SendString(USART3,cmd);
      for(j=0;j<200;j++) //等待的时间(ms单位)
      {
          if(USART3_RX_FLAG)
          {
              USART3_RX_BUFF[USART3_RX_CNT]='\0';
              if(strstr((char*)USART3_RX_BUFF,">")) //开始发送数据
              {
                  USART3_RX_FLAG=0;
                  USART3_RX_CNT=0;
                  USART_X_SendString(USART3,data); //发送数据
                  for(n=0;n<10000;n++)
                  {
                      if(USART3_RX_FLAG)
                      {
                          USART3_RX_BUFF[USART3_RX_CNT]='\0';
                          if(strstr((char*)USART3_RX_BUFF,"SEND OK")) //开始发送数据
                          {
                              USART3_RX_FLAG=0;
                              USART3_RX_CNT=0;
                              return 0;
                          }
                          else return 1;
                      }
                      delay_ms(1); //一次的时间 
                  }
              }
              else break;
          }
          delay_ms(5); //一次的时间
      }
   }
   return 1;
}

/*
函数功能: 设置STA模式下连接的热点信息
返 回 值: 0表示成功,1表示失败

连接成功的提示符:
WIFI DISCONNECT
WIFI CONNECTED
WIFI GOT IP
OK
*/
u8 ESP8266_STA_ModeConnectWIFI(char *cmd)
{
   u32 i,j;
   for(i=0;i<5;i++) //测试的总次数
   {
      USART3_RX_FLAG=0;
      USART3_RX_CNT=0;
      USART_X_SendString(USART3,cmd);
      for(j=0;j<10000;j++) //等待的时间(ms单位)
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
          delay_ms(1); //一次的时间
      }
   }
   return 1;
}


/*
函数功能: 用于在客户端模式下,连接服务器
返 回 值: 0表示成功,1表示失败
连接成功的提示符:CONNECT
*/
u8 ESP8266_ConnectServer(char *cmd)
{
   u32 i,j;
   for(i=0;i<5;i++) //测试的总次数
   {
      USART3_RX_FLAG=0;
      USART3_RX_CNT=0;
      USART_X_SendString(USART3,cmd);
      for(j=0;j<10000;j++) //等待的时间(ms单位)
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
          delay_ms(1); //一次的时间
      }
   }
   return 1;
}


/*
函数功能: 配置ESP8266WIFI为STA模式+TCP客户端模式
函数参数:
        char *ssid  :将要连接的WIFI热点名称(英文字母)
        char *password :将要连接的WIFI热点密码(最短长度8位)
        u16 port :TCP服务器的端口号(0~65535)
返 回 值:0表示成功,其他值表示失败
*/
u8 ESP8266_STA_TCP_ClientMode(const char *ssid,const char *password,char *server_ip,u16 port)
{
    u8 i;
    char *find_str=NULL;
    char cmd_buffer[100];
    /*1. 发送测试指令,检测WIFI是否正常*/
    printf("发送测试指令,检测WIFI是否正常...\n");    
    for(i=0;i<10;i++)
    {
        if(ESP8266_SendCmd("AT\r\n")) //可能WIFI正处于透传模式，无法接收指令
        {
           USART_X_SendString(USART3,"+++"); //退出透传模式
           delay_ms(40); //等待退出透传模式
        }
        else break;
    }
    if(i==10)return 1; //检测失败
   
    /*2. 关闭回显*/
    printf("关闭回显...\n");
    if(ESP8266_SendCmd("ATE0\r\n"))return 2;
    /*3. 设置当前WIFI的模式为STA模式*/
    printf("设置当前WIFI的模式为STA模式...\n");
    if(ESP8266_SendCmd("AT+CWMODE=1\r\n"))return 3;
    /*4. 复位模块,设置的模式必须复位之后才会生效*/
    printf("复位模块...\n");
    if(ESP8266_SendCmd("AT+RST\r\n"))return 4;
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    /*5. 关闭回显*/
    printf("关闭回显...\n");
    if(ESP8266_SendCmd("ATE0\r\n"))return 5;
    /*6. 设置连接的热点信息*/
    printf("设置连接的热点信息...\n");
    snprintf(cmd_buffer,100,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,password);
    if(ESP8266_STA_ModeConnectWIFI(cmd_buffer))return 6;
    
    /*7. 开启单连接,在客户端模式下才可开启*/
    printf("开启单连接...\n");
    if(ESP8266_SendCmd("AT+CIPMUX=0\r\n"))return 8;
    
    /*8.查询本地IP地址*/
    printf("查询本地IP地址...\n");
    if(ESP8266_SendCmd("AT+CIFSR\r\n"))return 7;
    
    /*9. 保存WIFI的信息*/
    esp8266_wifi.port=port;
    strcpy(esp8266_wifi.mode,"STA");
    
    //提取IP地址
    /*
    +CIFSR:STAIP,"192.168.1.115"
    +CIFSR:STAMAC,"84:f3:eb:17:e6:86"
    */
    find_str=strstr((char*)USART3_RX_BUFF,"STAIP");
    if(find_str) //判断是否查找成功
    {
        find_str+=7;
        for(i=0;*find_str!='"';i++)
        {
            esp8266_wifi.ip[i]=*find_str;
            find_str++;
        }
        esp8266_wifi.ip[i]='\0';
    }
    
    /*10. 连接服务器*/
    printf("开始连接服务器...\n");
    snprintf(cmd_buffer,100,"AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",server_ip,port);
    if(ESP8266_ConnectServer(cmd_buffer))return 9;
    
    /*11. 开启透传模式*/
    printf("开启透传模式...\n");
    if(ESP8266_SendCmd("AT+CIPMODE=1\r\n"))return 10;
    
    /*12. 开始透传*/
    printf("开始透传...\n");
    if(ESP8266_SendCmd("AT+CIPSEND\r\n"))return 11;
    
    printf("当前WIFI模式:%s\n",esp8266_wifi.mode);
    printf("当前网络协议类型:%s\n","TCP");
    printf("当前网络通信模式:%s\n","Client");
    printf("连接的服务器端口号:%d\n",esp8266_wifi.port);
    printf("连接的服务器IP地址:%s\n",server_ip);
    printf("本地的IP:%s\n",esp8266_wifi.ip);
    return 0;
}


/*
函数功能: 配置ESP8266WIFI为STA模式+TCP客户端模式
函数参数:
        char *ssid  :将要连接的WIFI热点名称(英文字母)
        char *password :将要连接的WIFI热点密码(最短长度8位)
返 回 值:0表示成功,其他值表示失败
*/
u8 ESP8266_STA_TCP_ClientMode_HTTP(const char *ssid,const char *password)
{
    u8 i;
    char *find_str=NULL;
    char cmd_buffer[100];
    /*1. 发送测试指令,检测WIFI是否正常*/
    printf("发送测试指令,检测WIFI是否正常...\n");    
    for(i=0;i<10;i++)
    {
        if(ESP8266_SendCmd("AT\r\n")) //可能WIFI正处于透传模式，无法接收指令
        {
           USART_X_SendString(USART3,"+++"); //退出透传模式
           delay_ms(40); //等待退出透传模式
        }
        else break;
    }
    if(i==10)return 1; //检测失败
   
    /*2. 关闭回显*/
    printf("关闭回显...\n");
    if(ESP8266_SendCmd("ATE0\r\n"))return 2;
    /*3. 设置当前WIFI的模式为STA模式*/
    printf("设置当前WIFI的模式为STA模式...\n");
    if(ESP8266_SendCmd("AT+CWMODE=1\r\n"))return 3;
    /*4. 复位模块,设置的模式必须复位之后才会生效*/
    printf("复位模块...\n");
    if(ESP8266_SendCmd("AT+RST\r\n"))return 4;
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    /*5. 关闭回显*/
    printf("关闭回显...\n");
    if(ESP8266_SendCmd("ATE0\r\n"))return 5;
    /*6. 设置连接的热点信息*/
    printf("设置连接的热点信息...\n");
    snprintf(cmd_buffer,100,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,password);
    if(ESP8266_STA_ModeConnectWIFI(cmd_buffer))return 6;
    
    /*7. 开启单连接,在客户端模式下才可开启*/
    printf("开启单连接...\n");
    if(ESP8266_SendCmd("AT+CIPMUX=0\r\n"))return 8;
    
    /*8.查询本地IP地址*/
    printf("查询本地IP地址...\n");
    if(ESP8266_SendCmd("AT+CIFSR\r\n"))return 7;
    
    /*9. 保存WIFI的信息*/
    strcpy(esp8266_wifi.mode,"STA");
    
    //提取IP地址
    /*
    +CIFSR:STAIP,"192.168.1.115"
    +CIFSR:STAMAC,"84:f3:eb:17:e6:86"
    */
    find_str=strstr((char*)USART3_RX_BUFF,"STAIP");
    if(find_str) //判断是否查找成功
    {
        find_str+=7;
        for(i=0;*find_str!='"';i++)
        {
            esp8266_wifi.ip[i]=*find_str;
            find_str++;
        }
        esp8266_wifi.ip[i]='\0';
    }
    
    printf("当前WIFI模式:%s\n",esp8266_wifi.mode);
    printf("当前网络协议类型:%s\n","TCP");
    printf("当前网络通信模式:%s\n","Client");
    printf("本地的IP地址:%s\n",esp8266_wifi.ip);
    return 0;
}

/*
函数功能: (客户端情况下)连接服务器
函数参数:
        char *ssid  :将要连接的WIFI热点名称(英文字母)
        char *password :将要连接的WIFI热点密码(最短长度8位)
返 回 值:0表示成功,其他值表示失败
*/
u8 ESP8266_ClientHttpServer(char *server_ip,u16 port)
{

    char cmd_buffer[100];
    snprintf(cmd_buffer,100,"AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",server_ip,port);
    /*1. 连接服务器*/
    printf("开始连接服务器...\n");
    if(ESP8266_ConnectServer(cmd_buffer))return 1; //检测失败
      
    /*2. 开启透传模式*/
    printf("开启透传模式...\n");
    if(ESP8266_SendCmd("AT+CIPMODE=1\r\n"))return 2;
    
    /*3. 开始透传*/
    printf("开始透传...\n");
    if(ESP8266_SendCmd("AT+CIPSEND\r\n"))return 3;    
    return 0;
}

