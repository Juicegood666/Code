#include "adc.h"
void ADCSW_Init(void)
{
	RCC->APB2ENR|=1<<3;//GPIOB
	RCC->APB2ENR|=1<<9;//ADC1
	RCC->APB1RSTR|=1<<3;//��λ
	RCC->APB2RSTR&=~(1<<9);
	GPIOB->CRL&=0xFFFFFFF0;//ģ������ģʽ
	
	RCC->CFGR&=~(0x3<<14);//adc��λ
	RCC->CFGR|=0x2<<14;
	
	ADC1->CR2|=1<<23;//�����¶ȴ�������VREFINT
	ADC1->CR2|=1<<20;//ʹ���ⲿ�¼�����ת��������
	ADC1->CR2|=0x7<<17;//SWSTART(����)
	ADC1->SMPR2|=0x7<<24;//ͨ��8 239.5
	ADC1->SMPR1|=0x7<<18;//ͨ��16 239.5
	
	ADC1->CR2|=1<<0;//����ADC
	
	ADC1->CR2|=1<<3;
	while(ADC1->CR2&1<<3){}
	ADC1->CR2|=1<<2;
	while(ADC1->CR2&1<<2){}
}

u16 ADCSW_Get_Data(u8 data)
{
	ADC1->SQR3&=~(0x1F<<0);
	ADC1->SQR3|=data<<0;
	ADC1->CR2|=1<<22;//��������ͨ��
	while(!(ADC1->SR&1<<1)){}
	ADC1->SR&=~(1<<1);
	return ADC1->DR;
	
}
void ADCJSW_Init(void)
{
	RCC->APB2ENR|=1<<3;//GPIOB
	RCC->APB2ENR|=1<<9;//ADC1
	RCC->APB1RSTR|=1<<3;//��λ
	RCC->APB2RSTR&=~(1<<9);
	GPIOB->CRL&=0xFFFFFFF0;//ģ������ģʽ
	
	RCC->CFGR&=~(0x3<<14);//adc��λ
	RCC->CFGR|=0x2<<14;
	
	ADC1->CR2|=1<<23;//�����¶ȴ�������VREFINT
	
	ADC1->CR2|=1<<15;//ʹ���ⲿ�¼�����ת����ע�룩
	ADC1->CR2|=0X7<<12;//JSWSTART��ע�룩
	ADC1->SMPR2|=0x7<<24;//ͨ��8 239.5
	ADC1->SMPR1|=0x7<<18;//ͨ��16 239.5
	
	ADC1->CR2|=1<<0;//����ADC
	
	ADC1->CR2|=1<<3;
	while(ADC1->CR2&1<<3){}
	ADC1->CR2|=1<<2;
	while(ADC1->CR2&1<<2){}
}
u16 ADCJSW_Get_Data(u8 data)
{
	ADC1->JSQR&=~(0x1F<<15);
	ADC1->JSQR|=data<<15;
	ADC1->CR2|=1<<21;//����ע��ͨ��
	while(!(ADC1->SR&1<<2)){}
	ADC1->SR&=~(1<<2);
	return ADC1->JDR1;
}

