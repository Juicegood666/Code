#include "led.h"

/*
函数名称：void LED_Init(void)
函数功能：LED初始化
硬件链接：
					LED0 -> PB5
					LED1 -> PE5
					低电平点亮
*/
void LED_Init(void)
{
	//打开时钟
	RCC->APB2ENR|=1<<3;//GPIOB时钟使能
  RCC->APB2ENR|=1<<6;//GPIOE时钟使能
	//配置寄存器
	GPIOB->CRL&=0xFF0FFFFF;
	GPIOB->CRL|=0x00300000;
	GPIOE->CRL&=0xFF0FFFFF;
	GPIOE->CRL|=0x00300033;
	//上拉
	GPIOB->ODR|=1<<5;
  GPIOE->ODR|=1<<5;
}

