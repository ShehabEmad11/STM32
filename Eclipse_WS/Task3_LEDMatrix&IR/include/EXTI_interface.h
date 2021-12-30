/***********************************/
/*Author: Shehab emad*/
/*Version 1.1*/
/*Data:6/5/2021*/
/**************************************/

#ifndef EXTI_INTERFACE_H
#define EXTI_INTERFACE_H

#include "STD_TYPES.h"
#include "BIT_MATH.h"


enum MEXTI_EDGES{MEXTI_RISING_EDGE,MEXTI_FALLING_EDGE,MEXTI_RISING_FALLING_EDGE};

enum MEXTI{MEXTI_0,MEXTI_1,MEXTI_2,MEXTI_3,MEXTI_4,MEXTI_5,MEXTI_6,MEXTI_7,MEXTI_8,
		   MEXTI_9,MEXTI_10,MEXTI_11,MEXTI_12,MEXTI_13,MEXTI_14,MEXTI_15};

/*We have to:
1-Enable NVIC from RCC
2-Enable AFIO from RCC
3-Set Pin Direction of corresponding EXTI
-** For alternate function inputs, the port must be configured in Input mode (floating, pull
up or pull-down) and the input pin must be driven externally.**-

4-Enable EXTI from NVIC
*/
extern void MEXTI_voidEnable(u8 copy_u8Line,u8 copy_u8Port,u8 copy_u8TriggerMode,void(*copy_PtrCallBack)(void));
extern void MEXTI_voidDisable(u8 copy_u8Line);
extern void EXTI_voidUNMaskLine(u8 copy_u8Line);
extern void EXTI_voidMaskLine(u8 copy_u8Line);
extern void EXTI_voidSelectTriggerMode(u8 copy_u8Line,u8 copy_u8Port,u8 copy_u8TriggerMode);
extern void MEXTI_voidSwTrigger(u8 copy_u8Line);
extern void MEXTI_voidSetCallBack(u8 copy_u8Line, void (*CallBack_Fptr) (void) );


#endif
