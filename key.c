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
    //����ģʽ
    GPIOA->CRL&=0xFF00FF00;
    GPIOA->CRL|=0x00880088;
    //����
    GPIOA->ODR|=1<<1;
    GPIOA->ODR|=1<<4;
    GPIOA->ODR|=1<<5;
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

