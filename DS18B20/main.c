#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "usart.h"
#include <string.h>
#include "timer.h"
#include "bluetooth.h"
#include "esp8266.h"
#include "aliyun_mqtt.h"
#include "oled.h"
#include "ds18b20.h"
int main()
{
	u16 temp=0;
	LED_Init();
	BEEP_Init();
	KEY_Init();
	USART1_Init(115200);
	TIMER1_Init(72,20000); //超时时间20ms
	USART2_Init(9600);//串口-蓝牙
	TIMER2_Init(72,20000); //超时时间20ms
	USART3_Init(115200);//串口-WIFI
	TIMER3_Init(72,20000); //超时时间20ms
	OLED_Init();
	DS18B20_Init();
	//USART1_Printf("%d",DS18B20_Check());
	while(1)
	{    
		temp=DS18B20_Read_Temp();
		USART1_Printf("当前温度为:%d.%d\n",temp>>4,temp&0xF);
		delay_ms(500);
	}
}

