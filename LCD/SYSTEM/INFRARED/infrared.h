#ifndef _INFRARED_H
#define _INFRARED_H
#include "stm32f10x.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"

void InfraredDecodeInit(void);
//红外线接收头引脚
#define INFRARED_INPUT PBin(9)

extern u8 infraredDecodeData[4];
extern u8 infraredFlag; //1表示解码成功


//红外线发射头引脚
#define INFRARED_OUTPUT PGout(11)
void InfraredCodingInit(void);
void InfraredSend38KHZ(u32 time_us,u8 flag);
void InfraredNECSend(u8 user,u8 key);
#endif 
