#ifndef _ADC_H_
#define _ADC_H_

#include "stm32f10x.h"
void ADC1_Init(void);   //����
u16 ADC1_CHx(u8 ch);    //����
void ADC1_Init_1(void); //ע��
u16 ADC1_CHx_1(u8 ch);  //ע��
#endif
