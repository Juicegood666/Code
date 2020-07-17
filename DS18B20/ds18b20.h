#ifndef _DS10B20_H
#define	_DS10B20_H
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#define DS18B20_Out PBout(15)
#define DS18B20_In PBin(15)
#define DS18B20_Out_Mode() {GPIOB->CRH&=0x0FFFFFFF;GPIOB->CRH|=0x30000000;}
#define DS18B20_In_Mode() {GPIOB->CRH&=0x0FFFFFFF;GPIOB->CRH|=0x80000000;}
void DS18B20_Init(void);
u8 DS18B20_Check(void);
void DS18B20_Write_Byte(u8 data);
u8 DS18B20_Read_Byte(void);
u16 DS18B20_Read_Temp(void);

#endif
