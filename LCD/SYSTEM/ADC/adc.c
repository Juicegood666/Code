#include "adc.h"
/*
函数名称：void ADC1_Init(void)
函数功能：ADC1初始化
硬件链接：PB0  ADC1_IN8
*/
void ADC1_Init(void)
{
	//打开时钟
	RCC->APB2ENR|=1<<3;//GPIOB时钟使能
	RCC->APB2ENR|=1<<9;//ADC1时钟使能
  //ADC复位
	RCC->APB2RSTR|=1<<9;//ADC1复位使能
	RCC->APB2RSTR&=~(1<<9);//ADC1复位使能
	//GPIOB配置
	GPIOB->CRL&=0xFFFFFFF0;//模拟输入模式
	//ADC工作频率
	RCC->CFGR&=~(0x3<<14);//清空ADC分频
	RCC->CFGR|=0x2<<14;//分频系数为6  12MHZ
	//配置ADC
	ADC1->CR2|=1<<20;    //使用外部事件
	ADC1->CR2|=0x7<<17;  //选择外部事件为 SWSTART
	ADC1->SMPR2|=0x7<<25;//设置通道8的采样时间为 239.5周期
	
	ADC1->CR2|=1<<0;//开启AD转换
	ADC1->CR2|=1<<3;//复位校准
	while(ADC1->CR2 & 1<<3){}  //等待初始化校准结束
	ADC1->CR2|=1<<2;//开始校准
	while(ADC1->CR2 & 1<<2){} //等待校准完成
}


/*
函数名称：u16 ADC1_CHx(u8 ch)
函数功能：采集通道X数据
参		数：ch  通道
*/
u16 ADC1_CHx(u8 ch)
{
	ADC1->SQR3&=0xFFFFFFE0;
	ADC1->SQR3|=ch<<0;
	ADC1->CR2|=1<<22;//开启规则通道转换（开关）
	while(!(ADC1->SR & 1<<1)){}//等待AD转换完成
	return ADC1->DR;
}


/*
函数名称：void ADC1_Init_1(void)
函数功能：ADC1初始化
硬件链接：PB0  ADC1_IN8
*/
void ADC1_Init_1(void)
{
	//打开时钟
	RCC->APB2ENR|=1<<3;//GPIOB时钟使能
	RCC->APB2ENR|=1<<9;//ADC1时钟使能
  //ADC复位
	RCC->APB2RSTR|=1<<9;//ADC1复位使能
	RCC->APB2RSTR&=~(1<<9);//ADC1复位使能
	//GPIOB配置
	GPIOB->CRL&=0xFFFFFFF0;//模拟输入模式
	//ADC工作频率
	RCC->CFGR&=~(0x3<<14);//清空ADC分频
	RCC->CFGR|=0x2<<14;//分频系数为6  12MHZ
	//配置ADC
	ADC1->CR2|=1<<15;    //注入通道的外部触发转换模式
	ADC1->CR2|=0x7<<12;  //选择启动注入通道组转换的外部事件
	ADC1->SMPR2|=0x7<<25;//设置通道8的采样时间为 239.5周期
	
	ADC1->CR2|=1<<0;//开启AD转换
	ADC1->CR2|=1<<3;//复位校准
	while(ADC1->CR2 & 1<<3){}  //等待初始化校准结束
	ADC1->CR2|=1<<2;//开始校准
	while(ADC1->CR2 & 1<<2){} //等待校准完成
}


/*
函数名称：u16 ADC1_CHx_1(u8 ch)
函数功能：采集通道X数据（注入通道）
参		数：ch  通道
*/
u16 ADC1_CHx_1(u8 ch)
{
	ADC1->JSQR&=~(0x1F<<15); //清空原来的转换通道
	ADC1->JSQR|=ch<<15;     //设置当前的转换通道
	ADC1->CR2|=1<<21;      //开始转换注入通道（开关）
	while(!(ADC1->SR & 1<<1)){}//等待AD转换完成
	return ADC1->JDR1;//读取注入通道转换的数据
}
