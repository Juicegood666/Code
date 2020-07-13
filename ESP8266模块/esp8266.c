#include "esp8266.h"
/*
函数功能:ESP8266 AP服务器模式硬件检测
函数返回值:0成功，1失败
*/
u8 ESP8266_AP_SERVER_Init(void)
{
	return ESP8266_SENDCMD("AT\r\n");
}



/*
函数功能:指令发送
函数返回值:0成功，1失败
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
函数功能:ESP8266 AP服务器配置函数
函数形参:服务器名 ssid,服务器密码 pass,端口号: port 
*/
u8 ESP8266_IP_ADDR[16]; //255.255.255.255
u8 ESP8266_MAC_ADDR[18]; //硬件地址
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
	USART1_Printf("\n当前WIFI模式:AP+TCP服务器\n");
	USART1_Printf("当前WIFI名字:%s\n",ssid);
	USART1_Printf("当前WIFI密码:%s\n",pass);
	USART1_Printf("当前TCP服务器端口号:%d\n",port);
	USART1_Printf("当前WiFi的IP地址:%s\n",ESP8266_IP_ADDR);
	USART1_Printf("当前WiFi的MAC地址:%s\n",ESP8266_MAC_ADDR);
	return 0;
}




u8 ESP8266_ServerSendData(u8 id,u8 *data,u16 len)
{
    u8 i,j,n;
    char ESP8266_SendCMD[100]; //组合发送过程中的命令
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
                    //继续发送数据
                    USARTx_DataSend(USART3,data,len);
                    //等待数据发送成功
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
函数功能:ESP8266 STA 客户端模式硬件检测
函数返回值:0成功，1失败
*/
u8 ESP8266_STA_CLIENT_Init(void)
{
	USARTx_Stringsend(USART3,(u8*)"+++");
	DELAY_Ms(50);
	return ESP8266_SENDCMD("AT\r\n");
}
/*
函数功能:ESP8266 STA客户端配置函数
函数形参:WIFI名:ssid、WIFI密码:pass、WIFI IP:IP、端口号:port
*/
u8 ESP8266_STA_TCP_CLIENT_MODE(char *ssid,char *pass,char *ip,u16 port,u8 flag)
{
    char ESP8266_SendCMD[100]; //组合发送过程中的命令
    //退出透传模式
    //USARTx_StringSend(USART3,"+++");
    //delay_ms(50);
    /*1. 测试硬件*/
    if(ESP8266_SENDCMD("AT\r\n"))return 1;
    /*2. 关闭回显*/
    if(ESP8266_SENDCMD("ATE0\r\n"))return 2;
    /*3. 设置WIFI模式*/
    if(ESP8266_SENDCMD("AT+CWMODE=1\r\n"))return 3;
    /*4. 复位*/
    ESP8266_SENDCMD("AT+RST\r\n");
    DELAY_Ms(1000);
    DELAY_Ms(1000);
    DELAY_Ms(1000);
    /*5. 关闭回显*/
    if(ESP8266_SENDCMD("ATE0\r\n"))return 5;
    /*6. 配置将要连接的WIFI热点信息*/
    sprintf(ESP8266_SendCMD,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,pass);
    if(ESP8266_SENDCMD(ESP8266_SendCMD))return 6;
    /*7. 设置单连接*/
    if(ESP8266_SENDCMD("AT+CIPMUX=0\r\n"))return 7;
    /*8. 配置要连接的TCP服务器信息*/
    sprintf(ESP8266_SendCMD,"AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",ip,port);
    if(ESP8266_SENDCMD(ESP8266_SendCMD))return 8;
    /*9. 开启透传模式*/
    if(flag)
    {
       if(ESP8266_SENDCMD("AT+CIPMODE=1\r\n"))return 9; //开启
       if(ESP8266_SENDCMD("AT+CIPSEND\r\n"))return 10;  //开始透传
       if(!(strstr((char*)USART3_RX_BUFFER,">")))
       {
            return 11;
       }
        //如果想要退出发送:  "+++"
    }
    
     //打印总体信息
    USART1_Printf("\n当前WIFI模式:STA+TCP客户端\n");
    USART1_Printf("当前连接的WIFI热点名称:%s\n",ssid);
    USART1_Printf("当前连接的WIFI热点密码:%s\n",pass);
    USART1_Printf("当前连接的TCP服务器端口号:%d\n",port);
    USART1_Printf("当前连接的TCP服务器IP地址:%s\n",ip);
    return 0;
}





/*
函数功能: TCP客户端模式下的发送函数
发送指令: 
*/
u8 ESP8266_ClientSendData(u8 *data,u16 len)
{
    u8 i,j,n;
    char ESP8266_SendCMD[100]; //组合发送过程中的命令
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
                    //继续发送数据
                    USARTx_DataSend(USART3,data,len);
                    //等待数据发送成功
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

