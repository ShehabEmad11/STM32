/***********************************/
/*Author: Shehab emad*/
/*Version 1.0*/
/*Data:19/7/2021*/
/**************************************/
#ifndef TIMER2TO5_INTERFACE_H
#define TIMER2TO5_INTERFACE_H

#include "STD_TYPES.h"
#include "BIT_MATH.h"

extern void MTIMR2to5_voidInit(u8 copy_u8TimerNumber, u16 copy_u16Prescaler);
extern void MTIMR2to5_voidSetBusyWait(u8 copy_u8TimerNumber,u16 copy_u16Delay);
extern void MTIMR2to5_voidSetTimerSingle(u8 copy_u8TimerNumber,u16 copy_u16Delay);

enum {MTIMER_2=2,MTIMER_3,MTIMER_4,MTIMER_5};

#endif
