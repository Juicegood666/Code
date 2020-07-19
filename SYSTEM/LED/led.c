#include "led.h"
/*
��������: LED��ʼ��
Ӳ������: PB6 PB7 PB8 PB9
����: �͵�ƽ����
*/
void LED_Init(void)
{
    //��ʱ��
    RCC->APB2ENR|=1<<3;
	RCC->APB2ENR|=1<<6;
    //����GPIO��
    GPIOB->CRL&=0xFF0FFFFF;
	GPIOB->CRL|=0x00300000;
	GPIOE->CRL&=0xFF0FFFFF;
	GPIOE->CRL|=0x00300000;
    //����
    GPIOB->ODR|=1<<5;
    GPIOE->ODR|=1<<5;
}

/*
��������: ��������ʼ��
Ӳ������: PA6
����: �ߵ�ƽ��
*/
void BEEP_Init(void)
{
   RCC->APB2ENR|=1<<2;
   GPIOB->CRH&=0xFFFFFFF0;
   GPIOB->CRH|=0x00000002;
}



