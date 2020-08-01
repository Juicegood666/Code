#include "dma.h"
void DMA_CH4_Init(u32 cpar,u32 cmar)
{
	RCC->AHBENR|=1<<0;
	
	DMA1_Channel4->CPAR=cpar;//外设地址
	DMA1_Channel4->CMAR=cmar;//储存器地址
	DMA1_Channel4->CCR&=~(1<<14);//储存器到存储期模式
	DMA1_Channel4->CCR|=3<<12;//通道优先级
	DMA1_Channel4->CCR&=~(3<<10);//存储器数据宽度
	DMA1_Channel4->CCR&=~(3<<8);//外设数据宽度
	DMA1_Channel4->CCR|=1<<7;//存储器地址增量
	DMA1_Channel4->CCR&=~(1<<6);//外设地址增量
	DMA1_Channel4->CCR&=~(1<<5);//循环模式
	DMA1_Channel4->CCR|=1<<4;//数据传输方向
}
void DMA_CH4_Start(u32 cndtr)
{
	DMA1_Channel4->CCR&=~(1<<0);
	DMA1_Channel4->CNDTR=cndtr;
	DMA1_Channel4->CCR|=1<<0;
}

void DMA_CH5_Init(u32 cpar,u32 cmar)
{
	RCC->AHBENR|=1<<0;
	
	DMA1_Channel5->CPAR=cpar;//外设地址
	DMA1_Channel5->CMAR=cmar;//储存器地址
	DMA1_Channel5->CCR&=~(1<<14);//储存器到存储期模式
	DMA1_Channel5->CCR|=3<<12;//通道优先级
	DMA1_Channel5->CCR&=~(3<<10);//存储器数据宽度
	DMA1_Channel5->CCR&=~(3<<8);//外设数据宽度
	DMA1_Channel5->CCR|=1<<7;//存储器地址增量
	DMA1_Channel5->CCR&=~(1<<6);//外设地址增量
	DMA1_Channel5->CCR&=~(1<<5);//循环模式
	DMA1_Channel5->CCR&=~(1<<4);//数据传输方向
}
void DMA_CH5_Start(u32 cndtr)
{
	DMA1_Channel5->CCR&=~(1<<0);
	DMA1_Channel5->CNDTR=cndtr;
	DMA1_Channel5->CCR|=1<<0;
}
