/***********************************/
/*Author: Shehab emad*/
/*Version 1.0*/
/*Data:19/8/2020*/
/**************************************/
#ifndef NVIC_INTERFACE_H
#define NVIC_INTERFACE_H


/*Note That NVIC is Core peripheral*/

#include "STD_TYPES.h"
#include "BIT_MATH.h"



/*Define the value to be passed to SCB_AIRCR to write it and choose the group/sub group combination*/
/*4 bits for group and 0 sub-group*/
#define MNVIC_GROUP_4_SUB0 				GROUP4
/*3 bits for group and 1 sub-group*/
#define MNVIC_GROUP_3_SUB1 				GROUP3
/*2 bits for group and 2 sub-group*/
#define MNVIC_GROUP_2_SUB2 				GROUP2
/*1 bits for group and 3 sub-group*/
#define MNVIC_GROUP_1_SUB3 				GROUP1
/*0 bits for group and 4 sub-group*/
#define MNVIC_GROUP_0_SUB4 				GROUP0


/*Input 0-59*/
extern void MNVIC_voidEnableInterrupt(u8 Copy_u8IntNumber);
extern void MNVIC_voidDisableInterrupt(u8 Copy_u8IntNumber);
extern void MNVIC_voidSetPendingFlagInterrupt(u8 Copy_u8IntNumber);
extern void MNVIC_voidClearPendingFlagInterrupt(u8 Copy_u8IntNumber);
extern u8 MNVIC_u8GetActiveFlag(u8 Copy_u8IntNumber);
//extern void MNVIC_voidSetPeriority(s8 Copy_s8IntID , u8 Copy_u8GroupPriority ,u8 Copy_u8SubPriority , u8 Copy_u32Group);

/*This function is for test purposes only*/
extern void MNVIC_voidSetPeriority(s8 Copy_s8IntNumber , u8 Copy_u8GroupPriority ,u8 Copy_u8SubPriority);
extern void MNVIC_voidInit(void);



#if 0
/*Define the value to be passed to SCB_AIRCR to write it and choose the group/sub group combination*/
/*4 bits for group and 0 sub-group*/
#define GROUP3		0x05FA0300
/*3 bits for group and 1 sub-group*/
#define GROUP4		0x05FA0400
/*2 bits for group and 2 sub-group*/
#define GROUP5		0x05FA0500
/*1 bits for group and 3 sub-group*/
#define GROUP6		0x05FA0600
/*0 bits for group and 4 sub-group*/
#define GROUP7		0x05FA0700


extern void MNVIC_voidDisableInterrupt(u8 Copy_u8IntNumber);
extern void MNVIC_voidEnableInterrupt(u8 Copy_u8IntNumber);
extern void MNVIC_voidSetPendingFlagInterrupt(u8 Copy_u8IntNumber);
extern void MNVIC_voidClearPendingFlagInterrupt(u8 Copy_u8IntNumber);
extern u8 MNVIC_u8GetActiveFlag(u8 Copy_u8IntNumber);
//extern void MNVIC_voidSetPeriority(s8 Copy_s8IntID , u8 Copy_u8GroupPriority ,u8 Copy_u8SubPriority , u8 Copy_u32Group);

/*This function is for test purposes only*/
extern void MNVIC_voidSetPeriority(u8 Copy_u8PeripheralIdx, u8 Copy_u8Priority);
extern void MNVIC_voidInit(void);

#endif


#endif
