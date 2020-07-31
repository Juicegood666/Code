#ifndef _ADC_H_
#define _ADC_H_

#include "stm32f10x.h"
void ADC1_Init(void);   //规则
u16 ADC1_CHx(u8 ch);    //规则
void ADC1_Init_1(void); //注入
u16 ADC1_CHx_1(u8 ch);  //注入
#endif
