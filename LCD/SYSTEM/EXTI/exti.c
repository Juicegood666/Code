#include "exti.h"
#include <stdio.h>
#include "delay.h"
#include "timer.h"
/*

�������ƣ�void EXTI_KEY_Init(void)
�������ܣ������ж�
Ӳ�����ӣ�
						S2 ->PA0
						S3 ->PA1
						S4 ->PA4
						S5 ->PA5
*/
void EXTI_KEY_Init(void)
{
	KEY_Init();
	EXTI->IMR|=1<<0;//�����ж���0���ж�����
	EXTI->IMR|=1<<1;//�����ж���1���ж�����
	EXTI->IMR|=1<<4;//�����ж���4���ж�����
	EXTI->IMR|=1<<5;//�����ж���5���ж�����
	EXTI->FTSR|=1<<0;//�����ж���0�½��ش�������
	EXTI->RTSR|=1<<0;//�����ж���0�������ش�������
	EXTI->FTSR|=1<<1;//�����ж���1�½��ش�������
	EXTI->RTSR|=1<<1;//�����ж���1�������ش�������
	EXTI->FTSR|=1<<4;//�����ж���4�½��ش�������
	EXTI->RTSR|=1<<4;//�����ж���4�������ش�������
	EXTI->FTSR|=1<<5;//�����ж���5�½��ش�������
	EXTI->RTSR|=1<<5;//�����ж���5�������ش�������
	RCC->APB2ENR|=1<<0;//AFIOʱ��ʹ��
	AFIO->EXTICR[0]&=~(0xF<< (0*4));//ѡ������ԴΪPA0
	AFIO->EXTICR[0]&=~(0xF<< (1*4));//ѡ������ԴΪPA1
	AFIO->EXTICR[1]&=~(0xF<< (0*4));//ѡ������ԴΪPA4
	AFIO->EXTICR[1]&=~(0xF<< (1*4));//ѡ������ԴΪPA5
	STM32_SetPriority(EXTI0_IRQn,1,1);
	STM32_SetPriority(EXTI1_IRQn,1,1);
	STM32_SetPriority(EXTI4_IRQn,1,1);
	STM32_SetPriority(EXTI9_5_IRQn,1,1);
}
/*
�������ƣ�void EVENT_Init(void)
�������ܣ��¼��ж�
*/
void EVENT_Init(void)
{
	EXTI->IMR|=1<<1;//�����ж���1���ж�����
	EXTI->EMR|=1<<1;//�����ж���1���¼�����
	STM32_SetPriority(EXTI1_IRQn,1,1);
}

void EXTI0_IRQHandler(void)
{
	EXTI->PR|=1<<0;//����ж���0�ж�����
}

void EXTI1_IRQHandler(void)
{
	EXTI->PR|=1<<1;//����ж���1�ж�����
}

void EXTI4_IRQHandler(void)
{
	EXTI->PR|=1<<4;//����ж���4�ж�����
}

//void EXTI9_5_IRQHandler(void)
//{
//	EXTI->PR|=1<<5;//����ж���5�ж�����
//}

