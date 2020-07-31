#include "key.h"
#include "delay.h"
/*
�������ƣ�void KEY_Init(void)
�������ܣ�������ʼ��
Ӳ�����ӣ�
					KEY_UP--PA0
          KEY0----PE4
          KEY1----PE3
          KEY2----PE2
*/
void KEY_Init(void)
{
	//��ʱ��
  RCC->APB2ENR|=1<<2;//GPIOAʱ��ʹ��
	RCC->APB2ENR|=1<<6;//GPIOEʱ��ʹ��
	//���üĴ���
	GPIOA->CRL&=0xFFFFFFF0;
	GPIOA->CRL|=0x00000008;
  GPIOE->CRL&=0xFFF000FF;
	GPIOE->CRL|=0x00088800;
	//����
	GPIOE->ODR|=0x7<<2;
}


/*
�������ƣ�u8 KEY_Scanf(void)
�������ܣ���ȡ��ֵ
�� �� ֵ��
					1 ����KEY_UP ����
					2 ����KEY_0 ����
					3 ����KEY_1 ����
					4 ����KEY_2 ����
					0	����û�а�������
*/
u8 KEY_Scanf(void)
{
	static u8 key_flag=0;//����״̬��ʶλ 0����û�а��� 1��������
	if((KEY_UP || !KEY0 || !KEY1 || !KEY2) && key_flag==0)
	{
		delay_ms(20);//������10ms~30ms��
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

