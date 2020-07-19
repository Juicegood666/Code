#ifndef _ADC_H
#define _ADC_H
#include "stm32f10x.h"
void ADCSW_Init(void);
u16 ADCSW_Get_Data(u8 data);
#endif
