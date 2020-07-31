#include "spi.h"

#if SPI2_MODE

//模拟时序引脚定义
#define SPI2_SCK PBout(13)
#define SPI2_MOSI PBout(15)
#define SPI2_MISO PBin(14)

/*
函数功能:(软件模拟时序)初始化SPI2
硬件连接:
				MISO:PB14---输入
				MOSI:PB15---输出
				SCK :PB13
*/
void SPI2_Init(void)
{
	 RCC->APB2ENR|=1<<3; //PB
	 GPIOB->CRH&=0x000FFFFF;
	 GPIOB->CRH|=0x38300000;
	 SPI2_SCK=1;
}

/*
函数功能: SPI底层读写一个字节
特性:
1. 时钟线空闲电平为高电平。
2. 数据在时钟线为低电平的时候发出
3. 读数据是在高电平
4. 数据先发高位
*/
u8 SPI2_ReadWriteOneByte(u8 tx_data)
{
		u8 rx_data=0;
	  u8 i;
		for(i=0;i<8;i++)
		{
				SPI2_SCK=0;
				if(tx_data&0x80)SPI2_MOSI=1;
			  else SPI2_MOSI=0;
				SPI2_SCK=1;
			  tx_data<<=1; //继续发送下一位
			
				rx_data<<=1;
				if(SPI2_MISO)rx_data|=0x01;
		}
		return rx_data;
}

#else   //以下是硬件时序

/*
函数功能:(硬件时序)初始化SPI2
硬件连接:
				MISO:PB14---输入
				MOSI:PB15---输出
				SCK :PB13---时钟线
*/
void SPI2_Init(void)
{
	 RCC->APB2ENR|=1<<3; //PB
	 GPIOB->CRH&=0x000FFFFF;
	 GPIOB->CRH|=0xB8B00000;
	
	 RCC->APB1ENR|=1<<14; //SPI2
	 RCC->APB1RSTR|=1<<14;
	 RCC->APB1RSTR&=~(1<<14);
	
	 //配置SPI核心寄存器
	 SPI2->CR1&=~(1<<15); //0：选择“双线双向”模式
	 SPI2->CR1&=~(1<<11); //0：使用8位数据帧格式进行发送/接收
	 SPI2->CR1|=1<<9;     //1：启用软件从设备管理
	 SPI2->CR1|=1<<8;     //它决定了NSS上的电平
	 SPI2->CR1&=~(1<<7);  //0：先发送MSB
	 SPI2->CR1&=~(0x7<<3); //选择波特率 clk/2
	 SPI2->CR1|=1<<2; 		//1：配置为主设备
	 SPI2->CR1|=1<<1; 		//1： 空闲状态时， SCK保持高电平
	 SPI2->CR1|=1<<0; 		//1： 数据采样从第二个时钟边沿开始
	 SPI2->CR1|=1<<6; 		//1：开启SPI设备
}
/*
函数功能: SPI底层读写一个字节
*/
u8 SPI2_ReadWriteOneByte(u8 tx_data)
{
	u16 cnt=0;
	while(!(SPI2->SR&1<<1)) //等待发送缓冲区为空
	{
			cnt++;
			if(cnt>=65530)return 0xFF;
	}
	
	cnt=0;
	SPI2->DR=tx_data; //发送数据
	while(!(SPI2->SR&1<<0)) //等待数据接收
	{
			cnt++;
			if(cnt>=65530)return 0xFF;
	}
	return SPI2->DR; //返回读到的数据
}
#endif
