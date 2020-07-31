#ifndef _AT24C08_H
#define _AT24C08_H
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "iic.h"

//µØÖ·
#define AT24C08_WRITE_ADDR 0xA0
#define AT24C08_READ_ADDR 0xA1

//º¯ÊýÉùÃ÷
void AT24C08_WriteOneByte(u16 addr,u8 data);
u8 AT24C08_ReadOneByte(u16 addr);
void AT24C08_WriteByte(u16 addr,u16 len,u8 *buffer);
void AT24C08_PageWrite(u16 addr,u16 len,u8 *buffer);
void AT24C08_ReadByte(u16 addr,u16 len,u8 *buffer);
#endif
