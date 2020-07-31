#include "oled.h"
/*
��������: OLED��ʾ��GPIO�ڳ�ʼ��
Ӳ������:
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
  //����Ĭ�ϵ�ƽ
  OLED_SCL=1;
  OLED_CS=1;
}

/*
��������:�ײ�SPI����һ���ֽ�
��������:
        u8 data :���͵�����
        u8 cmd  :0��ʾ���� 1��ʾ����
*/
void OLED_WriteOneByte(u8 data,u8 cmd)
{
   u8 i;
   /*1. ���ַ��͵������ݻ�������*/
   if(cmd)OLED_DC=1;
   else OLED_DC=0;
   /*2. ѭ������8��λ*/
   OLED_CS=0; //ѡ��OLED��ʾ��
   for(i=0;i<8;i++)
   {
      if(data&0x80)OLED_MOSI=1;
      else OLED_MOSI=0;
      OLED_SCL=0;
      OLED_SCL=1;
      data<<=1; //����������һλ
   }
    OLED_CS=1; //ȡ��ѡ��OLED��ʾ��
    OLED_DC=1;
}

/*
��������: ��ʼ��OLED��ʾ��
*/
void OLED_Init(void)
{
    //��ʼ��OLED��ʾ����GPIO��
    OLED_GPIO_Init();
  
    OLED_RES=1;
    delay_ms(100);
    OLED_RES=0;
    delay_ms(100);
    OLED_RES=1;
    delay_ms(100);
    
    OLED_WriteOneByte(0xAE,OLED_W_CMD); //�ر���ʾ
    
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
    
    OLED_WriteOneByte(0xAF,OLED_W_CMD); //������ʾ
}

/*
��������: ����OLED��ʾ���Ĺ��λ��
��������: x,y��ʾ�����λ��
          y����ķ�Χ(0~7)
          x����ķ�Χ(0~127)
*/
void OLED_SetPos(u16 x,u16 y)
{
    OLED_WriteOneByte(0xB0+y,OLED_W_CMD); //����ҳ��ַ
    OLED_WriteOneByte((x&0xF0)>>4|0x10,OLED_W_CMD);//�������е�ַ(��4λ)
    OLED_WriteOneByte((x&0x0F)|0x00,OLED_W_CMD);   //�������е�ַ(��4λ)
}

/*
��������: OLED��ʾ������������
*/
void OLED_Clear(u8 data)
{
    u8 i,j;
    for(i=0;i<8;i++) //ҳ��ַ
    {
        OLED_WriteOneByte(0xB0+i,OLED_W_CMD);//����ҳ��ַ
        OLED_WriteOneByte(0x10,OLED_W_CMD);  //�������е�ַ(��4λ)
        OLED_WriteOneByte(0x00,OLED_W_CMD);  //�������е�ַ(��4λ)

        //ѭ��д128�е�����
        for(j=0;j<128;j++)OLED_WriteOneByte(data,OLED_W_DAT);
    }
}

///*
//��������: ��ʾ����
//          u16 x  ��Χ0~127
//          u16 y  ��Χ0~7
//          u8 *p  ����
//˵��: 16*16������
//*/
//void OLED_DisplayData(u16 x,u16 y,u8 *p)
//{
//    u8 i;
//    /*1. ��ʾ�ϰ벿��*/
//    OLED_SetPos(x,y); //���ù��λ��
//    for(i=0;i<16;i++)
//    {
//       OLED_WriteOneByte(p[i],OLED_W_DAT); 
//    }
//    
//    /*2. ��ʾ�°벿��*/
//    OLED_SetPos(x,y+1); //���ù��λ��
//    for(i=0;i<16;i++)
//    {
//       OLED_WriteOneByte(p[i+16],OLED_W_DAT); 
//    }
//}

/*
��������: ��ʾ����
          u16 x  ��Χ0~127
          u16 y  ��Χ0~63
          u16 w  ���ݵĿ��(������8�ı���)
          u16 h  ���ݵĸ߶�
          u8 *p  ����
˵��: ȡģ����������뱣֤��8�ı���
*/
void OLED_DisplayData(u16 x,u16 y,u16 w,u16 h,u8 *p)
{
   u16 i,j,x0=x;
   u8 data;
   for(i=0;i<w/8*h;i++) //ȡ����ģ�����ֽ���
   {
       data=p[i]; //ȡ��������һ���ֽڵ�����
       for(j=0;j<8;j++)
       {
          if(data&0x80)OLED_DrawPoint(x0,y,1);
          else OLED_DrawPoint(x0,y,0);
          data<<=1; //�����ж���һλ
          x0++; //��������һ����
       }
       if(x0-x==w) //�ж��Ƿ���Ҫ����
       {
          x0=x;//�������λ
          y++; //����������
       }
   }
}


//����OLED��ʾ�����⻺����
u8 OLED_GRAM[8][128];
/*
��������: OLED���㺯��
��������:
    u16 x --������ 0~127
    u16 y---������ 0~63
    u8  c---0��ʾ��1��ʾ��
����: OLED_DrawPoint(3,2,1);
*/
void OLED_DrawPoint(u16 x,u16 y,u8 c)
{
    u8 page=y/8; //�õ�ҳ��λ��
    y=y%8; //���Ʒ�Χ����0~7֮�� 
    if(c)OLED_GRAM[page][x]|=1<<y;
    else OLED_GRAM[page][x]&=~(1<<y);
}

/*
��������: �������⻺������ݵ�OLED��ʾ��
*/
void OLED_Refresh_GRAM(void)
{
    u8 i,j;
    for(i=0;i<8;i++)
    {
        OLED_WriteOneByte(0xB0+i,OLED_W_CMD);//����ҳ��ַ
        OLED_WriteOneByte(0x10,OLED_W_CMD);  //�������е�ַ(��4λ)
        OLED_WriteOneByte(0x00,OLED_W_CMD);  //�������е�ַ(��4λ)

        //ѭ��д128�е�����
        for(j=0;j<128;j++)OLED_WriteOneByte(OLED_GRAM[i][j],OLED_W_DAT);
    }
}


/*
��������: ������⻺����������
*/
void OLED_Clear_GRAM(void)
{
    memset(OLED_GRAM,0,sizeof(OLED_GRAM));
}


/*
��������: ��ʾ�ַ���
          u16 x  ��Χ0~127
          u16 y  ��Χ0~63
          u16 szie ����Ĵ�С
          u8 *str  ��Ҫ��ʾ���ַ���
˵��: ȡģ����������뱣֤��8�ı�����
ASCII������������һ�롣
ʾ��: OLED_DisplayString(10,10,16,"abcd12345")

���õ�ASCII��: ' ' �� '~'

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
      str++;//������ʾ��һ������
      x0+=size/2; //����������
      
      if(x0>127)
      {
          x0=x; //�������λ
          y+=size;//����������
      }
      if(y>63)return; //��������Ļ��Χ����ǿ�ƽ�������
   } 
}

