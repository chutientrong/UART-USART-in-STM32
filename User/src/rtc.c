#include "rtc.h"
#include "uart.h"
#include "delay.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_pwr.h"
#include "stm32f429i_discovery.h"


static uint64_t nowMillis = 0;


ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	
	RTC_TimeTypeInitStructure.RTC_Hours=hour;
	RTC_TimeTypeInitStructure.RTC_Minutes=min;
	RTC_TimeTypeInitStructure.RTC_Seconds=sec;
	RTC_TimeTypeInitStructure.RTC_H12=ampm;
	
	return RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);
	
}

ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
	
	RTC_DateTypeDef RTC_DateTypeInitStructure;
	RTC_DateTypeInitStructure.RTC_Date=date;
	RTC_DateTypeInitStructure.RTC_Month=month;
	RTC_DateTypeInitStructure.RTC_WeekDay=week;
	RTC_DateTypeInitStructure.RTC_Year=year;
	return RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);
}


u8 My_RTC_Init(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	u16 retry=0X1FFF; 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_BackupAccessCmd(ENABLE);	
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5050)		
	{
		RCC_LSEConfig(RCC_LSE_ON);//LSE     
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	
		{
			retry++;
			Delay_ms(10);
		}
		if(retry==0)return 1;		//LSE  
			
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		
		RCC_RTCCLKCmd(ENABLE);	 

		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//(1~0X7F)
		RTC_InitStructure.RTC_SynchPrediv  = 0xFF;//(0~7FFF)
		RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;
		RTC_Init(&RTC_InitStructure);
 
		RTC_Set_Time(0,0,0,RTC_H12_AM);	
	 
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x5050);
	} 
#if resetDateFlag
	RTC_Set_Time(0,0,0,RTC_H12_AM);
#endif
	return 0;
}

void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec)
{ 
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	RTC_AlarmTypeDef RTC_AlarmTypeInitStructure;
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;

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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//?????1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//????2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//????????
	NVIC_Init(&NVIC_InitStructure);//??
}

//??????????  
/*wksel:  @ref RTC_Wakeup_Timer_Definitions
#define RTC_WakeUpClock_RTCCLK_Div16        ((uint32_t)0x00000000)
#define RTC_WakeUpClock_RTCCLK_Div8         ((uint32_t)0x00000001)
#define RTC_WakeUpClock_RTCCLK_Div4         ((uint32_t)0x00000002)
#define RTC_WakeUpClock_RTCCLK_Div2         ((uint32_t)0x00000003)
#define RTC_WakeUpClock_CK_SPRE_16bits      ((uint32_t)0x00000004)
#define RTC_WakeUpClock_CK_SPRE_17bits      ((uint32_t)0x00000006)
*/
//cnt:??????.??0,????.
void RTC_Set_WakeUp(u32 wksel,u16 cnt)
{ 
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//????????
	NVIC_Init(&NVIC_InitStructure);//??
}

//RTC????????
void RTC_Alarm_IRQHandler(void)
{    
	if(RTC_GetFlagStatus(RTC_FLAG_ALRAF)==SET)//ALARM A???
	{
		RTC_ClearFlag(RTC_FLAG_ALRAF);//??????
	}
	EXTI_ClearITPendingBit(EXTI_Line17);	//?????17????? 											 
}

//RTC WAKE UP??????
void RTC_WKUP_IRQHandler(void)
{    
	if(RTC_GetFlagStatus(RTC_FLAG_WUTF)==SET)//WK_UP???
	{ 
		nowMillis += 125;
		RTC_ClearFlag(RTC_FLAG_WUTF);	//??????
	}   
	EXTI_ClearITPendingBit(EXTI_Line22);//?????22????? 								
}

unsigned long secs(void)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	unsigned long nowTime;
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	nowTime = RTC_TimeStruct.RTC_Hours*3600 + RTC_TimeStruct.RTC_Minutes*60 + RTC_TimeStruct.RTC_Seconds;
	return nowTime;
}

void millisInit(void)
{
	My_RTC_Init();
	RTC_Set_WakeUp(RTC_WakeUpClock_RTCCLK_Div16,255);
}

uint64_t millis(void)
{
	return nowMillis;
}
