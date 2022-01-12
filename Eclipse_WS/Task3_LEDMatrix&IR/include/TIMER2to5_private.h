/***********************************/
/*Author: Shehab emad*/
/*Version 1.0*/
/*Data:19/7/2021*/
/**************************************/
#ifndef TIMER2TO5_PRIVATE_H
#define TIMER2TO5_PRIVATE_H


#include "STD_TYPES.h"
#include "BIT_MATH.h"



typedef struct 
{
	volatile u32 CR1;
	volatile u32 CR2;
	volatile u32 SMCR;
	volatile u32 DIER;
	volatile u32 SR;
	volatile u32 EGR;
	volatile u32 CCMR1;
	volatile u32 CCMR2;
	volatile u32 CCER;
	volatile u32 CNT;
	volatile u32 PSC;
	volatile u32 ARR;
	volatile u32 Reserved;
	volatile u32 CCR1;
	volatile u32 CCR2;
	volatile u32 CCR3;
	volatile u32 CCR4;
	volatile u32 Reserved2;
	volatile u32 DCR;
	volatile u32 DMAR;
}TIMx_t;  //create type

#define TIMx_SINGLE_INTERVAL	(1)
#define TIMx_PERIODIC_INTERVAL	(2)


/*Casting address to a pointer to structure*/
#define MTIM2 ((volatile TIMx_t *)(0x40000000))
#define MTIM3 ((volatile TIMx_t *)(0x40000400))
#define MTIM4 ((volatile TIMx_t *)(0x40000800))
#define MTIM5 ((volatile TIMx_t *)(0x40000C00))

typedef volatile TIMx_t *sptr;
volatile TIMx_t (*TIMx[4])={(sptr)0x40000000,(sptr)0x40000400,(sptr)0x40000800,(sptr)0x40000C00};


#endif
