#ifndef _W25Q64_H
#define _W25Q64_H
#include "stm32f10x.h"
#include "sys.h"
#include "spi.h"

void W25Q64_Init(void);
u16 W25Q64_ReadDeviceID(void);
void W25Q64_PageWrite(u32 addr,u32 len,u8 *buff);
void W25Q64_SectorErase(u32 addr);
void W25Q64_ReadData(u32 addr,u32 len,u8 *buff);
void W25Q64_WriteData(u32 addr,u32 len,u8 *buff);
#endif
