/***********************************/
/*Author: Shehab emad*/
/*Version 1.1*/
/*Data:6/5/2021*/
/**************************************/

#ifndef EXTI_PRIVATE_H
#define EXTI_PRIVATE_H

#include "STD_TYPES.h"
#include "BIT_MATH.h"

/*
  0x4001 0400 - 0x4001 07FF			EXTI
*/


typedef struct
{
	volatile u32 IMR;		/*Interrupt Mask Register*/
	volatile u32 EMR;		/*Event Mask Register*/
	volatile u32 RTSR; 		/*Rising trigger selection register*/
	volatile u32 FTSR;		/*Falling trigger selection register*/
	volatile u32 SWIER;		/*Software interrupt event register*/
	volatile u32 PR;		/*Pending register*/

}EXTI_t;

#define EXTI ((volatile EXTI_t *) (0x40010400))

static u8 _u8GetPendingFlag(u8 copy_u8Line);
static void _voidClearPendingFlag(u8 copy_u8Line);

#endif
