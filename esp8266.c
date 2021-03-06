#include "esp8266.h"
u8 ESP8266_IP_ADDR[16]; //255.255.255.255
u8 ESP8266_MAC_ADDR[18]; //硬件地址
/*
函数功能: ESP8266命令发送函数
函数返回值:0表示成功  1表示失败
*/
u8 ESP8266_SendCmd(char *cmd)
{
    u8 i,j;
    for(i=0;i<10;i++) //检测的次数--发送指令的次数
    {
        USARTx_StringSend(USART3,cmd);
        for(j=0;j<5;j++) //等待的时间
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
函数功能: ESP8266硬件初始化检测函数
函数返回值:0表示成功  1表示失败
*/
u8 ESP8266_Init(void)
{
    return ESP8266_SendCmd("AT\r\n");
}

/*
函数功能: 一键配置WIFI为AP+TCP服务器模式
函数参数:
char *ssid  创建的热点名称
char *pass  创建的热点密码 （最少8位）
u16 port    创建的服务器端口号
函数返回值: 0表示成功 其他值表示对应错误值
*/
u8 ESP8266_AP_TCP_Server_Mode(char *ssid,char *pass,u16 port)
{
    char *p;
    u8 i;
    char ESP8266_SendCMD[100]; //组合发送过程中的命令
    /*1. 测试硬件*/
    if(ESP8266_SendCmd("AT\r\n"))return 1;
    /*2. 关闭回显*/
    if(ESP8266_SendCmd("ATE0\r\n"))return 2;
    /*3. 设置WIFI模式*/
    if(ESP8266_SendCmd("AT+CWMODE=2\r\n"))return 3;
    /*4. 复位*/
    ESP8266_SendCmd("AT+RST\r\n");
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    /*5. 关闭回显*/
    if(ESP8266_SendCmd("ATE0\r\n"))return 5;
    /*6. 设置WIFI的AP模式参数*/
    sprintf(ESP8266_SendCMD,"AT+CWSAP=\"%s\",\"%s\",1,4\r\n",ssid,pass);
    if(ESP8266_SendCmd(ESP8266_SendCMD))return 6;
    /*7. 开启多连接*/
    if(ESP8266_SendCmd("AT+CIPMUX=1\r\n"))return 7;
    /*8. 设置服务器端口号*/
    sprintf(ESP8266_SendCMD,"AT+CIPSERVER=1,%d\r\n",port);
    if(ESP8266_SendCmd(ESP8266_SendCMD))return 8;
    /*9. 查询本地IP地址*/
    if(ESP8266_SendCmd("AT+CIFSR\r\n"))return 9;
    //提取IP地址
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
    //提取MAC地址
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
    
    //打印总体信息
    USART1_Printf("当前WIFI模式:AP+TCP服务器\n");
    USART1_Printf("当前WIFI热点名称:%s\n",ssid);
    USART1_Printf("当前WIFI热点密码:%s\n",pass);
    USART1_Printf("当前TCP服务器端口号:%d\n",port);
    USART1_Printf("当前TCP服务器IP地址:%s\n",ESP8266_IP_ADDR);
    USART1_Printf("当前TCP服务器MAC地址:%s\n",ESP8266_MAC_ADDR);
    return 0;
}

/*
函数功能: TCP服务器模式下的发送函数
发送指令: 
*/
u8 ESP8266_ServerSendData(u8 id,u8 *data,u16 len)
{
    u8 i,j,n;
    char ESP8266_SendCMD[100]; //组合发送过程中的命令
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
                    //继续发送数据
                    USARTx_DataSend(USART3,data,len);
                    //等待数据发送成功
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

