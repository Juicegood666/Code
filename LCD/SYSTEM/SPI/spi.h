#ifndef SPI_H
#define SPI_H
#include "stm32f10x.h"
#include "sys.h"

//1表示模拟时序  0表示硬件时序
#define SPI2_MODE 0 

u8 SPI2_ReadWriteOneByte(u8 tx_data);
void SPI2_Init(void);
#endif
