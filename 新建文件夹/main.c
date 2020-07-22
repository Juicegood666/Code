#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "usart.h"
#include <string.h>
#include "timer.h"
#include "adc.h"
#include "sys.h"
#include "iic.h"
#include "at24cXX.h"
#include "spi.h"
#include "w25q64.h"
extern u8 tmp[4096];
int main()
{
	
	u32 i=0;
	u8 tx_buff[]={0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa};
	u8 rx_buff[50];
	LED_Init();
	BEEP_Init();
	KEY_Init();
	USART1_Init(115200);
	IIC_Init();
	SPI_Init();
	W25Q64_Init();
	while(1)
	{
		if(KEY_Scan(0))
		{
			W25Q64_Sector_Erase(0);
			W25Q64_Write(250,tx_buff,10);
			 W25Q64_Read_Data(250,rx_buff,20);
			 for(i=0;i<20;i++)printf("rx_buff[%d]=0x%x\r\n",i,rx_buff[i]);
		}
	}
}

