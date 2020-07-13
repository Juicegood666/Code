#ifndef _ESP8266_H
#define _ESP8266_H
#include "stm32f10x.h"
#include "usart.h"
u8 ESP8266_AP_SERVER_Init(void);
u8 ESP8266_STA_CLIENT_Init(void);
u8 ESP8266_SENDCMD(char *cmd);
u8 ESP8266_AP_TCP_SERVER_MODE(char *ssid,char *pass,u16 port);
u8 ESP8266_STA_TCP_CLIENT_MODE(char *ssid,char *pass,char *ip,u16 port,u8 flag);
u8 ESP8266_ClientSendData(u8 *data,u16 len);
#endif

