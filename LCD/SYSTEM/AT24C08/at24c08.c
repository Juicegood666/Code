#include "at24c08.h"

#if IIC_MODE

/*
函数功能: 写一个字节
函数参数:
	u8 addr 数据的位置(0~1023)
	u8 data 数据范围(0~255)
*/
void AT24C08_WriteOneByte(u16 addr,u8 data)
{
		IIC_SendStart();//起始信号
		IIC_SendOneByte(AT24C08_WRITE_ADDR);//发送设备地址
		IIC_GetAck();//获取应答
		IIC_SendOneByte(addr); //数据存放的地址
		IIC_GetAck();//获取应答
		IIC_SendOneByte(data); //发送将要存放的数据
		IIC_GetAck();//获取应答
		IIC_SendStop(); //停止信号
		DelayMs(10); //等待写
}


/*
函数功能: 读一个字节
函数参数:
	u8 addr 数据的位置(0~1023)
返回值: 读到的数据
*/
u8 AT24C08_ReadOneByte(u16 addr)
{
		u8 data=0;
		IIC_SendStart();//起始信号
		IIC_SendOneByte(AT24C08_WRITE_ADDR);//发送设备地址
		IIC_GetAck();//获取应答
		IIC_SendOneByte(addr); //将要读取数据的地址
		IIC_GetAck();//获取应答
		
		IIC_SendStart();//起始信号
		IIC_SendOneByte(AT24C08_READ_ADDR);//发送设备地址
		IIC_GetAck();//获取应答
		data=IIC_RecvOneByte();//读取数据
		IIC_SendAck(1); //发送非应答
		IIC_SendStop(); //停止信号
		return data;
}

/*
函数功能: 从指定位置读取指定长度的数据
函数参数:
	u16 addr 数据的位置(0~1023)
	u16 len 读取的长度
	u8 *buffer 存放读取的数据
返回值: 读到的数据
*/
void AT24C08_ReadByte(u16 addr,u16 len,u8 *buffer)
{
		u16 i=0;
		IIC_SendStart();//起始信号
		IIC_SendOneByte(AT24C08_WRITE_ADDR);//发送设备地址
		IIC_GetAck();//获取应答
		IIC_SendOneByte(addr); //将要读取数据的地址
		IIC_GetAck();//获取应答
		
		IIC_SendStart();//起始信号
		IIC_SendOneByte(AT24C08_READ_ADDR);//发送设备地址
		IIC_GetAck();//获取应答
		for(i=0;i<len;i++)
		{
				buffer[i]=IIC_RecvOneByte();//读取数据
				if(i<len-1)IIC_SendAck(0); //发送应答
				else IIC_SendAck(1); //发送非应答
		}
		IIC_SendStop(); //停止信号
}

/*
函数功能: AT24C08页写函数
函数参数:
	u16 addr 写入的位置(0~1023)
	u8 len 写入的长度(每页16字节)
	u8 *buffer 存放读取的数据
*/
void AT24C08_PageWrite(u16 addr,u16 len,u8 *buffer)
{
		u16 i=0;
		IIC_SendStart();//起始信号
		IIC_SendOneByte(AT24C08_WRITE_ADDR);//发送设备地址
		IIC_GetAck();//获取应答
		IIC_SendOneByte(addr); //数据存放的地址
		IIC_GetAck();//获取应答
		
		for(i=0;i<len;i++)
		{
			IIC_SendOneByte(buffer[i]); //发送将要存放的数据
			IIC_GetAck();//获取应答	
		}
		IIC_SendStop(); //停止信号
		DelayMs(10); //等待写
}


/*
函数功能: 从指定位置写入指定长度的数据
函数参数:
	u16 addr 数据的位置(0~1023)
	u16 len 写入的长度
	u8 *buffer 存放即将写入的数据
返回值: 读到的数据
*/
void AT24C08_WriteByte(u16 addr,u16 len,u8 *buffer)
{
		u8 page_byte=16-addr%16; //得到当前页剩余的字节数量
		if(page_byte>len) //判断当前页剩余的字节空间是否够写
		{
				page_byte=len; //表示一次性可以写完
		}
		while(1)
		{
				AT24C08_PageWrite(addr,page_byte,buffer); //写一页
				if(page_byte==len)break; //写完了
				buffer+=page_byte; //指针偏移
				addr+=page_byte;//地址偏移
				len-=page_byte;//得到剩余没有写完的长度
				if(len>16)page_byte=16;
				else page_byte=len; //一次可以写完
		}
}

#else
/*
函数功能: 写一个字节
函数参数:
	u8 addr 数据的位置(0~1023)
	u8 data 数据范围(0~255)
*/
void AT24C08_WriteOneByte(u16 addr,u8 data)
{
		IIC_SendStart();//起始信号
		IIC_SendAddr(AT24C08_WRITE_ADDR);//发送设备地址
		IIC_SendOneByte(addr); //数据存放的地址
		IIC_SendOneByte(data); //发送将要存放的数据
		IIC_SendStop(); //停止信号
		DelayMs(10); //等待写
}


/*
函数功能: 读一个字节
函数参数:
	u8 addr 数据的位置(0~1023)
返回值: 读到的数据
*/
u8 AT24C08_ReadOneByte(u16 addr)
{
		u8 data=0;
		IIC_SendStart();//起始信号
		IIC_SendAddr(AT24C08_WRITE_ADDR);//发送设备地址
		IIC_SendOneByte(addr); //将要读取数据的地址
		IIC_SendStart();//起始信号
		IIC_SendAddr(AT24C08_READ_ADDR);//发送设备地址
		data=IIC_RecvOneByte();//读取数据
		IIC_SendStop(); //停止信号
		return data;
}


/*
函数功能: 从指定位置读取指定长度的数据 (顺序读)
函数参数:
	u16 addr 数据的位置(0~1023)
	u16 len 读取的长度
	u8 *buffer 存放读取的数据
返回值: 读到的数据
*/
void AT24C08_ReadByte(u16 addr,u16 len,u8 *buffer)
{
		u32 i=0;
	  IIC_SendStart();//起始信号
		IIC_SendAddr(AT24C08_WRITE_ADDR);//发送设备地址
		IIC_SendOneByte(addr); //将要读取数据的地址
		IIC_SendStart();//起始信号
		IIC_SendAddr(AT24C08_READ_ADDR);//发送设备地址
		
		for(i=0;i<len;i++)
		{
				buffer[i]=IIC_RecvOneByte();//读取数据
		}
		IIC_SendStop(); //停止信号
}

/*
函数功能: AT24C08页写函数
函数参数:
	u16 addr 写入的位置(0~1023)
	u8 len 写入的长度(每页16字节)
	u8 *buffer 存放读取的数据
*/
void AT24C08_PageWrite(u16 addr,u16 len,u8 *buffer)
{
		u16 i=0;
		IIC_SendStart();//起始信号
		IIC_SendAddr(AT24C08_WRITE_ADDR);//发送设备地址
		IIC_SendOneByte(addr); //数据存放的地址
		for(i=0;i<len;i++)
		{
			IIC_SendOneByte(buffer[i]); //发送将要存放的数据
		}
		IIC_SendStop(); //停止信号
		DelayMs(10); //等待写
}


/*
函数功能: 从指定位置写入指定长度的数据
函数参数:
	u16 addr 数据的位置(0~1023)
	u16 len 写入的长度
	u8 *buffer 存放即将写入的数据
返回值: 读到的数据
*/
void AT24C08_WriteByte(u16 addr,u16 len,u8 *buffer)
{
		u8 page_byte=16-addr%16; //得到当前页剩余的字节数量
		if(page_byte>len) //判断当前页剩余的字节空间是否够写
		{
				page_byte=len; //表示一次性可以写完
		}
		while(1)
		{
				AT24C08_PageWrite(addr,page_byte,buffer); //写一页
				if(page_byte==len)break; //写完了
				buffer+=page_byte; //指针偏移
				addr+=page_byte;//地址偏移
				len-=page_byte;//得到剩余没有写完的长度
				if(len>16)page_byte=16;
				else page_byte=len; //一次可以写完
		}
}

#endif
