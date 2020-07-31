#include "usart.h"

/*
�������ƣ�void USART1_Init(u32 baud)
�������ܣ�����1��ʼ��
Ӳ�����ӣ�
					TX->PA9
					RX->PA10
��		����baud ������
*/
void USART1_Init(u32 baud)
{
	//��ʱ��
	RCC->APB2ENR|=1<<2;
	RCC->APB2ENR|=1<<14;
  //����GPIOA
	GPIOA->CRH&=0xFFFFF00F;
	GPIOA->CRH|=0x000008B0;
	//USART1��λ
	RCC->APB2RSTR|=1<<14;//����USART1��λ
	RCC->APB2RSTR&=~(1<<14);//�ر�USART1��λ
	//���ô���
	USART1->BRR=72000000/baud;//���ò��ر���
	USART1->CR1|=1<<5;//���ڽ����ж�
	STM32_SetPriority(USART1_IRQn,0,2);
	USART1->CR1|=1<<2;//ʹ�ܽ���
	USART1->CR1|=1<<3;//ʹ�ܷ���
	USART1->CR1|=1<<13;//USARTʹ��
}

/*
�������ƣ�void USART1_Init(u32 baud)
�������ܣ����ڳ�ʼ��
Ӳ�����ӣ�
					TX1->PA9
					RX1->PA10
					TX2->PA2
					RX2->PA3
					TX3->PB10
					RX3->PB11
��		����
					USARTx ����ѡ��
					psc  ����Ƶ�� ��λM
					baud ������
*/
void USART_X_Init(USART_TypeDef *USARTx,u32 psc,u32 baud)
{
	if(USARTx==USART1)
	{
		//��ʱ��
		RCC->APB2ENR|=1<<2;
		RCC->APB2ENR|=1<<14;
		//����GPIOA
		GPIOA->CRH&=0xFFFFF00F;
		GPIOA->CRH|=0x000008B0;
		//USART1��λ
		RCC->APB2RSTR|=1<<14;//����USART1��λ
		RCC->APB2RSTR&=~(1<<14);//�ر�USART1��λ
#ifdef USART1_IN_IRQ
		USARTx->CR1|=1<<5;//���ڽ����ж�
		STM32_SetPriority(USART1_IRQn,0,2);
#endif
	}
	else if(USARTx==USART2)
	{
		//��ʱ��
		RCC->APB2ENR|=1<<2;
		RCC->APB1ENR|=1<<17;
		//����GPIOA
		GPIOA->CRL&=0xFFFF00FF;
		GPIOA->CRL|=0x00008B00;
		//USART1��λ
		RCC->APB1RSTR|=1<<17;//����USART2��λ
		RCC->APB1RSTR&=~(1<<17);//�ر�USART2��λ
		USARTx->CR1|=1<<5;//���ڽ����ж�
		STM32_SetPriority(USART2_IRQn,0,2);
	}
	else if(USARTx==USART3)
	{
		//��ʱ��
		RCC->APB2ENR|=1<<3;
		RCC->APB1ENR|=1<<18;
		//����GPIOA
		GPIOB->CRH&=0xFFFF00FF;
		GPIOB->CRH|=0x00008B00;
		//USART1��λ
		RCC->APB1RSTR|=1<<18;//����USART2��λ
		RCC->APB1RSTR&=~(1<<18);//�ر�USART2��λ
		USARTx->CR1|=1<<5;//���ڽ����ж�
		STM32_SetPriority(USART3_IRQn,0,2);
	}
	//���ô���
	USARTx->BRR=psc*1000000/baud;//���ò��ر���
	USARTx->CR1|=1<<2;//ʹ�ܽ���
	USARTx->CR1|=1<<3;//ʹ�ܷ���
	USARTx->CR1|=1<<13;//USARTʹ��
}

/*
�������ƣ�void USART1_SendString(u8 *str)
�������ܣ�����1�����ַ���
��		����str �����ַ������׵�ַ
*/
void USART1_SendString(u8 *str)
{
	while(*str!='\0')
	{
		USART1->DR=*str;
		while(!(USART1->SR & 1<<7)){}
		str++;
	}
}
/*
�������ƣ�void USART_X_SendString(USART_TypeDef *USARTx,u8 *str)
�������ܣ����ڷ����ַ���
��		����str �����ַ������׵�ַ
*/
void USART_X_SendString(USART_TypeDef *USARTx,char *str)
{
	while(*str!='\0')
	{
		USARTx->DR=*str;
		while(!(USARTx->SR & 1<<7)){}
		str++;
	}
}

/*
�������ƣ�void USART_X_SendData(USART_TypeDef *USARTx,char *str)
�������ܣ����ڷ�������
��		����data �������ݵ��׵�ַ
          u32 len �������ݵĳ���
*/
void USART_X_SendData(USART_TypeDef *USARTx,u8 *data,u32 len)
{
  u32 i;
  for(i=0;i<len;i++)
  {
     USARTx->DR=data[i];
		while(!(USARTx->SR & 1<<7)){} 
  }
}


int fputc(int c, FILE * stream)
{
	
		USART1->DR=c;
		while(!(USART1->SR & 1<<7)){}
		return c;
}

int fgetc(FILE * stream)
{
	while(!(USART1->SR & 1<<5)){}
	return USART1->DR;
}


u8 USART1_RX_BUFF[USART1_RX_len]; 
u8 USART1_RX_CNT=0; 
u8 USART1_RX_FLAG=0; //���ڽ��ձ�ʶλ  0����û�н������ 1����������
void USART1_IRQHandler(void)
{
	u8 c;
	if(USART1->SR & 1<<5)
	{
		c=USART1->DR;
		if(USART1_RX_FLAG==0) //�ж�ԭ���յ��������Ƿ��Ѿ��������
		{
			if(USART1_RX_CNT<USART1_RX_len) //�жϵ�ǰ�������Ƿ���Լ�����š�
			{
				USART1_RX_BUFF[USART1_RX_CNT]=c; //�򻺳�����������
				USART1_RX_CNT++; //�������±�������������һ�μ�������
				TIM1->CNT=0;     //���CNT������
				TIM1->CR1|=1<<0; //ʹ�ܶ�ʱ��1
			}
			else
			{
				USART1_RX_FLAG=1; //ǿ����λ
			}
		}
	}
}

u8 USART2_RX_BUFF[USART2_RX_len]; 
u8 USART2_RX_CNT=0; 
u8 USART2_RX_FLAG=0; //���ڽ��ձ�ʶλ  0����û�н������ 1����������
void USART2_IRQHandler(void)
{
	u8 c;
	if(USART2->SR & 1<<5)
	{
		c=USART2->DR;
		if(USART2_RX_FLAG==0) //�ж�ԭ���յ��������Ƿ��Ѿ��������
		{
			if(USART2_RX_CNT<USART2_RX_len) //�жϵ�ǰ�������Ƿ���Լ�����š�
			{
				USART2_RX_BUFF[USART2_RX_CNT]=c; //�򻺳�����������
				USART2_RX_CNT++; //�������±�������������һ�μ�������
				TIM2->CNT=0;     //���CNT������
				TIM2->CR1|=1<<0; //ʹ�ܶ�ʱ��2
			}
			else
			{
				USART2_RX_FLAG=1; //ǿ����λ
			}
		}
	}
}


u8 USART3_RX_BUFF[USART3_RX_len]; 
u16 USART3_RX_CNT=0; 
u8 USART3_RX_FLAG=0; //���ڽ��ձ�ʶλ  0����û�н������ 1����������
void USART3_IRQHandler(void)
{
	u8 c;
	if(USART3->SR & 1<<5)
	{
		c=USART3->DR;
		if(USART3_RX_FLAG==0) //�ж�ԭ���յ��������Ƿ��Ѿ��������
		{
			if(USART3_RX_CNT<USART3_RX_len) //�жϵ�ǰ�������Ƿ���Լ�����š�
			{
				USART3_RX_BUFF[USART3_RX_CNT]=c; //�򻺳�����������
				USART3_RX_CNT++; //�������±�������������һ�μ�������
				TIM3->CNT=0;     //���CNT������
				TIM3->CR1|=1<<0; //ʹ�ܶ�ʱ��3
			}
			else
			{
				USART3_RX_FLAG=1; //ǿ����λ
			}
		}
	}
}

