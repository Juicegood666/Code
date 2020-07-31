#include "timer.h"
#include "sys.h"
#include "beep.h"
#include "usart.h"

/*
函数名称：void TIM1_Init(u16 psc,u16 arr)
函数功能：定时器1初始化
参		数：
					arr  重装载值
					psc  预分频系数
*/
void TIM1_Init(u16 psc,u16 arr)
{
	RCC->APB2ENR|=1<<11;//TIM1时钟使能
	RCC->APB2RSTR|=1<<11;//TIM1复位使能
	RCC->APB2RSTR&=~(1<<11);//TIM1复位失能
	TIM1->DIER|=1<<0;//允许更新中断
	STM32_SetPriority(TIM1_UP_IRQn,1,1);//设置更新中断优先级
	TIM1->CNT=0;
	TIM1->ARR=arr;
	TIM1->PSC=psc-1;
	TIM1->CR1&=~(1<<0);//关闭定时器1
}

/*
函数名称: TIM1_UP_IRQHandler
函数功能: 定时器1的更新中断服务函数
*/
void TIM1_UP_IRQHandler(void)
{
  USART1_RX_FLAG=1; //表示串口1一帧数据接收完毕
  TIM1->SR=0;
  TIM1->CR1&=~(1<<0);//关闭定时器1
}

/*
函数名称：void TIM2_Init(u16 psc,u16 arr)
函数功能：定时器2初始化
参		数：
					arr  重装载值
					psc  预分频系数
*/
void TIM2_Init(u16 psc,u16 arr)
{
	RCC->APB1ENR|=1<<0;//TIM2时钟使能
	RCC->APB1RSTR|=1<<0;//TIM2复位使能
	RCC->APB1RSTR&=~(1<<0);//TIM2复位失能
	TIM2->DIER|=1<<0;//允许更新中断
	STM32_SetPriority(TIM2_IRQn,1,1);//设置更新中断优先级
	TIM2->CNT=0;
	TIM2->ARR=arr;
	TIM2->PSC=psc-1;
	TIM2->CR1&=~(1<<0);//关闭定时器2
}

/*
函数名称: TIM2_IRQHandler
函数功能: 定时器2的中断服务函数
*/
void TIM2_IRQHandler(void)
{
  USART2_RX_FLAG=1; //表示串口2一帧数据接收完毕
  TIM2->SR=0;
  TIM2->CR1&=~(1<<0);//关闭定时器2
}

/*
函数名称：void TIM3_Init(u16 psc,u16 arr)
函数功能：定时器3初始化
参		数：
					arr  重装载值
					psc  预分频系数
*/
void TIM3_Init(u16 psc,u16 arr)
{
	RCC->APB1ENR|=1<<1;//TIM3时钟使能
	RCC->APB1RSTR|=1<<1;//TIM3复位使能
	RCC->APB1RSTR&=~(1<<1);//TIM3复位失能
	TIM3->DIER|=1<<0;//允许更新中断
	STM32_SetPriority(TIM3_IRQn,1,1);//设置更新中断优先级
	TIM3->CNT=0;
	TIM3->ARR=arr;
	TIM3->PSC=psc-1;
	TIM3->CR1&=~(1<<0);//关闭定时器3
}

/*
函数名称: TIM3_IRQHandler
函数功能: 定时器3的中断服务函数
*/
void TIM3_IRQHandler(void)
{
  USART3_RX_FLAG=1; //表示串口3一帧数据接收完毕
  TIM3->SR=0;
  TIM3->CR1&=~(1<<0);//关闭定时器3
}


/*
函数名称：void TIM4_Init(u16 psc,u16 arr)
函数功能：定时器4初始化
参		数：
					arr  重装载值
					psc  预分频系数
*/
void TIM4_Init(u16 psc,u16 arr)
{
	RCC->APB1ENR|=1<<2;//TIM4时钟使能
	RCC->APB1RSTR|=1<<2;//TIM4复位使能
	RCC->APB1RSTR&=~(1<<2);//TIM4复位失能
#ifdef TIM4_IN_IRQ
	TIM4->DIER|=1<<0;//允许更新中断
	STM32_SetPriority(TIM4_IRQn,1,1);
#endif
	TIM4->CNT=0;
	TIM4->ARR=arr;
	TIM4->PSC=psc-1;
	TIM4->CR1|=1<<0;//使能定时器4
}


/*
void TIM4_PWM_Init(u16 psc,u16 arr,u16 cmp)
函数功能：定时器4输出PWM波
参		数：
					arr  重装载值
					psc  预分频系数
					cmp  比较值
硬件链接：
					OC1 PB6
					OC2 PB7
					OC3 PB8
					OC4 PB9
*/
void TIM4_PWM_Init(u16 psc,u16 arr,u16 cmp)
{
	//打开时钟
	RCC->APB1ENR|=1<<2;//TIM4时钟使能
	RCC->APB2ENR|=1<<3;//GPIOB时钟使能
	RCC->APB1RSTR|=1<<2;//TIM4复位使能
	RCC->APB1RSTR&=~(1<<2);//TIM4复位失能
	
	//GPIOB配置
	GPIOB->CRL&=0x00FFFFFF;
	GPIOB->CRL|=0xBB000000;
	GPIOB->CRH&=0xFFFFFF00;
	GPIOB->CRH|=0x000000BB;
	
	//定时器4配置
	TIM4->CNT=0;
	TIM4->ARR=arr;
	TIM4->PSC=psc-1;
	TIM4->CR1|=1<<7;//重装载预装载
	
//	//OC1配置
//	TIM4->CCMR1&=~(0x3<<0);//OC1通道为输出
//	TIM4->CCMR1|=0x6<<4;//PWM1模式 CNT<CCR1 有效
//	TIM4->CCMR1|=1<<3;//输出比较预装载使能
//	TIM4->CCER|=1<<1;//设置有效电平为低电平
//	TIM4->CCER|=1<<0;//开启OC1输出
//	TIM4->CCR1=cmp;
//	
//	//OC2配置
//	TIM4->CCMR1&=~(0x3<<8);//OC2通道为输出
//	TIM4->CCMR1|=0x6<<12;//PWM1模式 CNT<CCR1 有效
//	TIM4->CCMR1|=1<<11;//输出比较预装载使能
//	TIM4->CCER|=1<<5;//设置有效电平为低电平
//	TIM4->CCER|=1<<4;//开启OC1输出
//	TIM4->CCR2=cmp;
//	//OC3配置
//	TIM4->CCMR2&=~(0x3<<0);//OC3通道为输出
//	TIM4->CCMR2|=0x6<<4;//PWM1模式 CNT<CCR1 有效
//	TIM4->CCMR2|=1<<3;//输出比较预装载使能
//	TIM4->CCER|=1<<9;//设置有效电平为低电平
//	TIM4->CCER|=1<<8;//开启OC1输出
//	TIM4->CCR3=cmp;
	//OC4配置
	TIM4->CCMR2&=~(0x3<<8);//OC2通道为输出
	TIM4->CCMR2|=0x6<<12;//PWM1模式 CNT<CCR1 有效
	TIM4->CCMR2|=1<<11;//输出比较预装载使能
	TIM4->CCER|=1<<13;//设置有效电平为低电平
	TIM4->CCER|=1<<12;//开启OC1输出
	TIM4->CCR4=cmp;
//	TIM4->DIER|=1<<0;//允许更新中断
//	STM32_SetPriority(TIM4_IRQn,1,1);
  TIM4->CR1|=1<<0;//使能定时器4
}	

u32 key_time=0;
void TIM4_IRQHandler(void)
{
//	static u8 dir_flag=0;//0代表自加  1代表自减
	if(TIM4->SR & 1<<0)
	{
		//key_time++;
//		if(TIM4->CCR1==1000)dir_flag=1;
//		else if(TIM4->CCR1==0)dir_flag=0;
//		if(dir_flag==1)TIM4->CCR1--;
//		else if(dir_flag==0)TIM4->CCR1++;
		USART1_RX_FLAG=1;
		TIM4->CR1&=~(1<<0);//失能定时器4
	}
	TIM4->SR=0;
}

/*
函数功能：定时器5输入捕获
参		数：
					arr  重装载值
					psc  预分频系数
硬件链接：
				TIM5--CH2---PA1
*/
void TIM5_PWM_Input_Init(u16 psc,u16 arr)
{
	//打开时钟
	RCC->APB1ENR|=1<<3;//TIM5时钟使能
	RCC->APB2ENR|=1<<2;//GPIOA时钟使能
	RCC->APB1RSTR|=1<<3;//TIM5复位使能
	RCC->APB1RSTR&=~(1<<3);//TIM5复位失能
	
	GPIOA->CRL&=0xFFFFFF0F;
	GPIOA->CRL|=0x00000080;
	
	TIM5->CNT=0;
	TIM5->ARR=arr;
	TIM5->PSC=psc-1;
	
	TIM5->CCMR1|=0x1<<8; //CH2
	TIM5->CCER|=1<<4; //使能输入捕获
	TIM5->CCER|=1<<5; //1捕获下降沿 0捕获上升沿
	TIM5->DIER|=1<<0; //更新中断
	TIM5->DIER|=1<<2; //捕获中断
	STM32_SetPriority(TIM5_IRQn,1,1);
  TIM5->CR1|=1<<0;//使能定时器
}


/*
输入捕获的处理
*/
#define TIM5_CH2_INPUT PAin(1)

u32 TIM5_infraredDecodeData=0; //存放红外线解码的结果
u8 TIM5_infraredDecodeflag=0;  //解码成功的标志位
u8 TIM5_infraredDecodeflag_UP=0;//成功获取引导码的标志位

void TIM5_IRQHandler(void)
{
	u32 time=0;
	if(TIM5->SR&1<<0) //更新中断
	{
			if(TIM5_infraredDecodeflag_UP)
			{
					TIM5_infraredDecodeflag_UP=0;
					TIM5_infraredDecodeflag=1; //解码成功的标志位
			}
	}
	else if(TIM5->SR&1<<2) //捕获中断
	{
			TIM5->CNT=0;
			if(TIM5_CH2_INPUT) //判断当前引脚电平
			{
					TIM5->CCER|=1<<5; //1捕获下降沿 0捕获上升沿
			}
			else
			{
					TIM5->CCER&=~(1<<5); //1捕获下降沿 0捕获上升沿
					
					time=TIM5->CCR2; //读取高电平的持续时间
					
					
					//如果收到引导码就进行接收数据
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
									TIM5_infraredDecodeflag_UP=0; //清空引导码接收标志
							}
					}
					//判断引导码是否接收成功
					else if(time>=3000&& time <=5500) //4500us
					{
						 	TIM5_infraredDecodeflag_UP=1; //标志引导码获取成功	
					}
					else
					{
							TIM5_infraredDecodeflag_UP=0; //清空引导码接收标志
					}	
			}	
	}
	TIM5->SR=0;
}

