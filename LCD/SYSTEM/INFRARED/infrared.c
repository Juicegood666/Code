#include "infrared.h"

//���NEC�����ߵĽ�������
u8 infraredDecodeData[4];
u8 infraredFlag=0; //1��ʾ����ɹ�

/*
��������: �����߽����ʼ��
Ӳ������: PB9
����˼·: �ⲿ�ж�+��ʱ��ʵ�ֽ���
(���ö�ʱ��6)
*/
void InfraredDecodeInit(void)
{
		/*1. ����GPIO��*/
		RCC->APB2ENR|=1<<3; //PB
	  GPIOB->CRH&=0xFFFFFF0F;
	  GPIOB->CRH|=0x00000080;
	  GPIOB->ODR|=1<<9; 
	
	  /*2. �����ⲿ�ж�*/
		EXTI->IMR|=1<<9;  //����������x�ϵ��ж�����
		EXTI->FTSR|=1<<9; //����������x�ϵ��½��ش���
		RCC->APB2ENR|=1<<0; //AFIOʱ��
	  AFIO->EXTICR[2]&=~(0xF<<4); 
	  AFIO->EXTICR[2]|=0x1<<4;
	  STM32_SetPriority(EXTI9_5_IRQn,1,1);  //�������ȼ�
	
	  /*3. ���ö�ʱ��6*/
		RCC->APB1ENR|=1<<4;
	  RCC->APB1RSTR|=1<<4;
	  RCC->APB1RSTR&=~(1<<4);
	  TIM6->ARR=65535;
	  TIM6->PSC=72-1;
	  TIM6->CR1&=~(1<<0); //�رն�ʱ��
}


/*
��������: ��ȡ�ߵ�ƽ�ĳ���ʱ��
*/
u32 Infrared_GetTimeH(void)
{
		TIM6->CNT=0;
	  TIM6->CR1|=1<<0;//������ʱ��
	  while(INFRARED_INPUT){} //�ȴ�����ʱ��
		TIM6->CR1&=~(1<<0);
		return TIM6->CNT; //�õ�ʱ��
}

/*
��������: ��ȡ�͵�ƽ�ĳ���ʱ��
*/
u32 Infrared_GetTimeL(void)
{
		TIM6->CNT=0;
	  TIM6->CR1|=1<<0;//������ʱ��
	  while(!INFRARED_INPUT){} //�ȴ�����ʱ��
		TIM6->CR1&=~(1<<0);
		return TIM6->CNT; //�õ�ʱ��
}


/*
��������: �жϷ�����
*/
void EXTI9_5_IRQHandler(void)
{
		u8 i,j;
	  u8 data=0;
	  u32 time=0;
		EXTI->PR|=1<<9; //����жϱ�־λ
	  /*1. �ж�������*/
		time=Infrared_GetTimeL(); //��׼ʱ��:9ms�͵�ƽ
	  if(time<7000||time>11000)return;
	  time=Infrared_GetTimeH(); //��׼ʱ��:4.5ms�ߵ�ƽ
	  if(time<3000||time>5500)return;
	
	  /*2. ����32λ������*/
		for(i=0;i<4;i++)
		{
				for(j=0;j<8;j++)
				{
						time=Infrared_GetTimeL(); //��׼ʱ��:0.56ms�͵�ƽ
						if(time<400||time>700)return;
					  time=Infrared_GetTimeH(); //��׼ʱ��:0.56ms��1.685ms
						if(time>400&&time<700) //�յ�0
						{
							data>>=1;
						}
						else if(time>1500&&time<1800) //�յ�1
						{
							data>>=1;
							data|=0x80;
						}
						else return;
				}
				infraredDecodeData[i]=data;
		}
		infraredFlag=1; //����ɹ�
}


/*
��������: �����߱����ʼ��
Ӳ������: PG11
����˼·: ������ʱ����ʵ��38KHZ
*/
void InfraredCodingInit(void)
{
		RCC->APB2ENR|=1<<8; //PG
		GPIOG->CRH&=0xFFFF0FFF;
	  GPIOG->CRH|=0x00003000;
	  GPIOG->ODR|=1<<11;
}

/*
��������: ����38KHZ���ز�
����������
				u32 time_us ������ʱ��
				u8 flag 1��ʾ����38KHZ�ز���0��ʾ������
*/
void InfraredSend38KHZ(u32 time_us,u8 flag)
{
		u32 i;
		if(flag)
		{
			 //����38KHZ�ز�
				TIM4->CCR4=13;
			  DelayUs(time_us);
				TIM4->CCR4=0;
		}
		else
		{
				TIM4->CCR4=0;
				DelayUs(time_us);
		}
}

/*
��������: NECЭ����뷢��
��������: 
				u8 user �û���
				u8 key  ������
�ȷ���λ
��������+������+�û�����+�û���
*/
void InfraredNECSend(u8 user,u8 key)
{
	u32 i;
	 /*1. ��Ϸ��͵�����*/
	u32 data=((~key&0xFF)<<24)|((key&0xFF)<<16)|((~user&0xFF)<<8)|((user&0xFF)<<0);
		
	 /*2. ����������*/
	InfraredSend38KHZ(9000,1);//����38KHZ�ز�
	InfraredSend38KHZ(4500,0);//������
	
	/*3. ����32λ����*/
	 for(i=0;i<32;i++)
	 {
			InfraredSend38KHZ(560,1); //���ʱ��
			if(data&0x01)InfraredSend38KHZ(1685,0); //����1
			else InfraredSend38KHZ(560,0); //����0
		  data>>=1;
	 }
	 InfraredSend38KHZ(560,1); //���ʱ��
}
