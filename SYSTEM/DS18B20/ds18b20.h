#ifndef _DS18B20_H
#define _DS18B20_H
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"

#define DS18B20_OUT PBout(15)
#define DS18B20_IN  PBin(15)

#define DS18B20_IN_MODE() {GPIOB->CRH&=0x0FFFFFFF;GPIOB->CRH|=0x80000000;}
#define DS18B20_OUT_MODE() {GPIOB->CRH&=0x0FFFFFFF;GPIOB->CRH|=0x30000000;}

//º¯ÊýÉùÃ÷
void DS18B20_Init(void);
void DS18B20_WriteOnebyte(u8 cmd);
u8 DS18B20_Check(void);
u8 DS18B20_ReadOnebyte(void);
u16 DS18B20_ReadTemp(void); 
u16 DS18B20_ReadTemp_2(u8 *number);
#endif
