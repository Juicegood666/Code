#include "ds18b20.h"
void DS18B20_Init()
{
	RCC->APB2ENR|=1<<3;
	GPIOB->CRH&=0x0FFFFFFF;
	GPIOB->CRH|=0x30000000;
	GPIOB->ODR|=1<<15;
}
/*
函数功能: 发送复位脉冲检测DS18B20硬件--建立通信过程
返 回 值: 0表示成功  1表示失败  
*/
u8 DS18B20_Check(void)
{
	u8 i;
	DS18B20_Out_Mode();
	DS18B20_Out=0;
	delay_us(580);
	DS18B20_Out=1;
	DS18B20_In_Mode();
	for(i=0;i<100;i++)
	{
		if(DS18B20_In==0)break;
		delay_us(1);
	}
	if(i==100)return 9;
	for(i=0;i<250;i++)
	{
		if(DS18B20_In)break;
		delay_us(1);
	}
	if(i==250)return 8;
	return 0;
}
void DS18B20_Write_Byte(u8 data)
{
	u8 i;
	DS18B20_Out_Mode();
	for(i=0;i<8;i++)
	{
		if(data&0x01)
		{
			DS18B20_Out=0;
			delay_us(15);
			DS18B20_Out=1;
			delay_us(45);
			DS18B20_Out=1;
			delay_us(2);
		}
		else
		{
			DS18B20_Out=0;
			delay_us(15);
			DS18B20_Out=0;
			delay_us(45);
			DS18B20_Out=1;
			delay_us(2);
		}
		data>>=1;
	}
}
u8 DS18B20_Read_Byte(void)
{
	u8 i,data=0;
	for(i=0;i<8;i++)
	{
		DS18B20_Out_Mode();
		DS18B20_Out=0;
		delay_us(2);
		DS18B20_In_Mode();
		delay_us(8);
		data>>=1;
		if(DS18B20_In)data|=0x80;
		delay_us(50);
		DS18B20_Out=1;
		delay_us(2);
	}
	return data;
}
u16 DS18B20_Read_Temp(void)
{
	u8 temp_L,temp_H;
	u16 temp=0;
	if(DS18B20_Check())return 1;
	DS18B20_Write_Byte(0xCC);
	DS18B20_Write_Byte(0x44);
	if(DS18B20_Check())return 2;
	DS18B20_Write_Byte(0xCC);
	DS18B20_Write_Byte(0xBE);
	temp_L=DS18B20_Read_Byte();
	temp_H=DS18B20_Read_Byte();
	temp=temp_L|temp_H<<8;
	return temp;
		
}

