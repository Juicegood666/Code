#include "oled.h"
/*
函数功能:初始化OLED显示屏硬件
硬件连接:
D0--PB14--时钟线
D1--PB13--数据线
RES-PB12--复位脚
DC--PB1--命令数据选择脚
CS--PA7--片选
*/
void OLED_Init(void)
{
	RCC->APB2ENR|=1<<2;
	RCC->APB2ENR|=1<<3;
	GPIOA->CRL&=0x0fffffff;
	GPIOA->CRL|=0x30000000;
	GPIOB->CRL&=0xffffff0f;
	GPIOB->CRL|=0x00000030;
	GPIOB->CRH&=0xf000ffff;
	GPIOB->CRH|=0x03330000;
	OLED_SCK=1;
	OLED_CS=1;
	
	OLED_RES=1;
	delay_ms(1000);
	OLED_RES=0;
	delay_ms(1000);
	OLED_RES=1;
	write_i(0xAE,OLED_WRITE_CMD);
	
	write_i(0x00,OLED_WRITE_CMD);
	write_i(0x10,OLED_WRITE_CMD);
	
	write_i(0x40,OLED_WRITE_CMD);
	
	write_i(0xB0,OLED_WRITE_CMD);
	
	write_i(0x81,OLED_WRITE_CMD);
	write_i(0xCF,OLED_WRITE_CMD);
	
	write_i(0xA1,OLED_WRITE_CMD);
	
	write_i(0xA6,OLED_WRITE_CMD);
	
	write_i(0xA8,OLED_WRITE_CMD);
	write_i(0x3F,OLED_WRITE_CMD);
	
	write_i(0xC8,OLED_WRITE_CMD);
	
	write_i(0xD3,OLED_WRITE_CMD);
	write_i(0x00,OLED_WRITE_CMD);
	
	write_i(0xD5,OLED_WRITE_CMD);
	write_i(0x80,OLED_WRITE_CMD);
	
	write_i(0xD9,OLED_WRITE_CMD);
	write_i(0Xf1,OLED_WRITE_CMD);
	
	write_i(0xDA,OLED_WRITE_CMD);
	write_i(0x12,OLED_WRITE_CMD);
	
	write_i(0xdb,OLED_WRITE_CMD);
	write_i(0x30,OLED_WRITE_CMD);
	
	write_i(0x8d,OLED_WRITE_CMD);
	write_i(0x14,OLED_WRITE_CMD);
	write_i(0xAF,OLED_WRITE_CMD);
	
}
/*
函数功能:data为发送的数据，flag为1发数据，为0发命令

*/
void write_i(u8 data,u8 flag)
{
	u8 i;
	OLED_DC=flag;
	OLED_CS=0;
	for(i=0;i<8;i++)
	{
		OLED_SCK=0;
		if(data&0x80)
		{
			OLED_MOSI=1;
		}
		else
		{
			OLED_MOSI=0;
		}
		data<<=1;
		OLED_SCK=1;
	}
	OLED_CS=1;
}
void OLED_CLEAR(u8 data)
{
	u8 i,j;
	for(i=0;i<8;i++)
	{
		write_i(0xb0+i,OLED_WRITE_CMD);
		write_i(0x10,OLED_WRITE_CMD);
		write_i(0x00,OLED_WRITE_CMD);
		for(j=0;j<128;j++)
		{
			write_i(data,OLED_WRITE_DAT);
		}				
	}
}
void OLED_SETPOS(u8 x,u8 y)
{
	write_i(0xb0+y,OLED_WRITE_CMD);
	write_i(((x&0xf0)>>4)|0x10,OLED_WRITE_CMD);
	write_i((x&0x0f)|0x00,OLED_WRITE_CMD);
}
void OLED_DISPALY_DATA(u8 x,u8 y,u8 *p)
{
	u8 i,j;
	for(i=0;i<2;i++)
	{
		OLED_SETPOS(x,y+i);
		for(j=0;j<8;j++)
		{
			write_i(*p++,OLED_WRITE_DAT);
		}
	}
}
