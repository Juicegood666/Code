#include "infrared.h"

//存放NEC红外线的解码数据
u8 infraredDecodeData[4];
u8 infraredFlag=0; //1表示解码成功

/*
函数功能: 红外线解码初始化
硬件连接: PB9
解码思路: 外部中断+定时器实现解码
(采用定时器6)
*/
void InfraredDecodeInit(void)
{
		/*1. 配置GPIO口*/
		RCC->APB2ENR|=1<<3; //PB
	  GPIOB->CRH&=0xFFFFFF0F;
	  GPIOB->CRH|=0x00000080;
	  GPIOB->ODR|=1<<9; 
	
	  /*2. 配置外部中断*/
		EXTI->IMR|=1<<9;  //开放来自线x上的中断请求
		EXTI->FTSR|=1<<9; //允许输入线x上的下降沿触发
		RCC->APB2ENR|=1<<0; //AFIO时钟
	  AFIO->EXTICR[2]&=~(0xF<<4); 
	  AFIO->EXTICR[2]|=0x1<<4;
	  STM32_SetPriority(EXTI9_5_IRQn,1,1);  //设置优先级
	
	  /*3. 配置定时器6*/
		RCC->APB1ENR|=1<<4;
	  RCC->APB1RSTR|=1<<4;
	  RCC->APB1RSTR&=~(1<<4);
	  TIM6->ARR=65535;
	  TIM6->PSC=72-1;
	  TIM6->CR1&=~(1<<0); //关闭定时器
}


/*
函数功能: 获取高电平的持续时间
*/
u32 Infrared_GetTimeH(void)
{
		TIM6->CNT=0;
	  TIM6->CR1|=1<<0;//开启定时器
	  while(INFRARED_INPUT){} //等待持续时间
		TIM6->CR1&=~(1<<0);
		return TIM6->CNT; //得到时间
}

/*
函数功能: 获取低电平的持续时间
*/
u32 Infrared_GetTimeL(void)
{
		TIM6->CNT=0;
	  TIM6->CR1|=1<<0;//开启定时器
	  while(!INFRARED_INPUT){} //等待持续时间
		TIM6->CR1&=~(1<<0);
		return TIM6->CNT; //得到时间
}


/*
函数功能: 中断服务函数
*/
void EXTI9_5_IRQHandler(void)
{
		u8 i,j;
	  u8 data=0;
	  u32 time=0;
		EXTI->PR|=1<<9; //清除中断标志位
	  /*1. 判断引导码*/
		time=Infrared_GetTimeL(); //标准时间:9ms低电平
	  if(time<7000||time>11000)return;
	  time=Infrared_GetTimeH(); //标准时间:4.5ms高电平
	  if(time<3000||time>5500)return;
	
	  /*2. 接收32位的数据*/
		for(i=0;i<4;i++)
		{
				for(j=0;j<8;j++)
				{
						time=Infrared_GetTimeL(); //标准时间:0.56ms低电平
						if(time<400||time>700)return;
					  time=Infrared_GetTimeH(); //标准时间:0.56ms和1.685ms
						if(time>400&&time<700) //收到0
						{
							data>>=1;
						}
						else if(time>1500&&time<1800) //收到1
						{
							data>>=1;
							data|=0x80;
						}
						else return;
				}
				infraredDecodeData[i]=data;
		}
		infraredFlag=1; //解码成功
}


/*
函数功能: 红外线编码初始化
硬件连接: PG11
编码思路: 采用延时函数实现38KHZ
*/
void InfraredCodingInit(void)
{
		RCC->APB2ENR|=1<<8; //PG
		GPIOG->CRH&=0xFFFF0FFF;
	  GPIOG->CRH|=0x00003000;
	  GPIOG->ODR|=1<<11;
}

/*
函数功能: 发送38KHZ的载波
函数参数：
				u32 time_us 持续的时间
				u8 flag 1表示发送38KHZ载波，0表示不发送
*/
void InfraredSend38KHZ(u32 time_us,u8 flag)
{
		u32 i;
		if(flag)
		{
			 //发送38KHZ载波
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
函数功能: NEC协议编码发送
函数参数: 
				u8 user 用户码
				u8 key  按键码
先发低位
按键反码+按键码+用户反码+用户码
*/
void InfraredNECSend(u8 user,u8 key)
{
	u32 i;
	 /*1. 组合发送的数据*/
	u32 data=((~key&0xFF)<<24)|((key&0xFF)<<16)|((~user&0xFF)<<8)|((user&0xFF)<<0);
		
	 /*2. 发送引导码*/
	InfraredSend38KHZ(9000,1);//发送38KHZ载波
	InfraredSend38KHZ(4500,0);//不发送
	
	/*3. 发送32位数据*/
	 for(i=0;i<32;i++)
	 {
			InfraredSend38KHZ(560,1); //间隔时间
			if(data&0x01)InfraredSend38KHZ(1685,0); //发送1
			else InfraredSend38KHZ(560,0); //发送0
		  data>>=1;
	 }
	 InfraredSend38KHZ(560,1); //间隔时间
}
