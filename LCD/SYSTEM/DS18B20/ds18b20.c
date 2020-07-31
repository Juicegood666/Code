#include "ds18b20.h"
#include <stdio.h>

/*
函数名称：void DS18B20_Init(void)
函数功能：DS18B20初始化
硬件链接：DQ->PB15
*/
void DS18B20_Init(void)
{
	RCC->APB2ENR|=1<<3;//GPIOB时钟使能
	GPIOB->CRH&=0x0FFFFFFF;
	GPIOB->CRH|=0x30000000;
	GPIOB->ODR|=1<<15;//上拉
}


/*
函数名称：void DS18B20_Reset(void)
函数功能：发送复位信号
*/
void DS18B20_Reset(void)
{
	DS18B20_OUTPUT_MODE(); //GPIO口处于输出模式
	DS18B20_Out=0; //拉低总线
	delay_us(750); //等待时间 标准480us
	DS18B20_Out=1; //释放总线 
	delay_us(15); 
}

/*
函数名称：void DS18B20_Reset(void)
函数功能：检测存在脉冲
返 回 值：0代表存在  1代表不存在
*/
u8 DS18B20_Check(void)
{
	u32 i=0;
	DS18B20_INPUT_MODE(); //将DS18B20初始化为输入模式
	while(DS18B20_In && i<80) //标准时间: 15us ~ 60us
	{
		i++;
		delay_us(1);
	}
	if(i>=60)return 1; //说明DS18B20硬件有问题
	i=0;
  
  //等待DS18B20释放总线
	while(!DS18B20_In && i<240) //标准时间: 60us ~ 240us 
	{
		i++;
		delay_us(1);
	}
	if(i>=240)return 1; //说明DS18B20硬件有问题
	return 0;
}


/*
函数名称：void DS18B20_Reset(void)
函数功能：发送命令
*/
void DS18B20_WriteByte(u8 cmd)
{
	u8 i=0;
	DS18B20_OUTPUT_MODE(); //配置GPIO口为输出模式
	for(i=0;i<8;i++)
	{
		DS18B20_Out = 0;    //先拉低总线
		delay_us(2);        //持续1us以上
		DS18B20_Out = cmd & 0x01; //根据实际数据进行发送
		delay_us(60);       //等待时间(DS18B20采样的时间)
		DS18B20_Out=1;      //释放总线
		cmd>>=1;
	}
}

/*
函数名称：u8 DS18B20_ReadByte(void)
函数功能：读取数据
返 回 值：读到的数据
*/
u8 DS18B20_ReadByte(void)
{
	u8 i,data=0;	
	for(i=0;i<8;i++)
	{
		DS18B20_OUTPUT_MODE(); //初始化为输出模式 
		DS18B20_Out=0; //先拉低总线
		delay_us(2);   //延时时间 >1us
		DS18B20_INPUT_MODE(); //再初始化为输入模式
		delay_us(8);          //间隔时间
		data>>=1;
		if(DS18B20_In)data|=0x80;  //对总线电平进行采样
		delay_us(60);	             //间隔时间
		DS18B20_Out=1;             //释放总线
	}
	return data;
}

/*
函数名称：u16 DS18B20_Read_Temp(void)
函数功能：获取温度 (单个DS18B20情况)
返 回 值：温度值
*/
u16 DS18B20_Read_Temp(void)
{
	u8 temp_H,temp_L;
	u16 temp;
	DS18B20_Reset(); //发送复位信号
	if(DS18B20_Check())printf("检测失败\r\n"); //检测应答
	DS18B20_WriteByte(0xcc); //跳过ROM检测(64位编号)
	DS18B20_WriteByte(0x44); //开始温度检测
	DS18B20_Reset();         //发送复位信号
	if(DS18B20_Check())printf("检测失败\r\n"); //检测应答
	DS18B20_WriteByte(0xcc); //跳过ROM检测(64位编号)
	DS18B20_WriteByte(0xbe); //读取暂存器(温度数据前2个字节)
	temp_L=DS18B20_ReadByte();//读取温度低位
	temp_H=DS18B20_ReadByte();//读取温度高位
	temp=(temp_H<<8)|temp_L;  //合成温度
	return temp; //返回温度数据
}


/*
函数功能: 读取DS18B20 64位编号
*/
void DS18B20_ReadRom(u8 *ds18b20_rom)
{
  u8 i;
  DS18B20_Reset(); //发送复位信号
  DS18B20_Check(); //检测存在脉冲
  DS18B20_WriteByte(0x33); //读取64位的ROM编号
  
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
函数名称：u16 DS18B20_Read_Temp(void)
函数功能：获取温度 (多个DS18B20情况)
返 回 值：温度值
*/
u16 DS18B20_Read_Temp_2(u8 *ds18b20_rom)
{
	u8 temp_H,temp_L;
	u16 temp;
  u8 i;
	DS18B20_Reset(); //发送复位信号
	if(DS18B20_Check())printf("检测失败\r\n"); //检测应答
	DS18B20_WriteByte(0x55); //进行ROM匹配(64位编号)
  for(i=0;i<8;i++)DS18B20_WriteByte(ds18b20_rom[i]); //发送ROM编码
	
  DS18B20_WriteByte(0x44); //开始温度检测
	DS18B20_Reset();         //发送复位信号
	if(DS18B20_Check())printf("检测失败\r\n"); //检测应答
  DS18B20_WriteByte(0x55); //进行ROM匹配(64位编号)
  for(i=0;i<8;i++)DS18B20_WriteByte(ds18b20_rom[i]); //发送ROM编码
	DS18B20_WriteByte(0xbe); //读取暂存器(温度数据前2个字节)
	temp_L=DS18B20_ReadByte();//读取温度低位
	temp_H=DS18B20_ReadByte();//读取温度高位
	temp=(temp_H<<8)|temp_L;  //合成温度
	return temp; //返回温度数据
}

