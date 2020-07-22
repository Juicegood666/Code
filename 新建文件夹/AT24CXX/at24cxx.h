#ifndef _AT24CXX_H
#define	_AT24CXX_H
#include "iic.h"
#define Write_data 0xA0
#define Read_data 0xA1
u8 AT24CXX_Write(u8 addr,u8 data);
u8 AT24CXX_Read(u8 addr);
u8 AT24CXX_Read_More(u8 addr,u8 *buff,u8 len);
u8 AT24CXX_Write_Page(u8 addr,u8 *data,u8 len);
u8 AT24CXX_Write_More(u8 addr,u8 *buff,u8 len);
#endif

