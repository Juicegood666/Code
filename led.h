#ifndef _LED_H
#define _LED_H
#include "stm32f10x.h"
#include "sys.h"

//LED定义
#define LED1 PBout(6)
#define LED2 PBout(7)
#define LED3 PBout(8)
#define LED4 PBout(9)

//蜂鸣器IO口定义
#define BEEP PAout(6)

//函数声明
void LED_Init(void);
void BEEP_Init(void);
#endif
