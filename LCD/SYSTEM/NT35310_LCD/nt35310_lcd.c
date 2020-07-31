#include "nt35310_lcd.h"
/*
函数功能: LCD屏初始化
硬件连接：
FSMC_D0 ------PD14
FSMC_D1 ------PD15
FSMC_D2 ------PD0
FSMC_D3 ------PD1
FSMC_D4 ------PE7
FSMC_D5 ------PE8
FSMC_D6 ------PE9
FSMC_D7 ------PE10
FSMC_D8 ------PE11
FSMC_D9 ------PE12
FSMC_D10 -----PE13
FSMC_D11 ------PE14
FSMC_D12 ------PE15
FSMC_D13 ------PD8
FSMC_D14 ------PD9
FSMC_D15 ------PD10

LCD_BL(背光) ----PB0
FSMC_NE4(CS) --->PG12
FSMC_NWE(WR/CLK)--->PD5
FSMC_NOE(RD) --->PD4
FSMC_A10(RS) --->PG0
*/
void NT35310_LcdInit(void)
{
	 /*1. 开时钟*/
	 RCC->APB2ENR|=1<<3; //PB
	 RCC->APB2ENR|=1<<5; //PD
	 RCC->APB2ENR|=1<<6; //PE
	 RCC->APB2ENR|=1<<8; //PG
	
	 /*2. 配置模式*/
	 GPIOB->CRL&=0xFFFFFFF0;   //背光脚
	 GPIOB->CRL|=0x00000003;
	 
	 GPIOG->CRH&=0xFFF0FFFF;   //片选
	 GPIOG->CRH|=0x00030000;
	
	 GPIOD->CRL&=0xFF00FFFF;   //WR/RD
	 GPIOD->CRL|=0x00330000;
	
	 GPIOG->CRL&=0xFFFFFFF0;   //RS
	 GPIOG->CRL|=0x00000003;
	 
	 //配置数据线
	GPIOD->CRL&=0xFFFFFF00;
	GPIOD->CRL|=0x00000033;
	GPIOD->CRH&=0x00FFF000; 
	GPIOD->CRH|=0x33000333;
	GPIOE->CRL&=0x0FFFFFFF;
	GPIOE->CRL|=0x30000000;
	GPIOE->CRH&=0x00000000; 
	GPIOE->CRH|=0x33333333;
	
	/*3. 上拉GPIO口*/
	LCD_DATA_OC0=1;
	LCD_DATA_OC1=1;
	LCD_DATA_OC2=1;
	LCD_DATA_OC3=1;
	LCD_DATA_OC4=1;
	LCD_DATA_OC5=1;
	LCD_DATA_OC6=1;
	LCD_DATA_OC7=1;
	LCD_DATA_OC8=1;
	LCD_DATA_OC9=1;
	LCD_DATA_OC10=1;
	LCD_DATA_OC11=1;
	LCD_DATA_OC12=1;
	LCD_DATA_OC13=1;
	LCD_DATA_OC14=1;
	LCD_DATA_OC15=1;
	
	NT35310_LCD_CS=1;
	NT35310_LCD_RD=1;
	NT35310_LCD_RS=1;
	NT35310_LCD_WR=1;
	
	delay_ms(100);  //等待复位成功
	
	NT35310_LcdWriteReg(0xED);
	NT35310_LcdWriteData(0x01);
	NT35310_LcdWriteData(0xFE);
	NT35310_LcdWriteReg(0xEE);
	NT35310_LcdWriteData(0xDE);
	NT35310_LcdWriteData(0x21);
	NT35310_LcdWriteReg(0xF1);
	NT35310_LcdWriteData(0x01);
	NT35310_LcdWriteReg(0xDF);
	NT35310_LcdWriteData(0x10);
	//VCOMvoltage//
	NT35310_LcdWriteReg(0xC4);
	NT35310_LcdWriteData(0x8F); //5f
	NT35310_LcdWriteReg(0xC6);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xE2);
	NT35310_LcdWriteData(0xE2);
	NT35310_LcdWriteData(0xE2);
	NT35310_LcdWriteReg(0xBF);
	NT35310_LcdWriteData(0xAA);
	NT35310_LcdWriteReg(0xB0);
	NT35310_LcdWriteData(0x0D);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x0D);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x11);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x19);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x21);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x2D);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x3D);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x5D);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x5D);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xB1);
	NT35310_LcdWriteData(0x80);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x8B);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x96);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xB2);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x02);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x03);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xB3);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xB4);
	NT35310_LcdWriteData(0x8B);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x96);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xA1);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xB5);
	NT35310_LcdWriteData(0x02);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x03);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x04);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xB6);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xB7);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x3F);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x5E);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x64);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x8C);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xAC);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xDC);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x70);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x90);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xEB);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xDC);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xB8);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xBA);
	NT35310_LcdWriteData(0x24);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xC1);
	NT35310_LcdWriteData(0x20);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x54);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xFF);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xC2);
	NT35310_LcdWriteData(0x0A);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x04);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xC3);
	NT35310_LcdWriteData(0x3C);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x3A);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x39);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x37);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x3C);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x36);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x32);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x2F);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x2C);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x29);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x26);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x24);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x24);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x23);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x3C);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x36);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x32);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x2F);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x2C);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x29);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x26);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x24);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x24);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x23);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xC4);
	NT35310_LcdWriteData(0x62);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x05);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x84);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xF0);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x18);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xA4);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x18);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x50);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x0C);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x17);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x95);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xF3);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xE6);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xC5);
	NT35310_LcdWriteData(0x32);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x44);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x65);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x76);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x88);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xC6);
	NT35310_LcdWriteData(0x20);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x17);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x01);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xC7);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xC8);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xC9);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xE0);
	NT35310_LcdWriteData(0x16);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x1C);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x21);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x36);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x46);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x52);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x64);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x7A);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x8B);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x99);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xA8);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xB9);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xC4);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xCA);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xD2);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xD9);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xE0);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xF3);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xE1);
	NT35310_LcdWriteData(0x16);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x1C);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x22);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x36);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x45);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x52);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x64);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x7A);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x8B);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x99);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xA8);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xB9);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xC4);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xCA);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xD2);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xD8);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xE0);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xF3);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xE2);
	NT35310_LcdWriteData(0x05);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x0B);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x1B);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x34);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x44);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x4F);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x61);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x79);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x88);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x97);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xA6);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xB7);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xC2);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xC7);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xD1);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xD6);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xDD);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xF3);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xE3);
	NT35310_LcdWriteData(0x05);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xA);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x1C);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x33);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x44);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x50);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x62);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x78);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x88);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x97);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xA6);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xB7);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xC2);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xC7);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xD1);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xD5);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xDD);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xF3);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xE4);
	NT35310_LcdWriteData(0x01);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x01);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x02);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x2A);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x3C);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x4B);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x5D);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x74);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x84);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x93);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xA2);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xB3);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xBE);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xC4);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xCD);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xD3);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xDD);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xF3);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xE5);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x02);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x29);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x3C);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x4B);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x5D);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x74);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x84);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x93);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xA2);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xB3);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xBE);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xC4);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xCD);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xD3);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xDC);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xF3);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xE6);
	NT35310_LcdWriteData(0x11);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x34);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x56);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x76);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x77);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x66);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x88);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x99);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xBB);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x99);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x66);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x55);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x55);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x45);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x43);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x44);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xE7);
	NT35310_LcdWriteData(0x32);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x55);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x76);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x66);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x67);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x67);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x87);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x99);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xBB);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x99);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x77);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x44);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x56);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x23);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x33);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x45);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xE8);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x99);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x87);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x88);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x77);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x66);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x88);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xAA);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0xBB);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x99);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x66);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x55);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x55);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x44);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x44);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x55);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xE9);
	NT35310_LcdWriteData(0xAA);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0x00);
	NT35310_LcdWriteData(0xAA);
	NT35310_LcdWriteReg(0xCF);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xF0);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x50);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xF3);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0xF9);
	NT35310_LcdWriteData(0x06);
	NT35310_LcdWriteData(0x10);
	NT35310_LcdWriteData(0x29);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0x3A);
	NT35310_LcdWriteData(0x55);
	NT35310_LcdWriteReg(0x11);
	DelayMs(100);
	NT35310_LcdWriteReg(0x29);
	NT35310_LcdWriteReg(0x35);
	NT35310_LcdWriteData(0x00);
	NT35310_LcdWriteReg(0x51);
	NT35310_LcdWriteData(0xFF);
	NT35310_LcdWriteReg(0x53);
	NT35310_LcdWriteData(0x2C);
	NT35310_LcdWriteReg(0x55);
	NT35310_LcdWriteData(0x82);
	NT35310_LcdWriteReg(0x2c);
	
  NT35310_LCD_BL=1;  //开始背光
}


/*
函数功能: 写寄存器函数
*/
void NT35310_LcdWriteReg(u16 data)
{
		NT35310_LCD_CS=0; //拉低片选
	  NT35310_LCD_RS=0; //表示写命令
	  NT35310_LCD_WR=0; //表示写数据
    LCD_DATA_OC0=(data>>0)&0x01;  
	  LCD_DATA_OC1=(data>>1)&0x01;
	  LCD_DATA_OC2=(data>>2)&0x01;
	  LCD_DATA_OC3=(data>>3)&0x01;
	  LCD_DATA_OC4=(data>>4)&0x01;
	  LCD_DATA_OC5=(data>>5)&0x01;
	  LCD_DATA_OC6=(data>>6)&0x01;
	  LCD_DATA_OC7=(data>>7)&0x01;
	  LCD_DATA_OC8=(data>>8)&0x01;
	  LCD_DATA_OC9=(data>>9)&0x01;
	  LCD_DATA_OC10=(data>>10)&0x01;
	  LCD_DATA_OC11=(data>>11)&0x01;
	  LCD_DATA_OC12=(data>>12)&0x01;
	  LCD_DATA_OC13=(data>>13)&0x01;
	  LCD_DATA_OC14=(data>>14)&0x01;
	  LCD_DATA_OC15=(data>>15)&0x01;
	  NT35310_LCD_WR=1; //表示数据写完毕
	  NT35310_LCD_CS=1; //取消选中LCD屏
}


/*
函数功能: 写数据函数
*/
void NT35310_LcdWriteData(u16 data)
{
		NT35310_LCD_CS=0; //拉低片选
	  NT35310_LCD_RS=1; //表示写数据
	  NT35310_LCD_WR=0; //表示写数据
    LCD_DATA_OC0=(data>>0)&0x01;
	  LCD_DATA_OC1=(data>>1)&0x01;
	  LCD_DATA_OC2=(data>>2)&0x01;
	  LCD_DATA_OC3=(data>>3)&0x01;
	  LCD_DATA_OC4=(data>>4)&0x01;
	  LCD_DATA_OC5=(data>>5)&0x01;
	  LCD_DATA_OC6=(data>>6)&0x01;
	  LCD_DATA_OC7=(data>>7)&0x01;
	  LCD_DATA_OC8=(data>>8)&0x01;
	  LCD_DATA_OC9=(data>>9)&0x01;
	  LCD_DATA_OC10=(data>>10)&0x01;
	  LCD_DATA_OC11=(data>>11)&0x01;
	  LCD_DATA_OC12=(data>>12)&0x01;
	  LCD_DATA_OC13=(data>>13)&0x01;
	  LCD_DATA_OC14=(data>>14)&0x01;
	  LCD_DATA_OC15=(data>>15)&0x01;
	  
	  NT35310_LCD_WR=1; //表示数据写完毕
	  NT35310_LCD_CS=1; //取消选中LCD屏
}

/*
函数功能: 设置LCD屏的光标位置
*/
void NT35310_SetPos(u32 x,u32 y)
{
		NT35310_LcdWriteReg(0x2A); //设置X坐标
	  NT35310_LcdWriteData(x>>8);
	  NT35310_LcdWriteData(x&0xFF);
	  
	  NT35310_LcdWriteReg(0x2B); //设置Y坐标
	  NT35310_LcdWriteData(y>>8);
	  NT35310_LcdWriteData(y&0xFF);
}

/*
函数功能: 封装画点函数
函数参数：
		    u32 x,u32 y,u16 c
*/
void NT35310_DrawPoint(u32 x,u32 y,u16 c)
{
		NT35310_SetPos(x,y);
	  NT35310_LcdWriteReg(0x2C); //写颜色值
		NT35310_LcdWriteData(c); //写入实际的颜色值
}

/*
函数功能: LCD的清屏函数
*/
void NT35310_Clear(u16 c)
{
	 u32 i;
	 NT35310_SetPos(0,0);
	 NT35310_LcdWriteReg(0x2C); //写颜色值
	 for(i=0;i<320*480;i++)NT35310_LcdWriteData(c); //写入实际的颜色值
}

/*
函数功能: 显示数据
          u32 x  范围0~319
          u32 y  范围0~479
          u32 w  数据的宽度(必须是8的倍数)
          u32 h  数据的高度
          u8 *p  数据
说明: 取模横向坐标必须保证是8的倍数
*/
void NT35310_DisplayData(u32 x,u32 y,u32 w,u32 h,u8 *p)
{
   u16 i,j,x0=x;
   u8 data;
   for(i=0;i<w/8*h;i++) //取出的模型总字节数
   {
       data=p[i]; //取出数组里一个字节的数据
       for(j=0;j<8;j++)
       {
          if(data&0x80)NT35310_DrawPoint(x0,y,RED); //字体颜色
        //  else NT35310_DrawPoint(x0,y,0); //背景颜色
          data<<=1; //继续判断下一位
          x0++; //继续画下一个点
       }
       if(x0-x==w) //判断是否需要换行
       {
          x0=x;//横坐标归位
          y++; //纵坐标自增
       }
   }
}
