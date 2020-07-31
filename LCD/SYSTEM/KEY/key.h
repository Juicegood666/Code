#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f10x.h"
#include "sys.h"

#define KEY_UP PAin(0)
#define KEY0 PEin(4)
#define KEY1 PEin(3)
#define KEY2 PEin(2)

void KEY_Init(void);
u8 KEY_Scanf(void);
#endif
