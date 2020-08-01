#include "stm32f10x.h"
#include "beep.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include "exti.h"
#include "timer.h"
#include "rtc.h"
#include "adc.h"
#include "ds18b20.h"
#include "ble.h"
#include "esp8266.h"
#include "wdg.h"
#include "oled.h"
#include "rfid_rc522.h"
#include "infrared.h"
#include "iic.h"
#include "at24c08.h"
#include "w25q64.h"
#include "nt35310_lcd.h"
#include "sram.h"
#include "xpt2046.h"
#include "sdcard.h"
//#include "ff.h"  //FATFS文件系统的头文件
#include "bmp.h"
#include "includes.h"
#include "dma.h"
u8 buff[20];
int main()
{  
	u8 key=0;
	u8 tmp=0;
	LED_Init();
	KEY_Init();
	BEEP_Init();
	TIM1_Init(72,20000); //辅助串口1接收，超时时间为20ms
	USART_X_Init(USART1,72,115200);
	
	DMA_CH5_Init((u32)&USART1->DR,(u32)buff);
	
	
	while(1) 
	{
		key=KEY_Scanf();
		if(key)
		{
			printf("开始");
			USART1->CR3&=~(1<<6);
			tmp=USART1->DR;
			USART1->CR3|=1<<6;
			DMA_CH5_Start(10);
		}
		
		if(DMA1->ISR&1<<17)
		{
			printf("%s",buff);
			DMA1->IFCR|=1<<17;	
		}
	}
}
