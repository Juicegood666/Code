#include "timer.h"
/*
��������: ���ö�ʱ��1
��������: psc Ԥ��Ƶ��  arr��װ��ֵ
*/
void TIMER1_Init(u16 psc,u16 arr)
{
   /*1. ��ʱ��*/
   RCC->APB2ENR|=1<<11; //������ʱ��1��ʱ��
   RCC->APB2RSTR|=1<<11;//������ʱ��1��λʱ��
   RCC->APB2RSTR&=~(1<<11);//�رն�ʱ��1��λʱ��
   /*2. ���ú��ļĴ���*/
   TIM1->PSC=psc-1;
   TIM1->ARR=arr;
   TIM1->DIER|=1<<0; //���������ж�
   STM32_SetPriority(TIM1_UP_IRQn,1,1); //�����ж����ȼ�
   //TIM1->CR1|=1<<0; //������ʱ��1
}

/*
��������: ��ʱ��1�ĸ����жϷ�����
*/
void TIM1_UP_IRQHandler(void)
{
    if(TIM1->SR&1<<0)
    {
      TIM1->SR&=~(1<<0);
      USART1_RX_FLAG=1; //��ʾ�������
      TIM1->CR1&=~(1<<0); //�رն�ʱ��1 
    }
}

/*
��������: ���ö�ʱ��2
��������: psc Ԥ��Ƶ��  arr��װ��ֵ
*/
void TIMER2_Init(u16 psc,u16 arr)
{
   /*1. ��ʱ��*/
   RCC->APB1ENR|=1<<0; //������ʱ��2��ʱ��
   RCC->APB1RSTR|=1<<0;//������ʱ��2��λʱ��
   RCC->APB1RSTR&=~(1<<0);//�رն�ʱ��2��λʱ��
   /*2. ���ú��ļĴ���*/
   TIM2->PSC=psc-1;
   TIM2->ARR=arr;
   TIM2->DIER|=1<<0; //���������ж�
   STM32_SetPriority(TIM2_IRQn,1,1); //�����ж����ȼ�
   //TIM2->CR1|=1<<0; //������ʱ��2
}

/*
��������: ��ʱ��2�жϷ�����
*/
void TIM2_IRQHandler(void)
{
    if(TIM2->SR&1<<0)
    {
      TIM2->SR&=~(1<<0);
      USART2_RX_FLAG=1; //��ʾ�������
      TIM2->CR1&=~(1<<0); //�رն�ʱ��2
    }
}

/*
��������: ���ö�ʱ��3
��������: psc Ԥ��Ƶ��  arr��װ��ֵ
*/
void TIMER3_Init(u16 psc,u16 arr)
{
   /*1. ��ʱ��*/
   RCC->APB1ENR|=1<<1; //������ʱ��3��ʱ��
   RCC->APB1RSTR|=1<<1;//������ʱ��3��λʱ��
   RCC->APB1RSTR&=~(1<<1);//�رն�ʱ��3��λʱ��
   /*2. ���ú��ļĴ���*/
   TIM3->PSC=psc-1;
   TIM3->ARR=arr;
   TIM3->DIER|=1<<0; //���������ж�
   STM32_SetPriority(TIM3_IRQn,1,1); //�����ж����ȼ�
  // TIM3->CR1|=1<<0; //������ʱ��3
}

/*
��������: ��ʱ��3�жϷ�����
*/
void TIM3_IRQHandler(void)
{
    if(TIM3->SR&1<<0)
    {
      TIM3->SR&=~(1<<0);
      USART3_RX_FLAG=1; //��ʾ�������
      TIM3->CR1&=~(1<<0); //�رն�ʱ��3
    }
}

/*
��������: ���ö�ʱ��4
��������: psc Ԥ��Ƶ��  arr��װ��ֵ
*/
void TIMER4_Init(u16 psc,u16 arr)
{
   /*1. ��ʱ��*/
   RCC->APB1ENR|=1<<2; //������ʱ��4��ʱ��
   RCC->APB1RSTR|=1<<2;//������ʱ��4��λʱ��
   RCC->APB1RSTR&=~(1<<2);//�رն�ʱ��4��λʱ��
   /*2. ���ú��ļĴ���*/
   TIM4->PSC=psc-1;
   TIM4->ARR=arr;
   TIM4->DIER|=1<<0; //���������ж�
   STM32_SetPriority(TIM4_IRQn,1,1); //�����ж����ȼ�
   TIM4->CR1|=1<<0; //������ʱ��4
}

/*
��������: ��ʱ��4�жϷ�����
*/
void TIM4_IRQHandler(void)
{
    if(TIM4->SR&1<<0)
    {
      LED1=!LED1;
      BEEP=!BEEP;
      TIM4->SR&=~(1<<0);
    }
}
