#ifndef ESP8266_H
#define ESP8266_H
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
u8 ESP8266_SendCmd(char *cmd);
u8 ESP8266_AP_TCP_ServerMode(const char *ssid,const char *password,u16 port);
u8 ESP8266_ServerSendData(u8 id,char *data);
u8 ESP8266_STA_TCP_ClientMode(const char *ssid,const char *password,char *server_ip,u16 port);
u8 ESP8266_STA_TCP_ClientMode_HTTP(const char *ssid,const char *password);
u8 ESP8266_ClientHttpServer(char *server_ip,u16 port);
struct ESP8266_WIFI
{
   u16 port;
   char mode[10]; //sta/ap
   char ip[10];   //ipµÿ÷∑
};
extern struct ESP8266_WIFI esp8266_wifi;
#endif
