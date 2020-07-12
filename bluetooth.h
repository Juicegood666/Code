#ifndef _BLUETOOOTH_
#define _BLUETOOOTH_
#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
extern u8 BLE_MAC_BUFFER[15];  //保存使蓝牙的MAC地址
u8 BLE_Init(void);
u8 BLE_GetMacAddr(void);
u8 BLE_Reset(void);
u8 BLE_SetName(char *name);
u8 BLE_GetConnectState(void);
#endif
