#include "ds18b20.h"
/*
��������: Ӳ����ʼ��--IO����
Ӳ������: PB15
*/
void DS18B20_Init(void)
{
    /*1. ��ʱ��*/
    RCC->APB2ENR|=1<<3; //PB
    /*2. ����GPIO��ģʽ*/
    GPIOB->CRH&=0x0FFFFFFF;
    GPIOB->CRH|=0x30000000;
    /*3. ����*/
    GPIOB->ODR|=1<<15;
}

/*
��������: ���͸�λ������DS18B20Ӳ��--����ͨ�Ź���
�� �� ֵ: 0��ʾ�ɹ�  1��ʾʧ��  
*/
u8 DS18B20_Check(void)
{
    u8 i;
    DS18B20_OUT_MODE(); //����IO��Ϊ���ģʽ
    DS18B20_OUT=0;      //����
    delay_us(580);       
    DS18B20_OUT=1;      //����
    
    DS18B20_IN_MODE();  //����IO��Ϊ����ģʽ
    for(i=0;i<100;i++)
    {
        if(DS18B20_IN==0)break;
        delay_us(1);
    }
    if(i==100)return 1;
    
    for(i=0;i<250;i++)
    {
       if(DS18B20_IN)break;
       delay_us(1); 
    }
    if(i==250)return 1;
    return 0;
}

/*
��������: DS18B20дһ���ֽ�����
*/
void DS18B20_WriteOnebyte(u8 cmd)
{
    u8 i;
    DS18B20_OUT_MODE(); //���ģʽ
    for(i=0;i<8;i++)
    {
        if(cmd&0x01) //����1
        {
            DS18B20_OUT=0;
            delay_us(15);
            DS18B20_OUT=1;
            delay_us(45);
            DS18B20_OUT=1;
            delay_us(2);
        }
        else //����0
        {
            DS18B20_OUT=0;
            delay_us(15);
            DS18B20_OUT=0;
            delay_us(45);
            DS18B20_OUT=1;
            delay_us(2);
        }
        cmd>>=1;
    }
}

/*
��������: DS18B20��һ���ֽ�����
*/
u8 DS18B20_ReadOnebyte(void)
{
    u8 i;
    u8 data=0;
    for(i=0;i<8;i++)
    {
        DS18B20_OUT_MODE(); //���ģʽ
        DS18B20_OUT=0;
        delay_us(2);
        DS18B20_IN_MODE();
        delay_us(8);
        data>>=1; //����1λ
        if(DS18B20_IN)data|=0x80;
        delay_us(50);
        DS18B20_OUT=1;
        delay_us(2);
    }
    return data;
}

/*
��������: ��ȡһ��DS18B20���¶�����
����ֵ: ��ȡ���¶����ݸߵ�λ
�����������ֻ��һ��DS18B20�¶ȴ����������
*/
u16 DS18B20_ReadTemp(void)
{
   u16 temp;
   u8 t_L,t_H;
   if(DS18B20_Check())return 1;
   DS18B20_WriteOnebyte(0xCC); //��Ծ ROM ָ�� --����֤���
   DS18B20_WriteOnebyte(0x44); //�����¶�ת��ָ��
    
   if(DS18B20_Check())return 2;
   DS18B20_WriteOnebyte(0xCC); //��Ծ ROM ָ�� --����֤���
   DS18B20_WriteOnebyte(0xBE); //��ȡRAM�������
   
   //��ȡ�¶�
   t_L=DS18B20_ReadOnebyte(); //���ֽ�
   t_H=DS18B20_ReadOnebyte(); //���ֽ�
   temp=t_H<<8|t_L;
   return temp; 
}

/*
��������: ��ȡһ��DS18B20���¶�����
����ֵ: ��ȡ���¶����ݸߵ�λ
�������ж��DS18B20�¶ȴ����������
*/
u16 DS18B20_ReadTemp_2(u8 *number)
{
   u16 temp;
   u8 t_L,t_H;
   u8 i;
   if(DS18B20_Check())return 1;
   DS18B20_WriteOnebyte(0x55); //��֤���
   for(i=0;i<8;i++)DS18B20_WriteOnebyte(number[i]); //�������к� 
   DS18B20_WriteOnebyte(0x44); //�����¶�ת��ָ��
    
   if(DS18B20_Check())return 2;
   DS18B20_WriteOnebyte(0x55); //��֤���
   for(i=0;i<8;i++)DS18B20_WriteOnebyte(number[i]); //�������к�
   DS18B20_WriteOnebyte(0xBE); //��ȡRAM�������
   
   //��ȡ�¶�
   t_L=DS18B20_ReadOnebyte(); //���ֽ�
   t_H=DS18B20_ReadOnebyte(); //���ֽ�
   temp=t_H<<8|t_L;
   return temp; 
}
