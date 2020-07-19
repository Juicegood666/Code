#ifndef _OLED_H
#define _OLED_H
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include <string.h>

/*
��ʼ��OLED��ʾ��Ӳ��
Ӳ������:
D0--PB14--ʱ����
D1--PB13--������
RES-PB12-��λ��
DC--PB1--��������ѡ���
CS--PA7--Ƭѡ
*/
#define OLED_SCK PBout(14)
#define OLED_MOSI PBout(13)
#define OLED_RES PBout(12)
#define OLED_DC PBout(1)
#define OLED_CS PAout(7)

//��������
#define OLED_WRITE_CMD 0
#define OLED_WRITE_DAT 1

//��������
void OLED_SPI_WriteOneByte(u8 data,u8 flag);
void OLED_Init(void);
void OLED_Clear(u8 data);
void OLED_SetPos(u8 x,u8 y);
void OLED_DisplayPoint(u8 x,u8 y,u8 c);
void OLED_DisplayData(u8 x,u8 y,u8 w,u8 h,u8 *p);
void OLED_WriteGRAM(void);
void OLED_DisplayString(u8 x,u8 y,char *str);
extern u8 OLED_GRAM[8][128]; //8��128��---->8ҳ128��

extern const u8 ASCII_8x16[95][16];

#endif
