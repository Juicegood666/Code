#ifndef _IIC_H
#define _IIC_H
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"

//1表示模拟时序  0表示硬件时序
#define IIC_MODE 0 

#if IIC_MODE
//数据线
#define IIC_SDA_OUT  PBout(7)
#define IIC_SDA_IN PBin(7)

//时钟线
#define IIC_SCL PBout(6)

//数据线模式切换
#define IIC_INPUT_MODE()  {GPIOB->CRL&=0x0FFFFFFF;GPIOB->CRL|=0x80000000;}
#define IIC_OUTPUT_MODE() {GPIOB->CRL&=0x0FFFFFFF;GPIOB->CRL|=0x30000000;}

//函数声明
u8 IIC_RecvOneByte(void);
void IIC_SendOneByte(u8 data);
void IIC_SendAck(u8 ack);
u8 IIC_GetAck(void);
void IIC_SendStop(void);
void IIC_SendStart(void);
void IIC_Init(void);

#else
void IIC_Init(void);
void IIC_SendStart(void);
void IIC_SendStop(void);
void IIC_SendAddr(u8 addr);
void IIC_SendOneByte(u8 addr);
u8 IIC_RecvOneByte(void);
#endif


#endif
