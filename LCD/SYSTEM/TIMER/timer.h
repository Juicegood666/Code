#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32f10x.h"
extern u32 key_time;
void TIM4_Init(u16 psc,u16 arr);
void TIM4_PWM_Init(u16 psc,u16 arr,u16 cmp);
void TIM1_Init(u16 psc,u16 arr);
void TIM2_Init(u16 psc,u16 arr);
void TIM3_Init(u16 psc,u16 arr);
void TIM5_PWM_Input_Init(u16 psc,u16 arr);
#endif
