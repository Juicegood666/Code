#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "usart.h"
#include <string.h>
#include "timer.h"
#include "adc.h"
#include "sys.h"
int main()
{
	u8 key;
	LED_Init();
	BEEP_Init();
	KEY_Init();
	while(1)
	{
		
	}
}

