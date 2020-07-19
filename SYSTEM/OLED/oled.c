#include "oled.h"
/*
��������: ��ʼ��OLED��ʾ��Ӳ��
Ӳ������:
D0--PB14--ʱ����
D1--PB13--������
RES-PB12-��λ��
DC--PB1--��������ѡ���
CS--PA7--Ƭѡ
*/
void OLED_Init(void)
{
    /*1. ��ʱ��*/
    RCC->APB2ENR|=1<<2; //PA
    RCC->APB2ENR|=1<<3; //PB
    /*2. ����GPIO��ģʽ*/
    GPIOB->CRL&=0xFFFFFF0F;
    GPIOB->CRL|=0x00000030;
    GPIOB->CRH&=0xF000FFFF;
    GPIOB->CRH|=0x03330000;
    GPIOA->CRL&=0x0FFFFFFF;
    GPIOA->CRL|=0x30000000;
    /*����Ĭ�ϵ�ƽ*/
    OLED_CS=1;
    OLED_SCK=1;
    
    /*OLED��ʾ����������*/
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
��������: SPI�ײ㷢��һ���ֽڵĺ���
��������:data ��ʾ���͵�����  flag��ʾ�������������(0��ʾ���� 1��ʾ����)
*/
void OLED_SPI_WriteOneByte(u8 data,u8 flag)
{
    u8 i;
    OLED_DC=flag; //���ַ������ݻ�������
    OLED_CS=0;  //ѡ��OLED��ʾ��
    for(i=0;i<8;i++)
    {
        OLED_SCK=0; //��ʾ��������
        if(data&0x80) //�ȷ���λ
        {
            OLED_MOSI=1;
        }
        else 
        {
            OLED_MOSI=0;
        }
        data<<=1;
        OLED_SCK=1; //��ʾ�������
    }
    OLED_CS=1;  //ȡ��ѡ��OLED��ʾ��
}

/*
��������: OLED��������
��������: data=0xFF ��ʾȫ��   data=0x00 ��ʾȫ��
*/
void OLED_Clear(u8 data)
{
    memset(OLED_GRAM,data,sizeof(OLED_GRAM));
}

/*
��������: ���ù���λ��
��������:
u8 x ��������--�е�ַ(0~127)
u8 y ������--ҳ��ַ(0~7)
*/
void OLED_SetPos(u8 x,u8 y)
{
    OLED_SPI_WriteOneByte(0xB0+y,OLED_WRITE_CMD); //����ҳ��ַ
    OLED_SPI_WriteOneByte(((x&0xF0)>>4)|0x10,OLED_WRITE_CMD); //�������е�ַ
    OLED_SPI_WriteOneByte((x&0x0F)|0x00,OLED_WRITE_CMD); //�������е�ַ
}

/*
��������: ��ʾ����---ȡģ
ȡģ��ʽ������ȡģ
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
                OLED_DisplayPoint(x0,y,1); //��ǰ��ɫ--����
            }
            else
            {
                OLED_DisplayPoint(x0,y,0); //������ɫ--�ص�
            }
            x0++;
            data<<=1;
        }
        
        //���Ի���
        if(x0-x==w)
        {
            x0=x; //�������λ
            y++;  //����
        }
    }
}

u8 OLED_GRAM[8][128]; //8��128��---->8ҳ128��

/*
��������: ���㺯��
��������:
u8 x ������ 0~127
u8 y ������ 0~63
u8 c ��ʾֵ(0��  1��)
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
��������: OLED��ʾ������д����
���Դ����������������д��OLED��ʾ��
*/
void OLED_WriteGRAM(void)
{
  u8 i,j;
    for(i=0;i<8;i++)
    {
        OLED_SPI_WriteOneByte(0xB0+i,OLED_WRITE_CMD); //����ҳ��ַ
        OLED_SPI_WriteOneByte(0x10,OLED_WRITE_CMD); //�������е�ַ
        OLED_SPI_WriteOneByte(0x00,OLED_WRITE_CMD); //�������е�ַ
        for(j=0;j<128;j++)
        {
            OLED_SPI_WriteOneByte(OLED_GRAM[i][j],OLED_WRITE_DAT);
        }
    } 
}

/*
��������: ��ʾ�ַ���
��ʾ�ķ�Χ: !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
*/
void OLED_DisplayString(u8 x,u8 y,char *str)
{
    u8 x0=x;
    while(*str!='\0')
    {
        OLED_DisplayData(x0,y,8,16,(u8*)ASCII_8x16[*str-' ']);
        x0+=8;
        if(x0>127)  //�ж��Ƿ���
        {
            x0=x;
            y+=16; 
        }
        if(y>63)return; //�жϳ�����Ļ��ʾ��Χ
        str++; //��ʾ��һ���ַ�
    }
}

