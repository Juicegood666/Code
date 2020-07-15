#ifndef _OLED_H
#define _OLED_H
#include "delay.h"
#include "sys.h"
/*
硬件连接:
D0--PB14--时钟线
D1--PB13--数据线
RES-PB12--复位脚
DC--PB1--命令数据选择脚
CS--PA7--片选
*/
#include "stm32f10x.h"
#define OLED_SCK PBout(14)
#define OLED_MOSI PBout(13)
#define OLED_RES PBout(12)
#define OLED_DC PBout(1)
#define OLED_CS PAout(7)
#define OLED_WRITE_CMD 0
#define OLED_WRITE_DAT 1
void OLED_Init(void);
void write_i(u8 data,u8 flag);
void OLED_CLEAR(u8 data);
void OLED_SETPOS(u8 x,u8 y);
void OLED_DISPALY_DATA(u8 x,u8 y,u8 *p);
#endif
