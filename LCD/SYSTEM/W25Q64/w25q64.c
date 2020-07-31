#include "w25q64.h"

//芯片的片选脚
#define W25Q64_CS PBout(12)

/*
函数功能: W25Q64初始化
片选引脚: CS---PB12
*/
void W25Q64_Init(void)
{
		RCC->APB2ENR|=1<<3; //PB
		GPIOB->CRH&=0xFFF0FFFF;
		GPIOB->CRH|=0x00030000;
		W25Q64_CS=1; //片选上拉
		SPI2_Init(); //SPI初始化
}

/*
函数功能: 读设备ID
*/
u16 W25Q64_ReadDeviceID(void)
{
		u16 device_id=0;
		W25Q64_CS=0; //选中芯片
		SPI2_ReadWriteOneByte(0x90); //读芯片ID的指令
		SPI2_ReadWriteOneByte(0);    //发送地址16->23
		SPI2_ReadWriteOneByte(0);    //发送地址8->15
		SPI2_ReadWriteOneByte(0);    //发送地址0->7
		
		device_id|=SPI2_ReadWriteOneByte(0xFF)<<8;//读取数据 制造商ID
		device_id|=SPI2_ReadWriteOneByte(0xFF);//读取数据 设备ID
		return device_id; //0xEF16
}


/*
函数功能: 写使能指令
*/
void W25Q64_WriteEnabled(void)
{
		W25Q64_CS=0; //选中芯片
		SPI2_ReadWriteOneByte(0x06); //写使能指令
		W25Q64_CS=1; //取消选中芯片
}

/*
函数功能: 等待忙状态结束
*/
void W25Q64_WaitBusyState(void)
{
		u8 state=1;
		while(state&0x01)
		{
				W25Q64_CS=0; //选中芯片
				SPI2_ReadWriteOneByte(0x05); //读状态指令
				state=SPI2_ReadWriteOneByte(0xFF); //获取状态
				W25Q64_CS=1; //取消选中芯片
		}
}

/*
函数功能: 页编程
函数参数：
u32 addr  写入的起始位置
u8 *buff  写入的缓冲区数据
u32 len   写入的长度(每一页最大只能写256字节)
*/
void W25Q64_PageWrite(u32 addr,u32 len,u8 *buff)
{
		u16 i;
		W25Q64_WriteEnabled(); //写使能
		W25Q64_CS=0; //选中芯片
		SPI2_ReadWriteOneByte(0x02); //页编程指令
		SPI2_ReadWriteOneByte(addr>>16); //发送地址
		SPI2_ReadWriteOneByte(addr>>8);
		SPI2_ReadWriteOneByte(addr>>0);
		
		for(i=0;i<len;i++)
		{
				SPI2_ReadWriteOneByte(buff[i]); //写入数据
		}
		W25Q64_CS=1; //取消选中芯片
		W25Q64_WaitBusyState(); //等待写入结束
}


/*
函数功能: 扇区擦除指令
一个扇区大小: 4096字节
0~4095 表示第一个扇区 .........
*/
void W25Q64_SectorErase(u32 addr)
{
		W25Q64_WriteEnabled(); //写使能
		W25Q64_CS=0; //选中芯片
		SPI2_ReadWriteOneByte(0x20); 			//扇区擦除指令
		SPI2_ReadWriteOneByte(addr>>16);  //发送地址
		SPI2_ReadWriteOneByte(addr>>8);
		SPI2_ReadWriteOneByte(addr>>0);
		W25Q64_CS=1; //取消选中芯片
		W25Q64_WaitBusyState(); //等待写入结束
}

/*
函数功能: 从指定位置读取指定数量的数据
函数参数：
u32 addr  读取的起始位置
u8 *buff  存放读取的数据
u32 len   读取的长度
*/
void W25Q64_ReadData(u32 addr,u32 len,u8 *buff)
{
		u32 i=0;
		W25Q64_CS=0; //选中芯片
		SPI2_ReadWriteOneByte(0x03); 			//读数据指令
		SPI2_ReadWriteOneByte(addr>>16);  //发送地址
		SPI2_ReadWriteOneByte(addr>>8);
		SPI2_ReadWriteOneByte(addr>>0);
		for(i=0;i<len;i++)
		{
				buff[i]=SPI2_ReadWriteOneByte(0xFF);
		}
		W25Q64_CS=1; //取消选中芯片
}


/*
函数功能: 写数据，从指定位置写入指定长度的数据。(不考虑擦除、不考虑覆盖)
函数参数: 
			u32 addr  写入的起始位置
			u8 *buff  写入的缓冲区数据
			u32 len   写入的长度
*/
void W25Q64_WriteDataNoCheck(u32 addr,u32 len,u8 *buff)
{
		u16 page_byte=256-addr%256; //得到当前页剩余的字节数量
		if(page_byte>len) //判断当前页剩余的字节空间是否够写
		{
				page_byte=len; //表示一次性可以写完
		}
		while(1)
		{
				W25Q64_PageWrite(addr,page_byte,buff);//写一页
				if(page_byte==len)break; //写完了
				buff+=page_byte; //指针偏移
				addr+=page_byte;//地址偏移
				len-=page_byte;//得到剩余没有写完的长度
				if(len>256)page_byte=256;
				else page_byte=len; //一次可以写完
		}
}


/*
函数功能: 写数据，从指定位置写入指定长度的数据。(不考虑覆盖)
函数参数: 
			u32 addr  写入的起始位置
			u8 *buff  写入的缓冲区数据
			u32 len   写入的长度
*/
u8 W25Q64_CHECK_BUFF[4096]; //一个扇区的空间
void W25Q64_WriteData(u32 addr,u32 len,u8 *buff)
{
		u32 i=0;
		u16 p_byte=4096-addr%4096; //得到当前扇区剩余的字节数量
		if(p_byte>len) //判断当前扇区剩余的字节空间是否够写
		{
				p_byte=len; //表示一次性可以写完
		}
		while(1)
		{
				//读取将要写入空间的数据
				W25Q64_ReadData(addr,p_byte,W25Q64_CHECK_BUFF);
				//对数据进行判断
				for(i=0;i<p_byte;i++)
				{
						if(W25Q64_CHECK_BUFF[i]!=0xFF)break;
				}
				if(i!=p_byte) //需要擦除
				{
						W25Q64_SectorErase(addr);  //进行擦除
				}
				//写入数据
				W25Q64_WriteDataNoCheck(addr,p_byte,buff);
				if(p_byte==len)break; //写完了
				
				addr+=p_byte; //地址向后偏移
				buff+=p_byte; //数据指针偏移
				len-=p_byte;  //长度自减
				
				 //判断剩余扇区一次性是否可以写完
				if(len>4096)p_byte=4096;
				else p_byte=len;
		}
}


