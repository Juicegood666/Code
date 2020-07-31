#include "led.h"

/*
�������ƣ�void LED_Init(void)
�������ܣ�LED��ʼ��
Ӳ�����ӣ�
					LED0 -> PB5
					LED1 -> PE5
					�͵�ƽ����
*/
void LED_Init(void)
{
	//��ʱ��
	RCC->APB2ENR|=1<<3;//GPIOBʱ��ʹ��
  RCC->APB2ENR|=1<<6;//GPIOEʱ��ʹ��
	//���üĴ���
	GPIOB->CRL&=0xFF0FFFFF;
	GPIOB->CRL|=0x00300000;
	GPIOE->CRL&=0xFF0FFFFF;
	GPIOE->CRL|=0x00300033;
	//����
	GPIOB->ODR|=1<<5;
  GPIOE->ODR|=1<<5;
}

