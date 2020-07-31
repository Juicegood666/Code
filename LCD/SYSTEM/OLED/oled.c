#include "oled.h"
/*
函数功能: OLED显示屏GPIO口初始化
硬件连接:
        D0---PB14--SCL
        D1---PB13--MOSI
        RES--PB12
        DC---PB1
        CS---PA7
*/
void OLED_GPIO_Init(void)
{
  RCC->APB2ENR|=1<<2;//PA
  RCC->APB2ENR|=1<<3;//PB
  GPIOA->CRL&=0x0FFFFFFF;
  GPIOA->CRL|=0x30000000;
  GPIOB->CRL&=0xFFFFFF0F;
  GPIOB->CRL|=0x00000030;
  GPIOB->CRH&=0xF000FFFF;
  GPIOB->CRH|=0x03330000;
  //设置默认电平
  OLED_SCL=1;
  OLED_CS=1;
}

/*
函数功能:底层SPI发送一个字节
函数参数:
        u8 data :发送的数据
        u8 cmd  :0表示命令 1表示数据
*/
void OLED_WriteOneByte(u8 data,u8 cmd)
{
   u8 i;
   /*1. 区分发送的是数据还是命令*/
   if(cmd)OLED_DC=1;
   else OLED_DC=0;
   /*2. 循环发送8个位*/
   OLED_CS=0; //选中OLED显示屏
   for(i=0;i<8;i++)
   {
      if(data&0x80)OLED_MOSI=1;
      else OLED_MOSI=0;
      OLED_SCL=0;
      OLED_SCL=1;
      data<<=1; //继续发送下一位
   }
    OLED_CS=1; //取消选中OLED显示屏
    OLED_DC=1;
}

/*
函数功能: 初始化OLED显示屏
*/
void OLED_Init(void)
{
    //初始化OLED显示屏的GPIO口
    OLED_GPIO_Init();
  
    OLED_RES=1;
    delay_ms(100);
    OLED_RES=0;
    delay_ms(100);
    OLED_RES=1;
    delay_ms(100);
    
    OLED_WriteOneByte(0xAE,OLED_W_CMD); //关闭显示
    
    OLED_WriteOneByte(0x00,OLED_W_CMD);
    OLED_WriteOneByte(0x10,OLED_W_CMD);
  
    OLED_WriteOneByte(0x40,OLED_W_CMD);
  
    OLED_WriteOneByte(0xB0,OLED_W_CMD);
  
    OLED_WriteOneByte(0x81,OLED_W_CMD);
    OLED_WriteOneByte(0xCF,OLED_W_CMD);
  
    OLED_WriteOneByte(0xA1,OLED_W_CMD);
  
    OLED_WriteOneByte(0xA6,OLED_W_CMD);
    
    OLED_WriteOneByte(0xA8,OLED_W_CMD);
    OLED_WriteOneByte(0x3F,OLED_W_CMD);
    
    OLED_WriteOneByte(0xC8,OLED_W_CMD);
    
    OLED_WriteOneByte(0xD3,OLED_W_CMD);
    OLED_WriteOneByte(0x00,OLED_W_CMD);
    
    OLED_WriteOneByte(0xD5,OLED_W_CMD);
    OLED_WriteOneByte(0x80,OLED_W_CMD);
    
    OLED_WriteOneByte(0xD9,OLED_W_CMD);
    OLED_WriteOneByte(0xF1,OLED_W_CMD);
    
    OLED_WriteOneByte(0xDA,OLED_W_CMD);
    OLED_WriteOneByte(0x12,OLED_W_CMD);
    
    OLED_WriteOneByte(0xDB,OLED_W_CMD);
    OLED_WriteOneByte(0x30,OLED_W_CMD);
    
    OLED_WriteOneByte(0x8D,OLED_W_CMD);
    OLED_WriteOneByte(0x14,OLED_W_CMD);
    
    OLED_WriteOneByte(0xAF,OLED_W_CMD); //开启显示
}

/*
函数功能: 设置OLED显示屏的光标位置
函数参数: x,y表示坐标的位置
          y填入的范围(0~7)
          x填入的范围(0~127)
*/
void OLED_SetPos(u16 x,u16 y)
{
    OLED_WriteOneByte(0xB0+y,OLED_W_CMD); //设置页地址
    OLED_WriteOneByte((x&0xF0)>>4|0x10,OLED_W_CMD);//设置上列地址(高4位)
    OLED_WriteOneByte((x&0x0F)|0x00,OLED_W_CMD);   //设置下列地址(低4位)
}

/*
函数功能: OLED显示屏的清屏函数
*/
void OLED_Clear(u8 data)
{
    u8 i,j;
    for(i=0;i<8;i++) //页地址
    {
        OLED_WriteOneByte(0xB0+i,OLED_W_CMD);//设置页地址
        OLED_WriteOneByte(0x10,OLED_W_CMD);  //设置上列地址(高4位)
        OLED_WriteOneByte(0x00,OLED_W_CMD);  //设置下列地址(低4位)

        //循环写128列的数据
        for(j=0;j<128;j++)OLED_WriteOneByte(data,OLED_W_DAT);
    }
}

///*
//函数功能: 显示数据
//          u16 x  范围0~127
//          u16 y  范围0~7
//          u8 *p  数据
//说明: 16*16的数据
//*/
//void OLED_DisplayData(u16 x,u16 y,u8 *p)
//{
//    u8 i;
//    /*1. 显示上半部分*/
//    OLED_SetPos(x,y); //设置光标位置
//    for(i=0;i<16;i++)
//    {
//       OLED_WriteOneByte(p[i],OLED_W_DAT); 
//    }
//    
//    /*2. 显示下半部分*/
//    OLED_SetPos(x,y+1); //设置光标位置
//    for(i=0;i<16;i++)
//    {
//       OLED_WriteOneByte(p[i+16],OLED_W_DAT); 
//    }
//}

/*
函数功能: 显示数据
          u16 x  范围0~127
          u16 y  范围0~63
          u16 w  数据的宽度(必须是8的倍数)
          u16 h  数据的高度
          u8 *p  数据
说明: 取模横向坐标必须保证是8的倍数
*/
void OLED_DisplayData(u16 x,u16 y,u16 w,u16 h,u8 *p)
{
   u16 i,j,x0=x;
   u8 data;
   for(i=0;i<w/8*h;i++) //取出的模型总字节数
   {
       data=p[i]; //取出数组里一个字节的数据
       for(j=0;j<8;j++)
       {
          if(data&0x80)OLED_DrawPoint(x0,y,1);
          else OLED_DrawPoint(x0,y,0);
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


//定义OLED显示屏虚拟缓冲区
u8 OLED_GRAM[8][128];
/*
函数功能: OLED画点函数
函数参数:
    u16 x --横坐标 0~127
    u16 y---纵坐标 0~63
    u8  c---0表示灭、1表示亮
比如: OLED_DrawPoint(3,2,1);
*/
void OLED_DrawPoint(u16 x,u16 y,u8 c)
{
    u8 page=y/8; //得到页的位置
    y=y%8; //限制范围是在0~7之间 
    if(c)OLED_GRAM[page][x]|=1<<y;
    else OLED_GRAM[page][x]&=~(1<<y);
}

/*
函数功能: 更新虚拟缓冲的数据到OLED显示屏
*/
void OLED_Refresh_GRAM(void)
{
    u8 i,j;
    for(i=0;i<8;i++)
    {
        OLED_WriteOneByte(0xB0+i,OLED_W_CMD);//设置页地址
        OLED_WriteOneByte(0x10,OLED_W_CMD);  //设置上列地址(高4位)
        OLED_WriteOneByte(0x00,OLED_W_CMD);  //设置下列地址(低4位)

        //循环写128列的数据
        for(j=0;j<128;j++)OLED_WriteOneByte(OLED_GRAM[i][j],OLED_W_DAT);
    }
}


/*
函数功能: 清除虚拟缓冲区的数据
*/
void OLED_Clear_GRAM(void)
{
    memset(OLED_GRAM,0,sizeof(OLED_GRAM));
}


/*
函数功能: 显示字符串
          u16 x  范围0~127
          u16 y  范围0~63
          u16 szie 字体的大小
          u8 *str  将要显示的字符串
说明: 取模横向坐标必须保证是8的倍数。
ASCII码数据是中文一半。
示例: OLED_DisplayString(10,10,16,"abcd12345")

常用的ASCII是: ' ' 到 '~'

 !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
8*16 ------>16--->16/8=8
*/
void OLED_DisplayString(u16 x,u16 y,u16 size,u8 *str)
{
   u16 x0=x;
   while(*str!='\0')
   {
      switch(size)
      {
        case 16:
          OLED_DisplayData(x0,y,size/2,size,(u8*)ASCII_8_16[*str-' ']);
          break;
        /*......*/
      }
      str++;//继续显示下一个数据
      x0+=size/2; //横坐标自增
      
      if(x0>127)
      {
          x0=x; //横坐标归位
          y+=size;//纵坐标自增
      }
      if(y>63)return; //当超出屏幕范围，就强制结束函数
   } 
}

