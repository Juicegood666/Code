#include "delay.h"

void delay_ms(u32 time)//1ms
{
	u32 i,j,k;
	for(i=0;i<time;i++)
		for(j=0;j<100;j++)
			for(k=0;k<100;k++);
}

void delay_us(u32 time)//1us
{
	u32 i,j;
	for(i=0;i<time;i++)
		for(j=0;j<10;j++);
}


void DelayMs(u32 time)
{
    u32 tmp;
    SysTick->LOAD=time*9000; //设置定时的时间
    SysTick->VAL=0;          //计数器的值清成0
    SysTick->CTRL|=1<<0;     //开启滴答定时器
    do
    {
      tmp= SysTick->CTRL; //读取寄存器的状态 
    }while(!(tmp&(1<<16))); //判断定时器定时是否到达
    SysTick->CTRL=0; //关闭滴答定时器
}

void DelayUs(u32 time)
{
    u32 tmp;
    SysTick->LOAD=time*9; //设置定时的时间
    SysTick->VAL=0;          //计数器的值清成0
    SysTick->CTRL|=1<<0;     //开启滴答定时器
    do
    {
      tmp= SysTick->CTRL; //读取寄存器的状态 
    }while(!(tmp&(1<<16))); //判断定时器定时是否到达
    SysTick->CTRL=0; //关闭滴答定时器
}
