#ifndef _KEY_H
#define _KEY_H
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"

//定义按键IO口
#define KEY_UP PAin(0)
#define KEY0 PEin(4) 
#define KEY1 PEin(3)
#define KEY2 PEin(2)

//函数声明
void KEY_Init(void);
u8 KEY_Scan(u8 mode);
#endif
