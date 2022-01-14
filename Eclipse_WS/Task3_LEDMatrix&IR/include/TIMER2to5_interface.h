/***********************************/
/*Author: Shehab emad*/
/*Version 2.0*/
/*Data:12/1/2022*/
/**************************************/
#ifndef TIMER2TO5_INTERFACE_H
#define TIMER2TO5_INTERFACE_H

#include "STD_TYPES.h"
#include "BIT_MATH.h"



extern void MTIMR2to5_voidInit(u8 copy_u8TimerNumber, u16 copy_u16Prescaler);
extern void MTIMR2to5_voidSetBusyWait(u8 copy_u8TimerNumber,u16 copy_u16Delay);
extern void MTIMR2to5_voidSetTimerSingle(u8 copy_u8TimerNumber,u16 copy_u16Interval,void (*copy_Ptr2CallBack)(void));

extern void MTIMR2to5_voidSetTimerPeriodic(u8 copy_u8TimerNumber,u16 copy_u16PeriodMs,void (*copy_Ptr2CallBack)(void));
extern uint8 MTIMR2to5_u8GetElapsedTime(uint8 copy_u8TimerNumber, uint16* copy_ptru16ElapsedTime);
extern void MTIM2_voidCountAndFireTIM2Alarms(void);
extern void MTIMR2to5_voidStopInterval(uint8 copy_u8TimerNumber);


extern void MTIMR2to5_voidSetAlarm_Ms(u8 copy_u8TimerNumber,u8 copy_u8ContextNumber,u32 copy_u32msVal);
extern u8 TIMx_u8IsAlarmFired(u8 copy_u8TimerNumber,u8 copy_u8ContextNumber,u8* copy_Ptr2u8AlarmState);
extern u8 TIMx_u8ClrAlarmFired(u8 copy_u8TimerNumber,u8 copy_u8ContextNumber);

enum {MTIMER_2=0,MTIMER_3,MTIMER_4,MTIMER_5};
enum {MTIMx_CONTEXT0,MTIMx_CONTEXT1,MTIMx_CONTEXT2,MTIMx_CONTEXT3,MTIMx_CONTEXT4};

#endif
