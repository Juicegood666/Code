#ifndef _BEEP_H_
#define _BEEP_H_

#include "stm32f10x.h"
#include "sys.h"

#define BEEP PBout(8)
void BEEP_Init(void);

#endif



