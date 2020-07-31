#ifndef WDG_H
#define WDG_H
#include "stm32f10x.h"
#include "sys.h"
void IWDG_Init(u16 pr,u16 rlr);
void WWDG_Init(u16 psc,u16 w_arr,u16 arr);
#endif
