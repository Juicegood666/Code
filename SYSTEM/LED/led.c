#include "led.h"
/*
函数功能: LED初始化
硬件连接: PB6 PB7 PB8 PB9
特性: 低电平点亮
*/
void LED_Init(void)
{
    //开时钟
    RCC->APB2ENR|=1<<3;
	RCC->APB2ENR|=1<<6;
    //配置GPIO口
    GPIOB->CRL&=0xFF0FFFFF;
	GPIOB->CRL|=0x00300000;
	GPIOE->CRL&=0xFF0FFFFF;
	GPIOE->CRL|=0x00300000;
    //上拉
    GPIOB->ODR|=1<<5;
    GPIOE->ODR|=1<<5;
}

/*
函数功能: 蜂鸣器初始化
硬件连接: PA6
特性: 高电平响
*/
void BEEP_Init(void)
{
   RCC->APB2ENR|=1<<2;
   GPIOB->CRH&=0xFFFFFFF0;
   GPIOB->CRH|=0x00000002;
}



