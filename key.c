#include "key.h"
/*
函数功能:按键初始化
硬件连接:PA0 PA1 PA4 PA5
特性: PA0 高按下为高电平---没按下低电平
      其他按下为低电平---没按下高电平
*/
void KEY_Init(void)
{
    //开时钟
    RCC->APB2ENR|=1<<2;
    //配置模式
    GPIOA->CRL&=0xFF00FF00;
    GPIOA->CRL|=0x00880088;
    //上拉
    GPIOA->ODR|=1<<1;
    GPIOA->ODR|=1<<4;
    GPIOA->ODR|=1<<5;
}

/*
函数功能:函数扫描函数
函数参数: mode=1表示使用连续模式  mode=0使用单击模式
返回值:  2 3 4 5 表示具体的按钮   0表示没有按下
*/
u8 KEY_Scan(u8 mode)
{
   static u8 flag=1; //记录上一次按下的状态 
   if(mode)flag=1;
   if(flag&&(KEY_S2||KEY_S3==0||KEY_S4==0||KEY_S5==0))
   {
       flag=0;
       delay_ms(60);
       if(KEY_S2)return 2;
       if(KEY_S3==0)return 3;
       if(KEY_S4==0)return 4;
       if(KEY_S5==0)return 5;
   }
   else if(KEY_S2==0&&KEY_S3&&KEY_S4&&KEY_S5)
   {
       flag=1; 
   }
   return 0;
}

