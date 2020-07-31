#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"

#define DS18B20_OUTPUT_MODE() {GPIOB->CRH&=0x0FFFFFFF;GPIOB->CRH|=0x30000000;}
#define DS18B20_INPUT_MODE()  {GPIOB->CRH&=0x0FFFFFFF;GPIOB->CRH|=0x80000000;}
#define DS18B20_Out PBout(15)
#define DS18B20_In PBin(15)
void DS18B20_Init(void);
u16 DS18B20_Read_Temp(void);
void DS18B20_Reset(void);
u8 DS18B20_Check(void);
void DS18B20_WriteByte(u8 cmd);
u8 DS18B20_ReadByte(void);
void DS18B20_ReadRom(u8 *ds18b20_rom);
u16 DS18B20_Read_Temp_2(u8 *ds18b20_rom);
#endif
