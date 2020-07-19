#include "exti.h"
#include <stdio.h>
#include "delay.h"
#include "timer.h"
/*

函数名称：void EXTI_KEY_Init(void)
函数功能：按键中断
硬件链接：
						S2 ->PA0
						S3 ->PA1
						S4 ->PA4
						S5 ->PA5
*/
void EXTI_KEY_Init(void)
{
	KEY_Init();
	EXTI->IMR|=1<<0;//开放中断线0的中断请求
	EXTI->IMR|=1<<1;//开放中断线1的中断请求
	EXTI->IMR|=1<<4;//开放中断线4的中断请求
	EXTI->IMR|=1<<5;//开放中断线5的中断请求
	EXTI->FTSR|=1<<0;//允许中断线0下降沿触发请求
	EXTI->RTSR|=1<<0;//允许中断线0上升沿沿触发请求
	EXTI->FTSR|=1<<1;//允许中断线1下降沿触发请求
	EXTI->RTSR|=1<<1;//允许中断线1上升沿沿触发请求
	EXTI->FTSR|=1<<4;//允许中断线4下降沿触发请求
	EXTI->RTSR|=1<<4;//允许中断线4上升沿沿触发请求
	EXTI->FTSR|=1<<5;//允许中断线5下降沿触发请求
	EXTI->RTSR|=1<<5;//允许中断线5上升沿沿触发请求
	RCC->APB2ENR|=1<<0;//AFIO时钟使能
	AFIO->EXTICR[0]&=~(0xF<< (0*4));//选择输入源为PA0
	AFIO->EXTICR[0]&=~(0xF<< (1*4));//选择输入源为PA1
	AFIO->EXTICR[1]&=~(0xF<< (0*4));//选择输入源为PA4
	AFIO->EXTICR[1]&=~(0xF<< (1*4));//选择输入源为PA5
	STM32_SetPriority(EXTI0_IRQn,0,1);
	STM32_SetPriority(EXTI1_IRQn,1,1);
	STM32_SetPriority(EXTI4_IRQn,2,1);
	STM32_SetPriority(EXTI9_5_IRQn,3,1);
}
/*
函数名称：void EVENT_Init(void)
函数功能：事件中断
*/
void EVENT_Init(void)
{
	EXTI->IMR|=1<<1;//开放中断线1的中断请求
	EXTI->EMR|=1<<1;//开放中断线1的事件请求
	STM32_SetPriority(EXTI1_IRQn,1,1);
}

void EXTI0_IRQHandler(void)
{
	static u8 exti_flag=0;//按键标识位  0代表没有按下  1代表已经按下
	u32 time;
	delay_ms(20);
	if(exti_flag==0 && KEY_UP)
	{
		exti_flag=1;
		TIM4->CNT=0;
		
		printf("S2按下\r\n");
	}
	else if(exti_flag==1 && !KEY_UP)
	{
		exti_flag=0;
		
		printf("按键按下的时间为%d ms\r\n",time);
		printf("UP松开\r\n");
	}
	EXTI->PR|=1<<0;//清除中断线0中断请求
}

void EXTI1_IRQHandler(void)
{
	
	if(!KEY0)
	{
		printf("KEY0按下\r\n");
	}
	else
	{
		printf("KEY0松开\r\n");
	}
	
	EXTI->PR|=1<<1;//清除中断线1中断请求
}

void EXTI4_IRQHandler(void)
{
	
	if(!KEY1)
	{
		printf("KEY1按下\r\n");
	}
	else
	{
		printf("KEY1松开\r\n");
	}
	
	EXTI->PR|=1<<4;//清除中断线4中断请求
}

void EXTI9_5_IRQHandler(void)
{
	if(!KEY2)
	{
		printf("KEY2按下\r\n");
	}
	else
	{
		printf("KEY2松开\r\n");
	}
	EXTI->PR|=1<<5;//清除中断线5中断请求
}

