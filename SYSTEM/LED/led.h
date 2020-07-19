#ifndef _LED_H
#define _LED_H
#include "stm32f10x.h"
#include "sys.h"

//LED定义
#define LED0 PBout(5)
#define LED1 PEout(5)

//蜂鸣器IO口定义
#define BEEP PBout(8)

//函数声明
void LED_Init(void);
void BEEP_Init(void);
#endif
