#include "oled.h"
/*
函数功能: 初始化OLED显示屏硬件
硬件连接:
D0--PB14--时钟线
D1--PB13--数据线
RES-PB12-复位脚
DC--PB1--命令数据选择脚
CS--PA7--片选
*/
void OLED_Init(void)
{
    /*1. 开时钟*/
    RCC->APB2ENR|=1<<2; //PA
    RCC->APB2ENR|=1<<3; //PB
    /*2. 配置GPIO口模式*/
    GPIOB->CRL&=0xFFFFFF0F;
    GPIOB->CRL|=0x00000030;
    GPIOB->CRH&=0xF000FFFF;
    GPIOB->CRH|=0x03330000;
    GPIOA->CRL&=0x0FFFFFFF;
    GPIOA->CRL|=0x30000000;
    /*配置默认电平*/
    OLED_CS=1;
    OLED_SCK=1;
    
    /*OLED显示屏驱动代码*/
    OLED_RES=1;
    delay_ms(100);
    OLED_RES=0;
    delay_ms(100);
    OLED_RES=1;
    delay_ms(100);
    OLED_SPI_WriteOneByte(0xAE,OLED_WRITE_CMD);
    
    OLED_SPI_WriteOneByte(0x00,OLED_WRITE_CMD);
    OLED_SPI_WriteOneByte(0x10,OLED_WRITE_CMD);
    
    OLED_SPI_WriteOneByte(0x40,OLED_WRITE_CMD);
    
    OLED_SPI_WriteOneByte(0xB0,OLED_WRITE_CMD);
    
    OLED_SPI_WriteOneByte(0x81,OLED_WRITE_CMD);
    OLED_SPI_WriteOneByte(0xCF,OLED_WRITE_CMD);
    
    OLED_SPI_WriteOneByte(0xA1,OLED_WRITE_CMD);
    
    OLED_SPI_WriteOneByte(0xA6,OLED_WRITE_CMD);
    
    OLED_SPI_WriteOneByte(0xA8,OLED_WRITE_CMD);
    OLED_SPI_WriteOneByte(0x3F,OLED_WRITE_CMD);
    
    OLED_SPI_WriteOneByte(0xC8,OLED_WRITE_CMD);
    
    OLED_SPI_WriteOneByte(0xD3,OLED_WRITE_CMD);
    OLED_SPI_WriteOneByte(0x00,OLED_WRITE_CMD);
    
    OLED_SPI_WriteOneByte(0xD5,OLED_WRITE_CMD);
    OLED_SPI_WriteOneByte(0x80,OLED_WRITE_CMD);
    
    OLED_SPI_WriteOneByte(0xD9,OLED_WRITE_CMD);
    OLED_SPI_WriteOneByte(0xF1,OLED_WRITE_CMD);
    
    OLED_SPI_WriteOneByte(0xDA,OLED_WRITE_CMD);
    OLED_SPI_WriteOneByte(0x12,OLED_WRITE_CMD);
    
    OLED_SPI_WriteOneByte(0xDB,OLED_WRITE_CMD);
    OLED_SPI_WriteOneByte(0x30,OLED_WRITE_CMD);
    
    OLED_SPI_WriteOneByte(0x8D,OLED_WRITE_CMD);
    OLED_SPI_WriteOneByte(0x14,OLED_WRITE_CMD);
    
    OLED_SPI_WriteOneByte(0xAF,OLED_WRITE_CMD);
}

/*
函数功能: SPI底层发送一个字节的函数
函数参数:data 表示发送的数据  flag表示发送命令还是数据(0表示命令 1表示数据)
*/
void OLED_SPI_WriteOneByte(u8 data,u8 flag)
{
    u8 i;
    OLED_DC=flag; //区分发送数据还是命令
    OLED_CS=0;  //选中OLED显示屏
    for(i=0;i<8;i++)
    {
        OLED_SCK=0; //表示发送数据
        if(data&0x80) //先发高位
        {
            OLED_MOSI=1;
        }
        else 
        {
            OLED_MOSI=0;
        }
        data<<=1;
        OLED_SCK=1; //表示发送完毕
    }
    OLED_CS=1;  //取消选中OLED显示屏
}

/*
函数功能: OLED清屏函数
函数参数: data=0xFF 表示全亮   data=0x00 表示全灭
*/
void OLED_Clear(u8 data)
{
    memset(OLED_GRAM,data,sizeof(OLED_GRAM));
}

/*
函数功能: 设置光标的位置
函数参数:
u8 x 横向坐标--列地址(0~127)
u8 y 纵坐标--页地址(0~7)
*/
void OLED_SetPos(u8 x,u8 y)
{
    OLED_SPI_WriteOneByte(0xB0+y,OLED_WRITE_CMD); //设置页地址
    OLED_SPI_WriteOneByte(((x&0xF0)>>4)|0x10,OLED_WRITE_CMD); //设置上列地址
    OLED_SPI_WriteOneByte((x&0x0F)|0x00,OLED_WRITE_CMD); //设置下列地址
}

/*
函数功能: 显示数据---取模
取模方式：横向取模
*/
void OLED_DisplayData(u8 x,u8 y,u8 w,u8 h,u8 *p)
{
    u8 i,j,data;
    u8 x0=x;
    for(i=0;i<w/8*h;i++)
    {
        data=p[i];
        for(j=0;j<8;j++)
        {
            if(data&0x80)
            {
                OLED_DisplayPoint(x0,y,1); //画前景色--开灯
            }
            else
            {
                OLED_DisplayPoint(x0,y,0); //画背景色--关灯
            }
            x0++;
            data<<=1;
        }
        
        //可以换行
        if(x0-x==w)
        {
            x0=x; //横坐标归位
            y++;  //换行
        }
    }
}

u8 OLED_GRAM[8][128]; //8行128列---->8页128列

/*
函数功能: 画点函数
函数参数:
u8 x 横坐标 0~127
u8 y 纵坐标 0~63
u8 c 显示值(0灭  1亮)
*/
void OLED_DisplayPoint(u8 x,u8 y,u8 c)
{
    u8 page=y/8;//0~7
    y=y%8;
    if(c)
    {
       OLED_GRAM[page][x]|=1<<y; 
    }
    else
    {
       OLED_GRAM[page][x]&=~(1<<y);
    } 
}

/*
函数功能: OLED显示屏数据写函数
将显存数组里的所有数据写到OLED显示屏
*/
void OLED_WriteGRAM(void)
{
  u8 i,j;
    for(i=0;i<8;i++)
    {
        OLED_SPI_WriteOneByte(0xB0+i,OLED_WRITE_CMD); //设置页地址
        OLED_SPI_WriteOneByte(0x10,OLED_WRITE_CMD); //设置上列地址
        OLED_SPI_WriteOneByte(0x00,OLED_WRITE_CMD); //设置下列地址
        for(j=0;j<128;j++)
        {
            OLED_SPI_WriteOneByte(OLED_GRAM[i][j],OLED_WRITE_DAT);
        }
    } 
}

/*
函数功能: 显示字符串
显示的范围: !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
*/
void OLED_DisplayString(u8 x,u8 y,char *str)
{
    u8 x0=x;
    while(*str!='\0')
    {
        OLED_DisplayData(x0,y,8,16,(u8*)ASCII_8x16[*str-' ']);
        x0+=8;
        if(x0>127)  //判断是否换行
        {
            x0=x;
            y+=16; 
        }
        if(y>63)return; //判断超出屏幕显示范围
        str++; //显示下一个字符
    }
}

