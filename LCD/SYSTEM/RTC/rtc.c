#include "rtc.h"
#include "sys.h"
#include <stdio.h>

RTC_TIME rtc_time;

/*
�������ƣ�void RTC_Init(void)
�������ܣ�RTC��ʼ��
*/
void RTC_Init(void)
{
	if(BKP->DR1!=0xab)
	{
		RCC->APB1ENR|=1<<28;//��Դ�ӿ�ʱ��ʹ��
		RCC->APB1ENR|=1<<27;//������ӿ�ʱ��ʹ��
		PWR->CR|=1<<8;//����Ա������RTC��д��
		//ѡ��ʱ��Դ
		RCC->BDCR|=1<<0;//�ⲿ32kHz��������
		while(!(RCC->BDCR & 1<<1)){}//�ȴ��ⲿ32kHz�����������
		RCC->BDCR&=~(0x3<<8);//���ʱ��Դѡ��
		RCC->BDCR|=0x1<<8;//ѡ��ʱ��Դѡ��
		RCC->BDCR|=1<<15;//RTCʱ��ʹ��
		//����RTC
		while(!(RTC->CRL& 1<<5)){}//�ȴ�RTC�������
		RTC->CRL|=1<<4;//��������ģʽ	
		RTC->CNTH=0;
		RTC->CNTL=0;//����ֵ��0
		RTC->ALRH=0;
		RTC->ALRL=100;//����ֵ
		RTC->PRLH=0;
		RTC->PRLL=0x7FFF;//32768��Ƶ  ʱ�ӹ���Ƶ��Ϊ1hz
		RTC->CRL&=~(1<<4);//�˳�����ģʽ	
		while(!(RTC->CRL& 1<<5)){}//�ȴ�RTC�������
		BKP->DR1=0xab;
	}
	RTC->CRH|=1<<1;//���������ж�
	RTC->CRH|=1<<0;//�������ж�
	STM32_SetPriority(RTC_IRQn,1,1);
}

void RTC_IRQHandler(void)
{
	u32 time;
	if(RTC->CRL & 1<<0)//���жϱ�־
	{
		time=RTC->CNTL;
		time|=RTC->CNTH<<16;
		GET_RTC_TIME(time);
		printf("%d-%d-%d %d:%d:%d\r\n",rtc_time.year,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min,rtc_time.sec);
		//printf("time=%d\r\n",time);
	}
	if(RTC->CRL & 1<<1)//�����жϱ�־
	{
		printf("���ӵ���\r\n");
	}
	RTC->CRL&=~(0X3<<0);//����жϱ�־λ
}

//����ֵ  0����ƽ��  1��������
u8 GET_TIME_STATE(u32 year)
{
	if((year % 4 ==0 && year % 100!=0) || year %400 ==0 )return 1;
	else return 0;
}

u8 mon_r[12]={31,29,31,30,31,30,31,31,30,31,30,31};
u8 mon_p[12]={31,28,31,30,31,30,31,31,30,31,30,31};

//������ת��
void SET_RTC_TIME(u32 year ,u32 mon,u32 day,u32 hour,u32 min,u32 sec)
{
	u32 i;
	u32 rtc_sec=0;
	for(i=2017;i<year;i++)
	{
		if(GET_TIME_STATE(i))
		{
			rtc_sec+=366*24*60*60;
		}
		else
		{
			rtc_sec+=365*24*60*60;
		}
	}
	for(i=0;i<mon-1;i++)
	{
		if(GET_TIME_STATE(year))
		{
			rtc_sec+=mon_r[i]*24*60*60;
		}
		else
		{
			rtc_sec+=mon_p[i]*24*60*60;
		}
	}
	rtc_sec+=(day-1)*24*60*60;
	rtc_sec+=hour*60*60;
	rtc_sec+=min*60;
	rtc_sec+=sec;
	
	RCC->APB1ENR|=1<<28;//��Դ�ӿ�ʱ��ʹ��
	RCC->APB1ENR|=1<<27;//������ӿ�ʱ��ʹ��
	PWR->CR|=1<<8;//����Ա������RTC��д��
	while(!(RTC->CRL& 1<<5)){}//�ȴ�RTC�������
	RTC->CRL|=1<<4;//��������ģʽ	
	RTC->CNTH=rtc_sec>>16;
	RTC->CNTL=rtc_sec&0xFFFF;//����ֵ��0
	RTC->CRL&=~(1<<4);//�˳�����ģʽ	
	while(!(RTC->CRL& 1<<5)){}//�ȴ�RTC�������
}
//��ת������
void GET_RTC_TIME(u32 sec)
{
	u32 i=0;
	rtc_time.year=2017;
	rtc_time.mon=1;
	rtc_time.day=1;
	rtc_time.hour=0;
	rtc_time.min=0;
	rtc_time.sec=0;
	
	while(1)
	{
		if(GET_TIME_STATE(rtc_time.year))
		{
			if(sec>=366*24*60*60)
			{
				sec-=366*24*60*60;
				rtc_time.year++;
			}
			else break;
		}
		else 
		{
			if(sec>=365*24*60*60)
			{
				sec-=365*24*60*60;
				rtc_time.year++;
			}
			else break;
		}
	}
	for(i=0;i<12;i++)
	{
		if(GET_TIME_STATE(rtc_time.year))
		{
			if(sec>=mon_r[i]*24*60*60)
			{
				sec-=mon_r[i]*24*60*60;
				rtc_time.mon++;
			}
			else break;
		}
		else
		{
			if(sec>=mon_p[i]*24*60*60)
			{
				sec-=mon_p[i]*24*60*60;
				rtc_time.mon++;
			}
			else break;
		}
	}
	while(1)
	{
		if(sec>=24*60*60)
		{
			sec-=24*60*60;
			rtc_time.day++;
		}
		else break;
	}
	while(1)
	{
		if(sec>=60*60)
		{
			sec-=60*60;
			rtc_time.hour++;
		}
		else break;
	}
	while(1)
	{
		if(sec>=60)
		{
			sec-=60;
			rtc_time.min++;
		}
		else break;
	}
	rtc_time.sec=sec;
	
}









