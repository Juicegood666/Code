#include "at24cxx.h"
u8 AT24CXX_Write(u8 addr,u8 data)
{
	IIC_Start();
	IIC_Write_Byte(Write_data);
	if(IIC_Check_Ack())return 1;
	IIC_Write_Byte(addr);
	if(IIC_Check_Ack())return 2;
	IIC_Write_Byte(data);
	if(IIC_Check_Ack())return 3;
	IIC_Stop();
	DelayMs(10);
	return 0;
}

u8 AT24CXX_Read(u8 addr)
{
	u8 data=0;
	IIC_Start();
	IIC_Write_Byte(Write_data);
	if(IIC_Check_Ack())return 0xff;
	IIC_Write_Byte(addr);
	if(IIC_Check_Ack())return 0xff;
	IIC_Start();
	IIC_Write_Byte(Read_data);
	if(IIC_Check_Ack())return 0xff;
	data=IIC_Read_Byte();
	IIC_Send_Ack(1);
	IIC_Stop();
	return data;
}
u8 AT24CXX_Read_More(u8 addr,u8 *buff,u8 len)
{
	u8 i=0;
	IIC_Start();
	IIC_Write_Byte(Write_data);
	if(IIC_Check_Ack())return 0xff;
	IIC_Write_Byte(addr);
	if(IIC_Check_Ack())return 0xff;
	IIC_Start();
	IIC_Write_Byte(Read_data);
	if(IIC_Check_Ack())return 0xff;
	IIC_Start();
	IIC_Write_Byte(Read_data);
	if(IIC_Check_Ack())return 0xff;
	for(i=0;i<len;i++)
	{
		buff[i]=IIC_Read_Byte();
		if(i==len-1)IIC_Send_Ack(1);
		else IIC_Send_Ack(0);
	}
	IIC_Stop();
	return 0;
}

u8 AT24CXX_Write_Page(u8 addr,u8 *data,u8 len)
{
	u8 i=0;
	IIC_Start();
	IIC_Write_Byte(Write_data);
	if(IIC_Check_Ack())return 1;
	IIC_Write_Byte(addr);
	if(IIC_Check_Ack())return 2;
	for(i=0;i<len;i++)
	{
		IIC_Write_Byte(data[i]);
		if(IIC_Check_Ack())return 3;
	}
	IIC_Stop();
	DelayMs(10);
	return 0;
}
u8 AT24CXX_Write_More(u8 addr,u8 *buff,u8 len)
{
	u8 Write_len=16-len%16;
	if(Write_len>len)Write_len=len;
	while(1)
	{
		AT24CXX_Write_Page(addr,buff,Write_len);
		if(Write_len==len)break;
		addr+=Write_len;
		buff+=Write_len;
		len-=Write_len;
		if(len>16)Write_len=16;
		else Write_len=len;
	}
	return 0;
}
