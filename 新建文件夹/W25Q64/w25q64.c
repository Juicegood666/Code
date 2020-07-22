#include "w25q64.h"
void W25Q64_Init(void)
{
	RCC->APB2ENR|=1<<3;
	GPIOB->CRH&=0xFFF0FFFF;
	GPIOB->CRH|=0x00030000;
	W25q64_CS=1;
	
}
u16 W25Q64_Read_ID()
{
	u16 ID;
	u8 data_H,data_L;
	W25q64_CS=0;
	SPI_Write_Byte(0x90);
	SPI_Write_Byte(0x00);
	SPI_Write_Byte(0x00);
	SPI_Write_Byte(0x00);
	data_H=SPI_Read_Byte();
	data_L=SPI_Read_Byte();
	ID=data_H<<8|data_L;
	W25q64_CS=1;
	return ID;
}
void W25Q64_Write_Enable(void)
{
	W25q64_CS=0;
	SPI_Write_Byte(0x6);
	W25q64_CS=1;
}
u8 W25Q64_Read_State()
{
	u8 state=0;
	SPI_CS=0;
	SPI_Write_Byte(0x5);
	state=SPI_Read_Byte();
	SPI_CS=1;
	return state&0x01;
}
void W25Q64_Write_Page(u32 addr,u8 *buff,u32 len)
{
	u32 i;
	W25Q64_Write_Enable();
	SPI_CS=0;
	SPI_Write_Byte(0x2);
	SPI_Write_Byte(addr>>16);
	SPI_Write_Byte(addr>>8);
	SPI_Write_Byte(addr);
	for(i=0;i<len;i++)SPI_Write_Byte(buff[i]);
	SPI_CS=1;
	while(W25Q64_Read_State()){}
}
void W25Q64_Read_Data(u32 addr,u8 *buff,u32 len)
{
	u32 i;
	SPI_CS=0;
	SPI_Write_Byte(0x3);
	SPI_Write_Byte(addr>>16);
	SPI_Write_Byte(addr>>8);
	SPI_Write_Byte(addr);
	for(i=0;i<len;i++)buff[i]=SPI_Read_Byte();
	SPI_CS=1;
}
void W25Q64_Sector_Erase(u32 addr)
{
	W25Q64_Write_Enable();
	while(W25Q64_Read_State()){}
	SPI_CS=0;
	SPI_Write_Byte(0x20);
	SPI_Write_Byte(addr>>16);
	SPI_Write_Byte(addr>>8);
	SPI_Write_Byte(addr);
	SPI_CS=1;
	while(W25Q64_Read_State()){}
}
void W25Q64_Write_Nowipe(u32 addr,u8 *buff,u32 len)
{
	u32 write_len=256-addr%256;
	if(write_len>len)write_len=len;
	while(1)
	{
		W25Q64_Write_Page(addr,buff,write_len);
		if(write_len==len)break;
		addr+=write_len;
		buff+=write_len;
		len-=write_len;
		if(len>256)write_len=256;
		else write_len=len;
	}
}
u8 tmp[4096];
void W25Q64_Write(u32 addr,u8 *buff,u32 len)
{
	u32 i;
	u32 write_len=4096-addr%4096;
	if(write_len>len)write_len=len;
	while(1)
	{
		W25Q64_Read_Data(addr,tmp,write_len);
		for(i=0;i<write_len;i++)if(tmp[i]!=0xFF)break;
		if(i!=write_len)W25Q64_Sector_Erase(addr);
		W25Q64_Write_Nowipe(addr,buff,write_len);
		if(write_len==len)break;
		addr+=write_len;
		buff+=write_len;
		len-=write_len;
		if(len>4096)write_len=4096;
		else write_len=len;
	}
	
}





