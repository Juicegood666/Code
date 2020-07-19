#include "adc.h"
void ADCSW_Init(void)
{
	RCC->APB2ENR|=1<<3;//GPIOB
	RCC->APB2ENR|=1<<9;//ADC1
	RCC->APB1RSTR|=1<<3;//复位
	RCC->APB2RSTR&=~(1<<9);
	GPIOB->CRL&=0xFFFFFFF0;//模拟输入模式
	
	RCC->CFGR&=~(0x3<<14);//adc复位
	RCC->CFGR|=0x2<<14;
	
	ADC1->CR2|=1<<23;//启用温度传感器和VREFINT
	ADC1->CR2|=1<<20;//使用外部事件启动转换（规则）
	ADC1->CR2|=0x7<<17;//SWSTART(规则)
	ADC1->SMPR2|=0x7<<24;//通道8 239.5
	ADC1->SMPR1|=0x7<<18;//通道16 239.5
	
	ADC1->CR2|=1<<0;//开启ADC
	
	ADC1->CR2|=1<<3;
	while(ADC1->CR2&1<<3){}
	ADC1->CR2|=1<<2;
	while(ADC1->CR2&1<<2){}
}

u16 ADCSW_Get_Data(u8 data)
{
	ADC1->SQR3&=~(0x1F<<0);
	ADC1->SQR3|=data<<0;
	ADC1->CR2|=1<<22;//开启规则通道
	while(!(ADC1->SR&1<<1)){}
	ADC1->SR&=~(1<<1);
	return ADC1->DR;
	
}
void ADCJSW_Init(void)
{
	RCC->APB2ENR|=1<<3;//GPIOB
	RCC->APB2ENR|=1<<9;//ADC1
	RCC->APB1RSTR|=1<<3;//复位
	RCC->APB2RSTR&=~(1<<9);
	GPIOB->CRL&=0xFFFFFFF0;//模拟输入模式
	
	RCC->CFGR&=~(0x3<<14);//adc复位
	RCC->CFGR|=0x2<<14;
	
	ADC1->CR2|=1<<23;//启用温度传感器和VREFINT
	
	ADC1->CR2|=1<<15;//使用外部事件启动转换（注入）
	ADC1->CR2|=0X7<<12;//JSWSTART（注入）
	ADC1->SMPR2|=0x7<<24;//通道8 239.5
	ADC1->SMPR1|=0x7<<18;//通道16 239.5
	
	ADC1->CR2|=1<<0;//开启ADC
	
	ADC1->CR2|=1<<3;
	while(ADC1->CR2&1<<3){}
	ADC1->CR2|=1<<2;
	while(ADC1->CR2&1<<2){}
}
u16 ADCJSW_Get_Data(u8 data)
{
	ADC1->JSQR&=~(0x1F<<15);
	ADC1->JSQR|=data<<15;
	ADC1->CR2|=1<<21;//开启注入通道
	while(!(ADC1->SR&1<<2)){}
	ADC1->SR&=~(1<<2);
	return ADC1->JDR1;
}

