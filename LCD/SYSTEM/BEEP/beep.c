#include "beep.h"
/*
�������ƣ�void BEEP_Init(void)
�������ܣ���������ʼ��
Ӳ�����ӣ�PB8
*/
void BEEP_Init(void)
{
	RCC->APB2ENR|=1<<3;//GPIOBʱ��ʹ��
	GPIOB->CRH&=0xFFFFFFF0;
	GPIOB->CRH|=0x00000003;
}

