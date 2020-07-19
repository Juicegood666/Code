#ifndef _OLED_H
#define _OLED_H
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include <string.h>

/*
初始化OLED显示屏硬件
硬件连接:
D0--PB14--时钟线
D1--PB13--数据线
RES-PB12-复位脚
DC--PB1--命令数据选择脚
CS--PA7--片选
*/
#define OLED_SCK PBout(14)
#define OLED_MOSI PBout(13)
#define OLED_RES PBout(12)
#define OLED_DC PBout(1)
#define OLED_CS PAout(7)

//定义命令
#define OLED_WRITE_CMD 0
#define OLED_WRITE_DAT 1

//函数声明
void OLED_SPI_WriteOneByte(u8 data,u8 flag);
void OLED_Init(void);
void OLED_Clear(u8 data);
void OLED_SetPos(u8 x,u8 y);
void OLED_DisplayPoint(u8 x,u8 y,u8 c);
void OLED_DisplayData(u8 x,u8 y,u8 w,u8 h,u8 *p);
void OLED_WriteGRAM(void);
void OLED_DisplayString(u8 x,u8 y,char *str);
extern u8 OLED_GRAM[8][128]; //8行128列---->8页128列

extern const u8 ASCII_8x16[95][16];

#endif
