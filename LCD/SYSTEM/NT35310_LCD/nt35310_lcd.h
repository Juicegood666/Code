#ifndef NT35310_H
#define NT35310_H
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"

//16位的颜色值
#define WHITE         	 0xFFFF //白色
#define BLACK         	 0x0000	//黑色  
#define BLUE         	   0x001F //蓝色 
#define RED           	 0xF800 //红色
#define YELLOW        	 0xFFE0 //黄色
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色


//GPIO口的数据输出
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

//LCD屏的控制脚
#define NT35310_LCD_BL PBout(0)
#define NT35310_LCD_CS PGout(12)
#define NT35310_LCD_WR PDout(5)
#define NT35310_LCD_RD PDout(4)
#define NT35310_LCD_RS PGout(0)
								
//函数声明
void NT35310_LcdInit(void);
void NT35310_LcdWriteReg(u16 data);
void NT35310_LcdWriteData(u16 data);
void NT35310_SetPos(u32 x,u32 y);
void NT35310_DrawPoint(u32 x,u32 y,u16 c);
void NT35310_Clear(u16 c);
void NT35310_DisplayData(u32 x,u32 y,u32 w,u32 h,u8 *p);
#endif
