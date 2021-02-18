#include "rtc2.h"
#include "uart.h"
#include "delay.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_pwr.h"
#include "stm32f429i_discovery.h"
#include "misc.h"

ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	
	RTC_TimeTypeInitStructure.RTC_Hours=hour;    //??
	RTC_TimeTypeInitStructure.RTC_Minutes=min;   //??
	RTC_TimeTypeInitStructure.RTC_Seconds=sec;   //?
	RTC_TimeTypeInitStructure.RTC_H12=ampm;      //????
	
	return RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);
	
}

ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
	
	RTC_DateTypeDef RTC_DateTypeInitStructure;
	
	RTC_DateTypeInitStructure.RTC_Date=date;         //?
	RTC_DateTypeInitStructure.RTC_Month=month;       //?
	RTC_DateTypeInitStructure.RTC_WeekDay=week;      //??
	RTC_DateTypeInitStructure.RTC_Year=year;         //?
	return RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);
}

u8 My_RTC_Init(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	u16 retry=0X1FFF; 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//??PWR??
	PWR_BackupAccessCmd(ENABLE);	//????????? 
 
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5050)		//????????
	{
		RCC_LSEConfig(RCC_LSE_ON);//LSE ??    
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//?????RCC???????,????????
		{
		retry++;
		Delay_ms(10);
		}
		if(retry==0)return 1;		//LSE ????. 
 
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//??RTC??(RTCCLK),??LSE??RTC??    
		RCC_RTCCLKCmd(ENABLE);	//??RTC?? 
 
		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC??????(1~0X7F)
		RTC_InitStructure.RTC_SynchPrediv  = 0xFF;//RTC??????(0~7FFF)
		RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//RTC???,24????
		RTC_Init(&RTC_InitStructure);
 
		RTC_Set_Time(20,59,56,RTC_H12_AM);	//????
		RTC_Set_Date(15,8,13,1);		//????
 
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x5050);	//?????????
	} 
	return 0;
}

void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec)
{ 
	EXTI_InitTypeDef   EXTI_InitStructure;
	RTC_AlarmTypeDef RTC_AlarmTypeInitStructure;
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);//????A 
 
	RTC_TimeTypeInitStructure.RTC_Hours=hour;//??
	RTC_TimeTypeInitStructure.RTC_Minutes=min;//??
	RTC_TimeTypeInitStructure.RTC_Seconds=sec;//?
	RTC_TimeTypeInitStructure.RTC_H12=RTC_H12_AM;
 
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDay=week;//??
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_WeekDay;//????
	RTC_AlarmTypeInitStructure.RTC_AlarmMask=RTC_AlarmMask_None;//??????,???
	RTC_AlarmTypeInitStructure.RTC_AlarmTime=RTC_TimeTypeInitStructure;
	RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmTypeInitStructure);
 
 
	RTC_ClearITPendingBit(RTC_IT_ALRA);//??RTC??A???
	EXTI_ClearITPendingBit(EXTI_Line17);//??LINE17??????? 
 
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);//????A??
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);//????A 
 
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;//LINE17
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//????
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //????? 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//??LINE17
	EXTI_Init(&EXTI_InitStructure);//??
 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//?????1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//????2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//????????
	NVIC_Init(&NVIC_InitStructure);//??
}

void RTC_Set_WakeUp(u32 wksel,u16 cnt)
{ 
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	RTC_WakeUpCmd(DISABLE);//??WAKE UP
 
	RTC_WakeUpClockConfig(wksel);//??????
 
	RTC_SetWakeUpCounter(cnt);//??WAKE UP????????
 
	RTC_ClearITPendingBit(RTC_IT_WUT); //??RTC WAKE UP???
	EXTI_ClearITPendingBit(EXTI_Line22);//??LINE22??????? 
	 
	RTC_ITConfig(RTC_IT_WUT,ENABLE);//??WAKE UP ?????
	RTC_WakeUpCmd( ENABLE);//??WAKE UP ??? 
 
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;//LINE22
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//????
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //????? 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//??LINE22
	EXTI_Init(&EXTI_InitStructure);//??
 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//?????1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//????2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//????????
	NVIC_Init(&NVIC_InitStructure);//??
}

void RTC_Alarm_IRQHandler(void)
{    
	if(RTC_GetFlagStatus(RTC_FLAG_ALRAF)==SET)//ALARM A??
	{
		RTC_ClearFlag(RTC_FLAG_ALRAF);//??????

	}   
	EXTI_ClearITPendingBit(EXTI_Line17);	//?????17????? 											 
}
 
//RTC WAKE UP??????
void RTC_WKUP_IRQHandler(void)
{    
	if(RTC_GetFlagStatus(RTC_FLAG_WUTF)==SET)//WK_UP??
	{ 
		RTC_ClearFlag(RTC_FLAG_WUTF);	//??????
//		LED1=!LED1; 
	}   
	EXTI_ClearITPendingBit(EXTI_Line22);//?????22????? 								
}


