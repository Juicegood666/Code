#ifndef _DMA_H
#define _DMA_H
#include "stm32f10x.h"
void DMA_CH4_Init(u32 cpar,u32 cmar);
void DMA_CH4_Start(u32 cndtr);
void DMA_CH5_Init(u32 cpar,u32 cmar);
void DMA_CH5_Start(u32 cndtr);
#endif
