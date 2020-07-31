#ifndef _IIC_H
#define _IIC_H
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"

//1��ʾģ��ʱ��  0��ʾӲ��ʱ��
#define IIC_MODE 0 

#if IIC_MODE
//������
#define IIC_SDA_OUT  PBout(7)
#define IIC_SDA_IN PBin(7)

//ʱ����
#define IIC_SCL PBout(6)

//������ģʽ�л�
#define IIC_INPUT_MODE()  {GPIOB->CRL&=0x0FFFFFFF;GPIOB->CRL|=0x80000000;}
#define IIC_OUTPUT_MODE() {GPIOB->CRL&=0x0FFFFFFF;GPIOB->CRL|=0x30000000;}

//��������
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
