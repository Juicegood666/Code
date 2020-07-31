#include "beep.h"
/*
函数名称：void BEEP_Init(void)
函数功能：蜂鸣器初始化
硬件链接：PB8
*/
void BEEP_Init(void)
{
	RCC->APB2ENR|=1<<3;//GPIOB时钟使能
	GPIOB->CRH&=0xFFFFFFF0;
	GPIOB->CRH|=0x00000003;
}

