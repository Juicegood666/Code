#ifndef _KEY_H
#define _KEY_H
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"

//定义按键IO口
#define KEY_S2 PAin(0)
#define KEY_S3 PAin(1) 
#define KEY_S4 PAin(4)
#define KEY_S5 PAin(5)

//函数声明
void KEY_Init(void);
u8 KEY_Scan(u8 mode);
#endif
