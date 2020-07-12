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
    //配置GPIO口
    GPIOB->CRL&=0x00FFFFFF;
    GPIOB->CRL|=0x22000000;
    GPIOB->CRH&=0xFFFFFF00;
    GPIOB->CRH|=0x00000022;
    //上拉
    GPIOB->ODR|=1<<6;
    GPIOB->ODR|=1<<7;
    GPIOB->ODR|=1<<8;
    GPIOB->ODR|=1<<9;
}

/*
函数功能: 蜂鸣器初始化
硬件连接: PA6
特性: 高电平响
*/
void BEEP_Init(void)
{
   RCC->APB2ENR|=1<<2;
   GPIOA->CRL&=0xF0FFFFFF;
   GPIOA->CRL|=0x02000000;
}

