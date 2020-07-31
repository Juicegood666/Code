#include "adc.h"
/*
�������ƣ�void ADC1_Init(void)
�������ܣ�ADC1��ʼ��
Ӳ�����ӣ�PB0  ADC1_IN8
*/
void ADC1_Init(void)
{
	//��ʱ��
	RCC->APB2ENR|=1<<3;//GPIOBʱ��ʹ��
	RCC->APB2ENR|=1<<9;//ADC1ʱ��ʹ��
  //ADC��λ
	RCC->APB2RSTR|=1<<9;//ADC1��λʹ��
	RCC->APB2RSTR&=~(1<<9);//ADC1��λʹ��
	//GPIOB����
	GPIOB->CRL&=0xFFFFFFF0;//ģ������ģʽ
	//ADC����Ƶ��
	RCC->CFGR&=~(0x3<<14);//���ADC��Ƶ
	RCC->CFGR|=0x2<<14;//��Ƶϵ��Ϊ6  12MHZ
	//����ADC
	ADC1->CR2|=1<<20;    //ʹ���ⲿ�¼�
	ADC1->CR2|=0x7<<17;  //ѡ���ⲿ�¼�Ϊ SWSTART
	ADC1->SMPR2|=0x7<<25;//����ͨ��8�Ĳ���ʱ��Ϊ 239.5����
	
	ADC1->CR2|=1<<0;//����ADת��
	ADC1->CR2|=1<<3;//��λУ׼
	while(ADC1->CR2 & 1<<3){}  //�ȴ���ʼ��У׼����
	ADC1->CR2|=1<<2;//��ʼУ׼
	while(ADC1->CR2 & 1<<2){} //�ȴ�У׼���
}


/*
�������ƣ�u16 ADC1_CHx(u8 ch)
�������ܣ��ɼ�ͨ��X����
��		����ch  ͨ��
*/
u16 ADC1_CHx(u8 ch)
{
	ADC1->SQR3&=0xFFFFFFE0;
	ADC1->SQR3|=ch<<0;
	ADC1->CR2|=1<<22;//��������ͨ��ת�������أ�
	while(!(ADC1->SR & 1<<1)){}//�ȴ�ADת�����
	return ADC1->DR;
}


/*
�������ƣ�void ADC1_Init_1(void)
�������ܣ�ADC1��ʼ��
Ӳ�����ӣ�PB0  ADC1_IN8
*/
void ADC1_Init_1(void)
{
	//��ʱ��
	RCC->APB2ENR|=1<<3;//GPIOBʱ��ʹ��
	RCC->APB2ENR|=1<<9;//ADC1ʱ��ʹ��
  //ADC��λ
	RCC->APB2RSTR|=1<<9;//ADC1��λʹ��
	RCC->APB2RSTR&=~(1<<9);//ADC1��λʹ��
	//GPIOB����
	GPIOB->CRL&=0xFFFFFFF0;//ģ������ģʽ
	//ADC����Ƶ��
	RCC->CFGR&=~(0x3<<14);//���ADC��Ƶ
	RCC->CFGR|=0x2<<14;//��Ƶϵ��Ϊ6  12MHZ
	//����ADC
	ADC1->CR2|=1<<15;    //ע��ͨ�����ⲿ����ת��ģʽ
	ADC1->CR2|=0x7<<12;  //ѡ������ע��ͨ����ת�����ⲿ�¼�
	ADC1->SMPR2|=0x7<<25;//����ͨ��8�Ĳ���ʱ��Ϊ 239.5����
	
	ADC1->CR2|=1<<0;//����ADת��
	ADC1->CR2|=1<<3;//��λУ׼
	while(ADC1->CR2 & 1<<3){}  //�ȴ���ʼ��У׼����
	ADC1->CR2|=1<<2;//��ʼУ׼
	while(ADC1->CR2 & 1<<2){} //�ȴ�У׼���
}


/*
�������ƣ�u16 ADC1_CHx_1(u8 ch)
�������ܣ��ɼ�ͨ��X���ݣ�ע��ͨ����
��		����ch  ͨ��
*/
u16 ADC1_CHx_1(u8 ch)
{
	ADC1->JSQR&=~(0x1F<<15); //���ԭ����ת��ͨ��
	ADC1->JSQR|=ch<<15;     //���õ�ǰ��ת��ͨ��
	ADC1->CR2|=1<<21;      //��ʼת��ע��ͨ�������أ�
	while(!(ADC1->SR & 1<<1)){}//�ȴ�ADת�����
	return ADC1->JDR1;//��ȡע��ͨ��ת��������
}
