#include "sys.h"
/*
��������: �����ж����ȼ�
*/
void STM32_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority)
{
	uint32_t EncodePriority;
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);//�������ȼ�����
	EncodePriority = NVIC_EncodePriority(NVIC_PriorityGroup_2,PreemptPriority, SubPriority);//��ȡ���ȼ�����
	NVIC_SetPriority(IRQn, EncodePriority);//�������ȼ�
	NVIC_EnableIRQ(IRQn);//ʹ���ж���
}

/*
��������: �δ�ʱ����ʼ������
*/
void SysTickInit(void)
{
    SysTick->CTRL&=~(1<<2); //ѡ���ⲿʱ��Դ
    SysTick->LOAD=500*9000; //500ms�ж�һ��
    SysTick->VAL=0;         //��������ֵ���0
    SysTick->CTRL|=1<<1;    //�����ж�
    SysTick->CTRL|=1<<0;    //�����δ�ʱ��
}

/*
��������: ��д�δ�ʱ���жϷ�����
*/
void SysTick_Handler(void)
{
    BEEP=!BEEP;
}


