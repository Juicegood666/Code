#include "key.h"
#include "delay.h"
/*
函数名称：void KEY_Init(void)
函数功能：按键初始化
硬件链接：
					KEY_UP--PA0
          KEY0----PE4
          KEY1----PE3
          KEY2----PE2
*/
void KEY_Init(void)
{
	//打开时钟
  RCC->APB2ENR|=1<<2;//GPIOA时钟使能
	RCC->APB2ENR|=1<<6;//GPIOE时钟使能
	//配置寄存器
	GPIOA->CRL&=0xFFFFFFF0;
	GPIOA->CRL|=0x00000008;
  GPIOE->CRL&=0xFFF000FF;
	GPIOE->CRL|=0x00088800;
	//上拉
	GPIOE->ODR|=0x7<<2;
}


/*
函数名称：u8 KEY_Scanf(void)
函数功能：获取键值
返 回 值：
					1 代表KEY_UP 按下
					2 代表KEY_0 按下
					3 代表KEY_1 按下
					4 代表KEY_2 按下
					0	代表没有按键按下
*/
u8 KEY_Scanf(void)
{
	static u8 key_flag=0;//按键状态标识位 0代表没有按下 1代表按下了
	if((KEY_UP || !KEY0 || !KEY1 || !KEY2) && key_flag==0)
	{
		delay_ms(20);//消抖（10ms~30ms）
		key_flag=1;
		if(KEY_UP)return 1;
		else if(!KEY0)return 2;
		else if(!KEY1)return 3;
		else if(!KEY2)return 4;
	}
	else
	{
		if(!KEY_UP && KEY0 && KEY1 && KEY2) key_flag=0;
	}
	return 0;
}

