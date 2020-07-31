#include "usart.h"

/*
函数名称：void USART1_Init(u32 baud)
函数功能：串口1初始化
硬件链接：
					TX->PA9
					RX->PA10
参		数：baud 波特率
*/
void USART1_Init(u32 baud)
{
	//打开时钟
	RCC->APB2ENR|=1<<2;
	RCC->APB2ENR|=1<<14;
  //配置GPIOA
	GPIOA->CRH&=0xFFFFF00F;
	GPIOA->CRH|=0x000008B0;
	//USART1复位
	RCC->APB2RSTR|=1<<14;//开启USART1复位
	RCC->APB2RSTR&=~(1<<14);//关闭USART1复位
	//配置串口
	USART1->BRR=72000000/baud;//设置波特比率
	USART1->CR1|=1<<5;//串口接收中断
	STM32_SetPriority(USART1_IRQn,0,2);
	USART1->CR1|=1<<2;//使能接收
	USART1->CR1|=1<<3;//使能发送
	USART1->CR1|=1<<13;//USART使能
}

/*
函数名称：void USART1_Init(u32 baud)
函数功能：串口初始化
硬件链接：
					TX1->PA9
					RX1->PA10
					TX2->PA2
					RX2->PA3
					TX3->PB10
					RX3->PB11
参		数：
					USARTx 串口选择
					psc  工作频率 单位M
					baud 波特率
*/
void USART_X_Init(USART_TypeDef *USARTx,u32 psc,u32 baud)
{
	if(USARTx==USART1)
	{
		//打开时钟
		RCC->APB2ENR|=1<<2;
		RCC->APB2ENR|=1<<14;
		//配置GPIOA
		GPIOA->CRH&=0xFFFFF00F;
		GPIOA->CRH|=0x000008B0;
		//USART1复位
		RCC->APB2RSTR|=1<<14;//开启USART1复位
		RCC->APB2RSTR&=~(1<<14);//关闭USART1复位
#ifdef USART1_IN_IRQ
		USARTx->CR1|=1<<5;//串口接收中断
		STM32_SetPriority(USART1_IRQn,0,2);
#endif
	}
	else if(USARTx==USART2)
	{
		//打开时钟
		RCC->APB2ENR|=1<<2;
		RCC->APB1ENR|=1<<17;
		//配置GPIOA
		GPIOA->CRL&=0xFFFF00FF;
		GPIOA->CRL|=0x00008B00;
		//USART1复位
		RCC->APB1RSTR|=1<<17;//开启USART2复位
		RCC->APB1RSTR&=~(1<<17);//关闭USART2复位
		USARTx->CR1|=1<<5;//串口接收中断
		STM32_SetPriority(USART2_IRQn,0,2);
	}
	else if(USARTx==USART3)
	{
		//打开时钟
		RCC->APB2ENR|=1<<3;
		RCC->APB1ENR|=1<<18;
		//配置GPIOA
		GPIOB->CRH&=0xFFFF00FF;
		GPIOB->CRH|=0x00008B00;
		//USART1复位
		RCC->APB1RSTR|=1<<18;//开启USART2复位
		RCC->APB1RSTR&=~(1<<18);//关闭USART2复位
		USARTx->CR1|=1<<5;//串口接收中断
		STM32_SetPriority(USART3_IRQn,0,2);
	}
	//配置串口
	USARTx->BRR=psc*1000000/baud;//设置波特比率
	USARTx->CR1|=1<<2;//使能接收
	USARTx->CR1|=1<<3;//使能发送
	USARTx->CR1|=1<<13;//USART使能
}

/*
函数名称：void USART1_SendString(u8 *str)
函数功能：串口1发送字符串
参		数：str 发送字符串的首地址
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
函数名称：void USART_X_SendString(USART_TypeDef *USARTx,u8 *str)
函数功能：串口发送字符串
参		数：str 发送字符串的首地址
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
函数名称：void USART_X_SendData(USART_TypeDef *USARTx,char *str)
函数功能：串口发送数据
参		数：data 发送数据的首地址
          u32 len 发送数据的长度
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
u8 USART1_RX_FLAG=0; //串口接收标识位  0代表没有接收完毕 1代表接收完毕
void USART1_IRQHandler(void)
{
	u8 c;
	if(USART1->SR & 1<<5)
	{
		c=USART1->DR;
		if(USART1_RX_FLAG==0) //判断原来收到的数据是否已经处理完毕
		{
			if(USART1_RX_CNT<USART1_RX_len) //判断当前缓冲区是否可以继续存放。
			{
				USART1_RX_BUFF[USART1_RX_CNT]=c; //向缓冲区里存放数据
				USART1_RX_CNT++; //缓冲区下标自增，方便下一次继续接收
				TIM1->CNT=0;     //清除CNT计数器
				TIM1->CR1|=1<<0; //使能定时器1
			}
			else
			{
				USART1_RX_FLAG=1; //强制置位
			}
		}
	}
}

u8 USART2_RX_BUFF[USART2_RX_len]; 
u8 USART2_RX_CNT=0; 
u8 USART2_RX_FLAG=0; //串口接收标识位  0代表没有接收完毕 1代表接收完毕
void USART2_IRQHandler(void)
{
	u8 c;
	if(USART2->SR & 1<<5)
	{
		c=USART2->DR;
		if(USART2_RX_FLAG==0) //判断原来收到的数据是否已经处理完毕
		{
			if(USART2_RX_CNT<USART2_RX_len) //判断当前缓冲区是否可以继续存放。
			{
				USART2_RX_BUFF[USART2_RX_CNT]=c; //向缓冲区里存放数据
				USART2_RX_CNT++; //缓冲区下标自增，方便下一次继续接收
				TIM2->CNT=0;     //清除CNT计数器
				TIM2->CR1|=1<<0; //使能定时器2
			}
			else
			{
				USART2_RX_FLAG=1; //强制置位
			}
		}
	}
}


u8 USART3_RX_BUFF[USART3_RX_len]; 
u16 USART3_RX_CNT=0; 
u8 USART3_RX_FLAG=0; //串口接收标识位  0代表没有接收完毕 1代表接收完毕
void USART3_IRQHandler(void)
{
	u8 c;
	if(USART3->SR & 1<<5)
	{
		c=USART3->DR;
		if(USART3_RX_FLAG==0) //判断原来收到的数据是否已经处理完毕
		{
			if(USART3_RX_CNT<USART3_RX_len) //判断当前缓冲区是否可以继续存放。
			{
				USART3_RX_BUFF[USART3_RX_CNT]=c; //向缓冲区里存放数据
				USART3_RX_CNT++; //缓冲区下标自增，方便下一次继续接收
				TIM3->CNT=0;     //清除CNT计数器
				TIM3->CR1|=1<<0; //使能定时器3
			}
			else
			{
				USART3_RX_FLAG=1; //强制置位
			}
		}
	}
}

