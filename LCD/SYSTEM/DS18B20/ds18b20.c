#include "ds18b20.h"
#include <stdio.h>

/*
�������ƣ�void DS18B20_Init(void)
�������ܣ�DS18B20��ʼ��
Ӳ�����ӣ�DQ->PB15
*/
void DS18B20_Init(void)
{
	RCC->APB2ENR|=1<<3;//GPIOBʱ��ʹ��
	GPIOB->CRH&=0x0FFFFFFF;
	GPIOB->CRH|=0x30000000;
	GPIOB->ODR|=1<<15;//����
}


/*
�������ƣ�void DS18B20_Reset(void)
�������ܣ����͸�λ�ź�
*/
void DS18B20_Reset(void)
{
	DS18B20_OUTPUT_MODE(); //GPIO�ڴ������ģʽ
	DS18B20_Out=0; //��������
	delay_us(750); //�ȴ�ʱ�� ��׼480us
	DS18B20_Out=1; //�ͷ����� 
	delay_us(15); 
}

/*
�������ƣ�void DS18B20_Reset(void)
�������ܣ�����������
�� �� ֵ��0�������  1��������
*/
u8 DS18B20_Check(void)
{
	u32 i=0;
	DS18B20_INPUT_MODE(); //��DS18B20��ʼ��Ϊ����ģʽ
	while(DS18B20_In && i<80) //��׼ʱ��: 15us ~ 60us
	{
		i++;
		delay_us(1);
	}
	if(i>=60)return 1; //˵��DS18B20Ӳ��������
	i=0;
  
  //�ȴ�DS18B20�ͷ�����
	while(!DS18B20_In && i<240) //��׼ʱ��: 60us ~ 240us 
	{
		i++;
		delay_us(1);
	}
	if(i>=240)return 1; //˵��DS18B20Ӳ��������
	return 0;
}


/*
�������ƣ�void DS18B20_Reset(void)
�������ܣ���������
*/
void DS18B20_WriteByte(u8 cmd)
{
	u8 i=0;
	DS18B20_OUTPUT_MODE(); //����GPIO��Ϊ���ģʽ
	for(i=0;i<8;i++)
	{
		DS18B20_Out = 0;    //����������
		delay_us(2);        //����1us����
		DS18B20_Out = cmd & 0x01; //����ʵ�����ݽ��з���
		delay_us(60);       //�ȴ�ʱ��(DS18B20������ʱ��)
		DS18B20_Out=1;      //�ͷ�����
		cmd>>=1;
	}
}

/*
�������ƣ�u8 DS18B20_ReadByte(void)
�������ܣ���ȡ����
�� �� ֵ������������
*/
u8 DS18B20_ReadByte(void)
{
	u8 i,data=0;	
	for(i=0;i<8;i++)
	{
		DS18B20_OUTPUT_MODE(); //��ʼ��Ϊ���ģʽ 
		DS18B20_Out=0; //����������
		delay_us(2);   //��ʱʱ�� >1us
		DS18B20_INPUT_MODE(); //�ٳ�ʼ��Ϊ����ģʽ
		delay_us(8);          //���ʱ��
		data>>=1;
		if(DS18B20_In)data|=0x80;  //�����ߵ�ƽ���в���
		delay_us(60);	             //���ʱ��
		DS18B20_Out=1;             //�ͷ�����
	}
	return data;
}

/*
�������ƣ�u16 DS18B20_Read_Temp(void)
�������ܣ���ȡ�¶� (����DS18B20���)
�� �� ֵ���¶�ֵ
*/
u16 DS18B20_Read_Temp(void)
{
	u8 temp_H,temp_L;
	u16 temp;
	DS18B20_Reset(); //���͸�λ�ź�
	if(DS18B20_Check())printf("���ʧ��\r\n"); //���Ӧ��
	DS18B20_WriteByte(0xcc); //����ROM���(64λ���)
	DS18B20_WriteByte(0x44); //��ʼ�¶ȼ��
	DS18B20_Reset();         //���͸�λ�ź�
	if(DS18B20_Check())printf("���ʧ��\r\n"); //���Ӧ��
	DS18B20_WriteByte(0xcc); //����ROM���(64λ���)
	DS18B20_WriteByte(0xbe); //��ȡ�ݴ���(�¶�����ǰ2���ֽ�)
	temp_L=DS18B20_ReadByte();//��ȡ�¶ȵ�λ
	temp_H=DS18B20_ReadByte();//��ȡ�¶ȸ�λ
	temp=(temp_H<<8)|temp_L;  //�ϳ��¶�
	return temp; //�����¶�����
}


/*
��������: ��ȡDS18B20 64λ���
*/
void DS18B20_ReadRom(u8 *ds18b20_rom)
{
  u8 i;
  DS18B20_Reset(); //���͸�λ�ź�
  DS18B20_Check(); //����������
  DS18B20_WriteByte(0x33); //��ȡ64λ��ROM���
  
  for(i=0;i<8;i++)
  {
     ds18b20_rom[i]=DS18B20_ReadByte();
  }
  
  for(i=0;i<8;i++)
  {
     printf("%d ",ds18b20_rom[i]);
    //40 6 235 80 10 0 0 180 
  }
  printf("\n");
}

/*
�������ƣ�u16 DS18B20_Read_Temp(void)
�������ܣ���ȡ�¶� (���DS18B20���)
�� �� ֵ���¶�ֵ
*/
u16 DS18B20_Read_Temp_2(u8 *ds18b20_rom)
{
	u8 temp_H,temp_L;
	u16 temp;
  u8 i;
	DS18B20_Reset(); //���͸�λ�ź�
	if(DS18B20_Check())printf("���ʧ��\r\n"); //���Ӧ��
	DS18B20_WriteByte(0x55); //����ROMƥ��(64λ���)
  for(i=0;i<8;i++)DS18B20_WriteByte(ds18b20_rom[i]); //����ROM����
	
  DS18B20_WriteByte(0x44); //��ʼ�¶ȼ��
	DS18B20_Reset();         //���͸�λ�ź�
	if(DS18B20_Check())printf("���ʧ��\r\n"); //���Ӧ��
  DS18B20_WriteByte(0x55); //����ROMƥ��(64λ���)
  for(i=0;i<8;i++)DS18B20_WriteByte(ds18b20_rom[i]); //����ROM����
	DS18B20_WriteByte(0xbe); //��ȡ�ݴ���(�¶�����ǰ2���ֽ�)
	temp_L=DS18B20_ReadByte();//��ȡ�¶ȵ�λ
	temp_H=DS18B20_ReadByte();//��ȡ�¶ȸ�λ
	temp=(temp_H<<8)|temp_L;  //�ϳ��¶�
	return temp; //�����¶�����
}

