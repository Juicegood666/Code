#ifndef NT35310_H
#define NT35310_H
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"

//16λ����ɫֵ
#define WHITE         	 0xFFFF //��ɫ
#define BLACK         	 0x0000	//��ɫ  
#define BLUE         	   0x001F //��ɫ 
#define RED           	 0xF800 //��ɫ
#define YELLOW        	 0xFFE0 //��ɫ
#define BROWN 			     0XBC40 //��ɫ
#define BRRED 			     0XFC07 //�غ�ɫ
#define GRAY  			     0X8430 //��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
#define LIGHTGREEN     	 0X841F //ǳ��ɫ


//GPIO�ڵ��������
#define LCD_DATA_OC0 PDout(14)
#define LCD_DATA_OC1 PDout(15)
#define LCD_DATA_OC2 PDout(0)
#define LCD_DATA_OC3 PDout(1)
#define LCD_DATA_OC4 PEout(7) 
#define LCD_DATA_OC5 PEout(8)
#define LCD_DATA_OC6 PEout(9)
#define LCD_DATA_OC7 PEout(10)
#define LCD_DATA_OC8 PEout(11) 
#define LCD_DATA_OC9 PEout(12) 
#define LCD_DATA_OC10 PEout(13) 
#define LCD_DATA_OC11 PEout(14) 
#define LCD_DATA_OC12 PEout(15) 
#define LCD_DATA_OC13 PDout(8)
#define LCD_DATA_OC14 PDout(9) 
#define LCD_DATA_OC15 PDout(10)

//LCD���Ŀ��ƽ�
#define NT35310_LCD_BL PBout(0)
#define NT35310_LCD_CS PGout(12)
#define NT35310_LCD_WR PDout(5)
#define NT35310_LCD_RD PDout(4)
#define NT35310_LCD_RS PGout(0)
								
//��������
void NT35310_LcdInit(void);
void NT35310_LcdWriteReg(u16 data);
void NT35310_LcdWriteData(u16 data);
void NT35310_SetPos(u32 x,u32 y);
void NT35310_DrawPoint(u32 x,u32 y,u16 c);
void NT35310_Clear(u16 c);
void NT35310_DisplayData(u32 x,u32 y,u32 w,u32 h,u8 *p);
#endif
