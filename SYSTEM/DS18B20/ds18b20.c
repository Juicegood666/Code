#include "ds18b20.h"
/*
函数功能: 硬件初始化--IO配置
硬件连接: PB15
*/
void DS18B20_Init(void)
{
    /*1. 开时钟*/
    RCC->APB2ENR|=1<<3; //PB
    /*2. 配置GPIO口模式*/
    GPIOB->CRH&=0x0FFFFFFF;
    GPIOB->CRH|=0x30000000;
    /*3. 上拉*/
    GPIOB->ODR|=1<<15;
}

/*
函数功能: 发送复位脉冲检测DS18B20硬件--建立通信过程
返 回 值: 0表示成功  1表示失败  
*/
u8 DS18B20_Check(void)
{
    u8 i;
    DS18B20_OUT_MODE(); //配置IO口为输出模式
    DS18B20_OUT=0;      //拉低
    delay_us(580);       
    DS18B20_OUT=1;      //拉高
    
    DS18B20_IN_MODE();  //配置IO口为输入模式
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
函数功能: DS18B20写一个字节数据
*/
void DS18B20_WriteOnebyte(u8 cmd)
{
    u8 i;
    DS18B20_OUT_MODE(); //输出模式
    for(i=0;i<8;i++)
    {
        if(cmd&0x01) //发送1
        {
            DS18B20_OUT=0;
            delay_us(15);
            DS18B20_OUT=1;
            delay_us(45);
            DS18B20_OUT=1;
            delay_us(2);
        }
        else //发送0
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
函数功能: DS18B20读一个字节数据
*/
u8 DS18B20_ReadOnebyte(void)
{
    u8 i;
    u8 data=0;
    for(i=0;i<8;i++)
    {
        DS18B20_OUT_MODE(); //输出模式
        DS18B20_OUT=0;
        delay_us(2);
        DS18B20_IN_MODE();
        delay_us(8);
        data>>=1; //右移1位
        if(DS18B20_IN)data|=0x80;
        delay_us(50);
        DS18B20_OUT=1;
        delay_us(2);
    }
    return data;
}

/*
函数功能: 读取一次DS18B20的温度数据
返回值: 读取的温度数据高低位
针对是总线上只有一个DS18B20温度传感器的情况
*/
u16 DS18B20_ReadTemp(void)
{
   u16 temp;
   u8 t_L,t_H;
   if(DS18B20_Check())return 1;
   DS18B20_WriteOnebyte(0xCC); //跳跃 ROM 指令 --不验证身份
   DS18B20_WriteOnebyte(0x44); //发送温度转换指令
    
   if(DS18B20_Check())return 2;
   DS18B20_WriteOnebyte(0xCC); //跳跃 ROM 指令 --不验证身份
   DS18B20_WriteOnebyte(0xBE); //读取RAM里的数据
   
   //读取温度
   t_L=DS18B20_ReadOnebyte(); //低字节
   t_H=DS18B20_ReadOnebyte(); //高字节
   temp=t_H<<8|t_L;
   return temp; 
}

/*
函数功能: 读取一次DS18B20的温度数据
返回值: 读取的温度数据高低位
总线上有多个DS18B20温度传感器的情况
*/
u16 DS18B20_ReadTemp_2(u8 *number)
{
   u16 temp;
   u8 t_L,t_H;
   u8 i;
   if(DS18B20_Check())return 1;
   DS18B20_WriteOnebyte(0x55); //验证身份
   for(i=0;i<8;i++)DS18B20_WriteOnebyte(number[i]); //发送序列号 
   DS18B20_WriteOnebyte(0x44); //发送温度转换指令
    
   if(DS18B20_Check())return 2;
   DS18B20_WriteOnebyte(0x55); //验证身份
   for(i=0;i<8;i++)DS18B20_WriteOnebyte(number[i]); //发送序列号
   DS18B20_WriteOnebyte(0xBE); //读取RAM里的数据
   
   //读取温度
   t_L=DS18B20_ReadOnebyte(); //低字节
   t_H=DS18B20_ReadOnebyte(); //高字节
   temp=t_H<<8|t_L;
   return temp; 
}
