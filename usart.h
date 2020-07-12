#ifndef _USART_H
#define _USART_H
#include "stm32f10x.h"
#include <stdio.h>
#include <stdarg.h>
#include "sys.h"
#include <string.h>

#define USART1_RX_LENGTH 1024
extern u8 USART1_RX_BUFFER[USART1_RX_LENGTH]; //����������ݵĻ�����
extern u32 USART1_RX_CNT;  //��ǰ���յ������ݳ���
extern u8 USART1_RX_FLAG; //1��ʾ���ݽ������ 0��ʾû�н������

#define USART2_RX_LENGTH 1024
extern u8 USART2_RX_BUFFER[USART2_RX_LENGTH]; //����������ݵĻ�����
extern u32 USART2_RX_CNT;  //��ǰ���յ������ݳ���
extern u8 USART2_RX_FLAG; //1��ʾ���ݽ������ 0��ʾû�н������

#define USART3_RX_LENGTH 1024
extern u8 USART3_RX_BUFFER[USART3_RX_LENGTH]; //����������ݵĻ�����
extern u32 USART3_RX_CNT;  //��ǰ���յ������ݳ���
extern u8 USART3_RX_FLAG; //1��ʾ���ݽ������ 0��ʾû�н������

void USART1_Init(u32 baud);
void USART2_Init(u32 baud);
void USART3_Init(u32 baud);
void USARTx_StringSend(USART_TypeDef *USARTx,char *str);
void USART1_Printf(char *fmt,...);
void USARTx_DataSend(USART_TypeDef *USARTx,u8 *data,u32 len);
#endif
