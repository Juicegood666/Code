#ifndef BLE_H
#define BLE_H
#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include <string.h>
#include <stdio.h>
u8 BLE_InitCheck(void);
u8 BLE_GetMacAddr(void);
u8 BLE_SetName(const char *name);
u8 BLE_RST(void);
u8 BLE_GetName(void);
u8 BLE_GetConnectState(void);

//����������Ϣ
struct Bluetooth
{
   char mac[15]; //ʵ�ʵ�MAC��ַ��Ч������12
   char name[20];//ʵ�ʵ�������󳤶�18���ֽ�
};
extern struct Bluetooth bluetooth;
#endif
