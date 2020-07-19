#ifndef _LED_H
#define _LED_H
#include "stm32f10x.h"
#include "sys.h"

//LED����
#define LED0 PBout(5)
#define LED1 PEout(5)

//������IO�ڶ���
#define BEEP PBout(8)

//��������
void LED_Init(void);
void BEEP_Init(void);
#endif
