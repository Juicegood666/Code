#include "timer.h"
#include "sys.h"
#include "beep.h"
#include "usart.h"

/*
�������ƣ�void TIM1_Init(u16 psc,u16 arr)
�������ܣ���ʱ��1��ʼ��
��		����
					arr  ��װ��ֵ
					psc  Ԥ��Ƶϵ��
*/
void TIM1_Init(u16 psc,u16 arr)
{
	RCC->APB2ENR|=1<<11;//TIM1ʱ��ʹ��
	RCC->APB2RSTR|=1<<11;//TIM1��λʹ��
	RCC->APB2RSTR&=~(1<<11);//TIM1��λʧ��
	TIM1->DIER|=1<<0;//��������ж�
	STM32_SetPriority(TIM1_UP_IRQn,1,1);//���ø����ж����ȼ�
	TIM1->CNT=0;
	TIM1->ARR=arr;
	TIM1->PSC=psc-1;
	TIM1->CR1&=~(1<<0);//�رն�ʱ��1
}

/*
��������: TIM1_UP_IRQHandler
��������: ��ʱ��1�ĸ����жϷ�����
*/
void TIM1_UP_IRQHandler(void)
{
  USART1_RX_FLAG=1; //��ʾ����1һ֡���ݽ������
  TIM1->SR=0;
  TIM1->CR1&=~(1<<0);//�رն�ʱ��1
}

/*
�������ƣ�void TIM2_Init(u16 psc,u16 arr)
�������ܣ���ʱ��2��ʼ��
��		����
					arr  ��װ��ֵ
					psc  Ԥ��Ƶϵ��
*/
void TIM2_Init(u16 psc,u16 arr)
{
	RCC->APB1ENR|=1<<0;//TIM2ʱ��ʹ��
	RCC->APB1RSTR|=1<<0;//TIM2��λʹ��
	RCC->APB1RSTR&=~(1<<0);//TIM2��λʧ��
	TIM2->DIER|=1<<0;//��������ж�
	STM32_SetPriority(TIM2_IRQn,1,1);//���ø����ж����ȼ�
	TIM2->CNT=0;
	TIM2->ARR=arr;
	TIM2->PSC=psc-1;
	TIM2->CR1&=~(1<<0);//�رն�ʱ��2
}

/*
��������: TIM2_IRQHandler
��������: ��ʱ��2���жϷ�����
*/
void TIM2_IRQHandler(void)
{
  USART2_RX_FLAG=1; //��ʾ����2һ֡���ݽ������
  TIM2->SR=0;
  TIM2->CR1&=~(1<<0);//�رն�ʱ��2
}

/*
�������ƣ�void TIM3_Init(u16 psc,u16 arr)
�������ܣ���ʱ��3��ʼ��
��		����
					arr  ��װ��ֵ
					psc  Ԥ��Ƶϵ��
*/
void TIM3_Init(u16 psc,u16 arr)
{
	RCC->APB1ENR|=1<<1;//TIM3ʱ��ʹ��
	RCC->APB1RSTR|=1<<1;//TIM3��λʹ��
	RCC->APB1RSTR&=~(1<<1);//TIM3��λʧ��
	TIM3->DIER|=1<<0;//��������ж�
	STM32_SetPriority(TIM3_IRQn,1,1);//���ø����ж����ȼ�
	TIM3->CNT=0;
	TIM3->ARR=arr;
	TIM3->PSC=psc-1;
	TIM3->CR1&=~(1<<0);//�رն�ʱ��3
}

/*
��������: TIM3_IRQHandler
��������: ��ʱ��3���жϷ�����
*/
void TIM3_IRQHandler(void)
{
  USART3_RX_FLAG=1; //��ʾ����3һ֡���ݽ������
  TIM3->SR=0;
  TIM3->CR1&=~(1<<0);//�رն�ʱ��3
}


/*
�������ƣ�void TIM4_Init(u16 psc,u16 arr)
�������ܣ���ʱ��4��ʼ��
��		����
					arr  ��װ��ֵ
					psc  Ԥ��Ƶϵ��
*/
void TIM4_Init(u16 psc,u16 arr)
{
	RCC->APB1ENR|=1<<2;//TIM4ʱ��ʹ��
	RCC->APB1RSTR|=1<<2;//TIM4��λʹ��
	RCC->APB1RSTR&=~(1<<2);//TIM4��λʧ��
#ifdef TIM4_IN_IRQ
	TIM4->DIER|=1<<0;//��������ж�
	STM32_SetPriority(TIM4_IRQn,1,1);
#endif
	TIM4->CNT=0;
	TIM4->ARR=arr;
	TIM4->PSC=psc-1;
	TIM4->CR1|=1<<0;//ʹ�ܶ�ʱ��4
}


/*
void TIM4_PWM_Init(u16 psc,u16 arr,u16 cmp)
�������ܣ���ʱ��4���PWM��
��		����
					arr  ��װ��ֵ
					psc  Ԥ��Ƶϵ��
					cmp  �Ƚ�ֵ
Ӳ�����ӣ�
					OC1 PB6
					OC2 PB7
					OC3 PB8
					OC4 PB9
*/
void TIM4_PWM_Init(u16 psc,u16 arr,u16 cmp)
{
	//��ʱ��
	RCC->APB1ENR|=1<<2;//TIM4ʱ��ʹ��
	RCC->APB2ENR|=1<<3;//GPIOBʱ��ʹ��
	RCC->APB1RSTR|=1<<2;//TIM4��λʹ��
	RCC->APB1RSTR&=~(1<<2);//TIM4��λʧ��
	
	//GPIOB����
	GPIOB->CRL&=0x00FFFFFF;
	GPIOB->CRL|=0xBB000000;
	GPIOB->CRH&=0xFFFFFF00;
	GPIOB->CRH|=0x000000BB;
	
	//��ʱ��4����
	TIM4->CNT=0;
	TIM4->ARR=arr;
	TIM4->PSC=psc-1;
	TIM4->CR1|=1<<7;//��װ��Ԥװ��
	
//	//OC1����
//	TIM4->CCMR1&=~(0x3<<0);//OC1ͨ��Ϊ���
//	TIM4->CCMR1|=0x6<<4;//PWM1ģʽ CNT<CCR1 ��Ч
//	TIM4->CCMR1|=1<<3;//����Ƚ�Ԥװ��ʹ��
//	TIM4->CCER|=1<<1;//������Ч��ƽΪ�͵�ƽ
//	TIM4->CCER|=1<<0;//����OC1���
//	TIM4->CCR1=cmp;
//	
//	//OC2����
//	TIM4->CCMR1&=~(0x3<<8);//OC2ͨ��Ϊ���
//	TIM4->CCMR1|=0x6<<12;//PWM1ģʽ CNT<CCR1 ��Ч
//	TIM4->CCMR1|=1<<11;//����Ƚ�Ԥװ��ʹ��
//	TIM4->CCER|=1<<5;//������Ч��ƽΪ�͵�ƽ
//	TIM4->CCER|=1<<4;//����OC1���
//	TIM4->CCR2=cmp;
//	//OC3����
//	TIM4->CCMR2&=~(0x3<<0);//OC3ͨ��Ϊ���
//	TIM4->CCMR2|=0x6<<4;//PWM1ģʽ CNT<CCR1 ��Ч
//	TIM4->CCMR2|=1<<3;//����Ƚ�Ԥװ��ʹ��
//	TIM4->CCER|=1<<9;//������Ч��ƽΪ�͵�ƽ
//	TIM4->CCER|=1<<8;//����OC1���
//	TIM4->CCR3=cmp;
	//OC4����
	TIM4->CCMR2&=~(0x3<<8);//OC2ͨ��Ϊ���
	TIM4->CCMR2|=0x6<<12;//PWM1ģʽ CNT<CCR1 ��Ч
	TIM4->CCMR2|=1<<11;//����Ƚ�Ԥװ��ʹ��
	TIM4->CCER|=1<<13;//������Ч��ƽΪ�͵�ƽ
	TIM4->CCER|=1<<12;//����OC1���
	TIM4->CCR4=cmp;
//	TIM4->DIER|=1<<0;//��������ж�
//	STM32_SetPriority(TIM4_IRQn,1,1);
  TIM4->CR1|=1<<0;//ʹ�ܶ�ʱ��4
}	

u32 key_time=0;
void TIM4_IRQHandler(void)
{
//	static u8 dir_flag=0;//0�����Լ�  1�����Լ�
	if(TIM4->SR & 1<<0)
	{
		//key_time++;
//		if(TIM4->CCR1==1000)dir_flag=1;
//		else if(TIM4->CCR1==0)dir_flag=0;
//		if(dir_flag==1)TIM4->CCR1--;
//		else if(dir_flag==0)TIM4->CCR1++;
		USART1_RX_FLAG=1;
		TIM4->CR1&=~(1<<0);//ʧ�ܶ�ʱ��4
	}
	TIM4->SR=0;
}

/*
�������ܣ���ʱ��5���벶��
��		����
					arr  ��װ��ֵ
					psc  Ԥ��Ƶϵ��
Ӳ�����ӣ�
				TIM5--CH2---PA1
*/
void TIM5_PWM_Input_Init(u16 psc,u16 arr)
{
	//��ʱ��
	RCC->APB1ENR|=1<<3;//TIM5ʱ��ʹ��
	RCC->APB2ENR|=1<<2;//GPIOAʱ��ʹ��
	RCC->APB1RSTR|=1<<3;//TIM5��λʹ��
	RCC->APB1RSTR&=~(1<<3);//TIM5��λʧ��
	
	GPIOA->CRL&=0xFFFFFF0F;
	GPIOA->CRL|=0x00000080;
	
	TIM5->CNT=0;
	TIM5->ARR=arr;
	TIM5->PSC=psc-1;
	
	TIM5->CCMR1|=0x1<<8; //CH2
	TIM5->CCER|=1<<4; //ʹ�����벶��
	TIM5->CCER|=1<<5; //1�����½��� 0����������
	TIM5->DIER|=1<<0; //�����ж�
	TIM5->DIER|=1<<2; //�����ж�
	STM32_SetPriority(TIM5_IRQn,1,1);
  TIM5->CR1|=1<<0;//ʹ�ܶ�ʱ��
}


/*
���벶��Ĵ���
*/
#define TIM5_CH2_INPUT PAin(1)

u32 TIM5_infraredDecodeData=0; //��ź����߽���Ľ��
u8 TIM5_infraredDecodeflag=0;  //����ɹ��ı�־λ
u8 TIM5_infraredDecodeflag_UP=0;//�ɹ���ȡ������ı�־λ

void TIM5_IRQHandler(void)
{
	u32 time=0;
	if(TIM5->SR&1<<0) //�����ж�
	{
			if(TIM5_infraredDecodeflag_UP)
			{
					TIM5_infraredDecodeflag_UP=0;
					TIM5_infraredDecodeflag=1; //����ɹ��ı�־λ
			}
	}
	else if(TIM5->SR&1<<2) //�����ж�
	{
			TIM5->CNT=0;
			if(TIM5_CH2_INPUT) //�жϵ�ǰ���ŵ�ƽ
			{
					TIM5->CCER|=1<<5; //1�����½��� 0����������
			}
			else
			{
					TIM5->CCER&=~(1<<5); //1�����½��� 0����������
					
					time=TIM5->CCR2; //��ȡ�ߵ�ƽ�ĳ���ʱ��
					
					
					//����յ�������ͽ��н�������
					if(TIM5_infraredDecodeflag_UP)
					{
							if(time>1400&&time<1800) //1685us  1
							{
									TIM5_infraredDecodeData<<=1;
									TIM5_infraredDecodeData|=0x01;
							}
							else if(time>400&&time<700) //560us  0
							{
									TIM5_infraredDecodeData<<=1;
							}
							else
							{
									TIM5_infraredDecodeflag_UP=0; //�����������ձ�־
							}
					}
					//�ж��������Ƿ���ճɹ�
					else if(time>=3000&& time <=5500) //4500us
					{
						 	TIM5_infraredDecodeflag_UP=1; //��־�������ȡ�ɹ�	
					}
					else
					{
							TIM5_infraredDecodeflag_UP=0; //�����������ձ�־
					}	
			}	
	}
	TIM5->SR=0;
}

