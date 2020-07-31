#ifndef _USART_H_
#define _USART_H_
#include "stm32f10x.h"
#include "sys.h"
#include <stdio.h>
#include "beep.h"
#include <string.h>

#define USART1_RX_len  200
extern u8 USART1_RX_BUFF[USART1_RX_len]; 
extern u8 USART1_RX_CNT; 
extern u8 USART1_RX_FLAG; //串口接收标识位  0代表没有接收完毕 1代表接收完毕

#define USART2_RX_len  200
extern u8 USART2_RX_BUFF[USART2_RX_len]; 
extern u8 USART2_RX_CNT; 
extern u8 USART2_RX_FLAG; //串口接收标识位  0代表没有接收完毕 1代表接收完毕

#define USART3_RX_len  1024
extern u8 USART3_RX_BUFF[USART3_RX_len]; 
extern u16 USART3_RX_CNT; 
extern u8 USART3_RX_FLAG; //串口接收标识位  0代表没有接收完毕 1代表接收完毕

void USART1_Init(u32 baud);
void USART1_SendString(u8 *str);
void USART_X_Init(USART_TypeDef *USARTx,u32 psc,u32 baud);
void USART_X_SendString(USART_TypeDef *USARTx,char *str);
void USART_X_SendData(USART_TypeDef *USARTx,u8 *data,u32 len);
#endif
