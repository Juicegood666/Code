#include "iic.h"

#if IIC_MODE
/*
函数功能: 初始化IIC总线
硬件连接: 
				SCL---PB6
				SDA---PB7
*/
void IIC_Init(void)
{
	 RCC->APB2ENR|=1<<3;
	 GPIOB->CRL&=0x00FFFFFF;
	 GPIOB->CRL|=0x33000000;
	 GPIOB->ODR|=0x3<<6;
}

/*
函数功能: 发送起始信号
当时钟线为高电平的时候，数据线由高电平变为低电平的过程
*/
void IIC_SendStart(void)
{
		IIC_OUTPUT_MODE(); //输出模式
		IIC_SCL=1;
		IIC_SDA_OUT=1;
		DelayUs(2);
		IIC_SDA_OUT=0;
		DelayUs(2);
		IIC_SCL=0; //总线开始工作、开始读写数据
}

/*
函数功能: 停止信号
当时钟线为高电平的时候，数据线由低电平变为高电平的过程
*/
void IIC_SendStop(void)
{
		IIC_OUTPUT_MODE(); //输出模式
		IIC_SCL=0;
		IIC_SDA_OUT=0;
	  DelayUs(2);
		IIC_SCL=1;
		DelayUs(2);
		IIC_SDA_OUT=1;
}

/*
函数功能: 获取应答信号
返 回 值: 0表示获取到应答 1表示没有获取到应答
*/
u8 IIC_GetAck(void)
{
		u8 cnt;
		IIC_INPUT_MODE();//输入模式
		IIC_SDA_OUT=1; //上拉
		IIC_SCL=0;
		DelayUs(2);
	  IIC_SCL=1;
		while(IIC_SDA_IN)
		{
				cnt++;
				if(cnt>250)return 1; //获取不到应答
		}
		IIC_SCL=0;
		return 0;
}


/*
函数功能: 发送应答信号
函数参数：0表示应答 1表示非应答
*/
void IIC_SendAck(u8 ack)
{
		IIC_OUTPUT_MODE(); //输出模式
		IIC_SCL=0;
	  DelayUs(2);
		if(ack)IIC_SDA_OUT=1; //发送非应答
		else IIC_SDA_OUT=0; //发送应答
		DelayUs(2);
		IIC_SCL=1;
		DelayUs(2);
		IIC_SCL=0;
}

/*
函数功能: 发送一个字节数据
*/
void IIC_SendOneByte(u8 data)
{
		u8 i=0;
		IIC_OUTPUT_MODE(); //输出模式
		for(i=0;i<8;i++)
		{
				IIC_SCL=0;
				if(data&0x80)IIC_SDA_OUT=1;
				else IIC_SDA_OUT=0;
				DelayUs(2);
				IIC_SCL=1;
				DelayUs(2);
				data<<=1;
		}
		IIC_SCL=0;
}

/*
函数功能: 接收一个字节数据
*/
u8 IIC_RecvOneByte(void)
{
	u8 i=0;
	u8 data=0;
	IIC_INPUT_MODE();
	for(i=0;i<8;i++)
	{
			IIC_SCL=0;
			DelayUs(2);
			IIC_SCL=1;
			data<<=1; //表示默认收到0
			if(IIC_SDA_IN)data|=0x01;
			DelayUs(2);
	}
	IIC_SCL=0;
	return data;
}

#else  //硬件时序

/*
函数功能: 初始化IIC总线
硬件连接: 
				SCL---PB6
				SDA---PB7
*/
void IIC_Init(void)
{
	 /*1. 时钟配置*/
	 RCC->APB2ENR|=1<<3; //PB

 	 /*2. GPIO口模式配置*/
	 GPIOB->CRL&=0x00FFFFFF;
	 GPIOB->CRL|=0xFF000000; //复用开漏输出
	 GPIOB->ODR|=0x3<<6;
		
	 /*3. GPIO口时钟配置(顺序不能错)*/
	 RCC->APB1ENR|=1<<21; //I2C1时钟
	 RCC->APB1RSTR|=1<<21; //开启复位时钟
	 RCC->APB1RSTR&=~(1<<21);//关闭复位时钟
	
	 /*4. 配置IIC的核心寄存器*/
	 I2C1->CR2=0x24<<0;  //配置主机频率为36MHZ
	 I2C1->CCR|=0x2D<<0; //配置主机频率是400KHZ
	 I2C1->CR1|=1<<0;    //开启IIC模块
	
	/*
	CCR=主机时钟频率/2/IIC总线的频率
	45=36MHZ/2/400KHZ  ---0x2D
	*/
}


/*
函数功能: 发送起始信号
当时钟线为高电平的时候，数据线由高电平变为低电平的过程
*/
void IIC_SendStart(void)
{
	I2C1->CR1|=1<<8; //产生起始信号
	while(!(I2C1->SR1&1<<0)){} //等待起始信号完成
	I2C1->SR1=0; //清除状态位
}

/*
函数功能: 停止信号
当时钟线为高电平的时候，数据线由低电平变为高电平的过程
*/
void IIC_SendStop(void)
{
		I2C1->CR1|=1<<9;
}

/*
函数功能: 发送地址数据
*/
void IIC_SendAddr(u8 addr)
{
	u32 s1,s2;
	I2C1->DR=addr; //发送数据
	while(1)
	{
			s1=I2C1->SR1;
			s2=I2C1->SR2;
		  if(s1&1<<1) //判断地址有没有发送成功
			{
					break;
			}
	}
}

/*
函数功能: 发送数据
*/
void IIC_SendOneByte(u8 addr)
{
	u32 s1,s2;
	I2C1->DR=addr; //发送数据
	while(1)
	{
			s1=I2C1->SR1;
			s2=I2C1->SR2;
			if(s1&1<<2) //判断数据有没有发送成功
			{
					break;
			}
	}
}

/*
函数功能: 接收一个字节数据
*/
u8 IIC_RecvOneByte(void)
{
	u8 data=0;
	I2C1->CR1|=1<<10; //使能应答
	while(!(I2C1->SR1&1<<6)){} //等待数据
	data=I2C1->DR;
	I2C1->CR1&=~(1<<10); //关闭应答使能
	return data;
}
#endif
