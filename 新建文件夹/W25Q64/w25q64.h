#ifndef _W25Q64_H
#define _W25Q64_H
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "spi.h"
#define W25q64_CS PBout(12)
#define W25Q64_SCK PBout(13)
#define W25Q64_MISO PBin(14)
#define W25Q64_MOSI PBout(15)
void W25Q64_Init(void);
u16 W25Q64_Read_ID(void);
void W25Q64_Write_cmd(void);
u8 W25Q64_Read_State(void);
void W25Q64_Write_Page(u32 addr,u8 *buff,u32 len);
void W25Q64_Read_Data(u32 addr,u8 *buff,u32 len);
void W25Q64_Sector_Erase(u32 addr);
void W25Q64_Write_Nowipe(u32 addr,u8 *buff,u32 len);
void W25Q64_Write(u32 addr,u8 *buff,u32 len);
#endif
