#ifndef __RTC_H
#define __RTC_H	 
#include "stm32f4xx.h"

#define resetDateFlag 1

u8 My_RTC_Init(void);						//RTC???
void millisInit(void);
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);			//RTC????
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week); 		//RTC????
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec);		//??????(?????,24???)
void RTC_Set_WakeUp(u32 wksel,u16 cnt);					//??????????

unsigned long secs(void);
uint64_t millis(void);

#endif
