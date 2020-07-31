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

//保存蓝牙信息
struct Bluetooth
{
   char mac[15]; //实际的MAC地址有效长度是12
   char name[20];//实际的名称最大长度18个字节
};
extern struct Bluetooth bluetooth;
#endif
