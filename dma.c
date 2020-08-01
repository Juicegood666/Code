#include "dma.h"
void DMA_CH4_Init(u32 cpar,u32 cmar)
{
	RCC->AHBENR|=1<<0;
	
	DMA1_Channel4->CPAR=cpar;//�����ַ
	DMA1_Channel4->CMAR=cmar;//��������ַ
	DMA1_Channel4->CCR&=~(1<<14);//���������洢��ģʽ
	DMA1_Channel4->CCR|=3<<12;//ͨ�����ȼ�
	DMA1_Channel4->CCR&=~(3<<10);//�洢�����ݿ��
	DMA1_Channel4->CCR&=~(3<<8);//�������ݿ��
	DMA1_Channel4->CCR|=1<<7;//�洢����ַ����
	DMA1_Channel4->CCR&=~(1<<6);//�����ַ����
	DMA1_Channel4->CCR&=~(1<<5);//ѭ��ģʽ
	DMA1_Channel4->CCR|=1<<4;//���ݴ��䷽��
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
	
	DMA1_Channel5->CPAR=cpar;//�����ַ
	DMA1_Channel5->CMAR=cmar;//��������ַ
	DMA1_Channel5->CCR&=~(1<<14);//���������洢��ģʽ
	DMA1_Channel5->CCR|=3<<12;//ͨ�����ȼ�
	DMA1_Channel5->CCR&=~(3<<10);//�洢�����ݿ��
	DMA1_Channel5->CCR&=~(3<<8);//�������ݿ��
	DMA1_Channel5->CCR|=1<<7;//�洢����ַ����
	DMA1_Channel5->CCR&=~(1<<6);//�����ַ����
	DMA1_Channel5->CCR&=~(1<<5);//ѭ��ģʽ
	DMA1_Channel5->CCR&=~(1<<4);//���ݴ��䷽��
}
void DMA_CH5_Start(u32 cndtr)
{
	DMA1_Channel5->CCR&=~(1<<0);
	DMA1_Channel5->CNDTR=cndtr;
	DMA1_Channel5->CCR|=1<<0;
}
