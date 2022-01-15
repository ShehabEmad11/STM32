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



typedef enum
{
	NVICPOS_WWDG=0,
	NVICPOS_PVD=1,
	NVICPOS_TAMPER=2,
	NVICPOS_RTC=3,
	NVICPOS_FLASH=4,
	NVICPOS_RCC=5,
	NVICPOS_EXTI0=6,
	NVICPOS_EXTI1=7,
	NVICPOS_EXTI2=8,
	NVICPOS_EXTI3=9,
	NVICPOS_EXTI4=10,
	NVICPOS_DMA1_Channel1=11,
	NVICPOS_DMA1_Channel2=12,
	NVICPOS_DMA1_Channel3=13,
	NVICPOS_DMA1_Channel4=14,
	NVICPOS_DMA1_Channel5=15,
	NVICPOS_DMA1_Channel6=16,
	NVICPOS_DMA1_Channel7=17,
	NVICPOS_ADC1_2=18,
	NVICPOS_CAN1_TX=19,
	NVICPOS_CAN1_RX0=20,
	NVICPOS_CAN1_RX1=21,
	NVICPOS_CAN1_SCE=22,
	NVICPOS_EXTI9_5=23,
	NVICPOS_TIM1_BRK=24,
	NVICPOS_TIM1_UP=25,
	NVICPOS_TIM1_TRG_COM=26,
	NVICPOS_TIM1_CC=27,
	NVICPOS_TIM2=28,
	NVICPOS_TIM3=29,
	NVICPOS_TIM4=30,
	NVICPOS_I2C1_EV=31,
	NVICPOS_I2C1_ER=32,
	NVICPOS_I2C2_EV=33,
	NVICPOS_I2C2_ER=34,
	NVICPOS_SPI1=35,
	NVICPOS_SPI2=36,
	NVICPOS_USART1=37,
	NVICPOS_USART2=38,
	NVICPOS_USART3=39,
	NVICPOS_EXTI15_10=40,
	NVICPOS_RTCAlarm=41,
	NVICPOS_OTG_FS_WKUP=42,

	NVICPOS_TIM5=50,
	NVICPOS_SPI3=51,
	NVICPOS_UART4=52,
	NVICPOS_UART5=53,
	NVICPOS_TIM6=54,
	NVICPOS_TIM7=55,
	NVICPOS_DMA2_Channel1=56,
	NVICPOS_DMA2_Channel2=57,
	NVICPOS_DMA2_Channel3=58,
	NVICPOS_DMA2_Channel4=59,
	NVICPOS_DMA2_Channel5=60,
	NVICPOS_ETH=61,
	NVICPOS_ETH_WKUP=62,
	NVICPOS_CAN2_TX=63,
	NVICPOS_CAN2_RX0=64,
	NVICPOS_CAN2_RX1=65,
	NVICPOS_CAN2_SCE=66,
	NVICPOS_OTG_FS=67

}NVIC_VectorTable;


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
