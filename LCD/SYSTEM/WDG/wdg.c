#include "wdg.h"
/*
��������: ��ʼ���������Ź�
��������:
        u16 pr  Ԥ��Ƶϵ��
        u16 rlr ��װ��ֵ(���0xFFF)
        
*/
void IWDG_Init(u16 pr,u16 rlr)
{
  IWDG->KR=0x5555; //�����д���¼Ĵ���
  IWDG->PR=pr;     //Ԥ��Ƶϵͳ
  IWDG->RLR=rlr;   //��װ��ֵ
  IWDG->KR=0xAAAA; //IWDG_RLR�е�ֵ�ͻᱻ���¼��ص�
                   //���������Ӷ�����������Ź���λ
  IWDG->KR=0xCCCC; //�������Ź�
}

/*
��������: ��ʼ���������Ź�
��������:
        u16 psc :��Ƶϵ��
        u16 w_arr:����ֵ--���ֵ0x7F
        u16 arr :��װ��ֵ--���ֵ0x7F
*/
u16 wwdg_arr;
void WWDG_Init(u16 psc,u16 w_arr,u16 arr)
{
    wwdg_arr=arr; //������װ��ֵ
    RCC->APB1ENR|=1<<11; //�������ڿ��Ź���ʱ��
    WWDG->CFR|=1<<9;     //��������ֵ�ﵽ40h���������ж�
    STM32_SetPriority(WWDG_IRQn,1,1); //�����ж����ȼ�
    WWDG->CFR|=psc<<7;   //���÷�Ƶϵ��
    WWDG->CFR|=w_arr<<0; //���ô���ֵ
    WWDG->CR|=arr<<0;    //������װ��ֵ
    WWDG->CR|=1<<7;      //�������Ź�  
}

/*
��������: ���ڿ��Ź�
����ʱ��ֵ����0x40ʱ��
*/
void WWDG_IRQHandler(void)
{
    WWDG->CR|=wwdg_arr;//ι��---���»ָ�����ֵ
    WWDG->SR=0;
}
