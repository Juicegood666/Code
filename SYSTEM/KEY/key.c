#include "key.h"
/*
��������:������ʼ��
Ӳ������:PA0 PA1 PA4 PA5
����: PA0 �߰���Ϊ�ߵ�ƽ---û���µ͵�ƽ
      ��������Ϊ�͵�ƽ---û���¸ߵ�ƽ
*/
void KEY_Init(void)
{
    //��ʱ��
    RCC->APB2ENR|=1<<2;
	RCC->APB2ENR|=1<<6;
    //����ģʽ
    GPIOA->CRL&=0xFFFFFFF0;
    GPIOA->CRL|=0x00000008;
	GPIOE->CRL&=0xFFF000FF;
    GPIOE->CRL|=0x00088800;
    //����
    GPIOE->ODR|=1<<2;
    GPIOE->ODR|=1<<3;
    GPIOE->ODR|=1<<4;
}

/*
��������:����ɨ�躯��
��������: mode=1��ʾʹ������ģʽ  mode=0ʹ�õ���ģʽ
����ֵ:  2 3 4 5 ��ʾ����İ�ť   0��ʾû�а���
*/
u8 KEY_Scan(u8 mode)
{
   static u8 flag=1; //��¼��һ�ΰ��µ�״̬ 
   if(mode)flag=1;
   if(flag&&(KEY_UP||KEY0==0||KEY1==0||KEY2==0))
   {
       flag=0;
       delay_ms(60);
       if(KEY_UP)return 1;
       if(KEY0==0)return 2;
       if(KEY1==0)return 3;
       if(KEY2==0)return 4;
   }
   else if(KEY_UP==0&&KEY0&&KEY1&&KEY2)
   {
       flag=1; 
   }
   return 0;
}

