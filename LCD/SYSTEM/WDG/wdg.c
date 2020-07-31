#include "wdg.h"
/*
函数功能: 初始化独立看门狗
函数参数:
        u16 pr  预分频系数
        u16 rlr 重装载值(最大0xFFF)
        
*/
void IWDG_Init(u16 pr,u16 rlr)
{
  IWDG->KR=0x5555; //允许改写以下寄存器
  IWDG->PR=pr;     //预分频系统
  IWDG->RLR=rlr;   //重装载值
  IWDG->KR=0xAAAA; //IWDG_RLR中的值就会被重新加载到
                   //计数器，从而避免产生看门狗复位
  IWDG->KR=0xCCCC; //启动看门狗
}

/*
函数功能: 初始化独立看门狗
函数参数:
        u16 psc :分频系数
        u16 w_arr:窗口值--最大值0x7F
        u16 arr :重装载值--最大值0x7F
*/
u16 wwdg_arr;
void WWDG_Init(u16 psc,u16 w_arr,u16 arr)
{
    wwdg_arr=arr; //保存重装载值
    RCC->APB1ENR|=1<<11; //开启窗口看门狗的时钟
    WWDG->CFR|=1<<9;     //当计数器值达到40h，即产生中断
    STM32_SetPriority(WWDG_IRQn,1,1); //设置中断优先级
    WWDG->CFR|=psc<<7;   //设置分频系数
    WWDG->CFR|=w_arr<<0; //设置窗口值
    WWDG->CR|=arr<<0;    //设置重装载值
    WWDG->CR|=1<<7;      //开启看门狗  
}

/*
函数功能: 窗口看门狗
当定时器值到达0x40时候
*/
void WWDG_IRQHandler(void)
{
    WWDG->CR|=wwdg_arr;//喂狗---重新恢复计数值
    WWDG->SR=0;
}
