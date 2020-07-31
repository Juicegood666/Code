#ifndef _RTC_H_
#define _RTC_H_

#include "stm32f10x.h"
typedef struct str
{
	u32 year;
	u32 mon;
	u32 day;
	u32 hour;
	u32 min;
	u32 sec;
}RTC_TIME;

extern RTC_TIME rtc_time;

void RTC_Init(void);
u8 GET_TIME_STATE(u32 year);
void SET_RTC_TIME(u32 year ,u32 mon,u32 day,u32 hour,u32 min,u32 sec);
void GET_RTC_TIME(u32 sec);
#endif

