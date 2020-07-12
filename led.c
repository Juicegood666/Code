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
    //����GPIO��
    GPIOB->CRL&=0x00FFFFFF;
    GPIOB->CRL|=0x22000000;
    GPIOB->CRH&=0xFFFFFF00;
    GPIOB->CRH|=0x00000022;
    //����
    GPIOB->ODR|=1<<6;
    GPIOB->ODR|=1<<7;
    GPIOB->ODR|=1<<8;
    GPIOB->ODR|=1<<9;
}

/*
��������: ��������ʼ��
Ӳ������: PA6
����: �ߵ�ƽ��
*/
void BEEP_Init(void)
{
   RCC->APB2ENR|=1<<2;
   GPIOA->CRL&=0xF0FFFFFF;
   GPIOA->CRL|=0x02000000;
}

