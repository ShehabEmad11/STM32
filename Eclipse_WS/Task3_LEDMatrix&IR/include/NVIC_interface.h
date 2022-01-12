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
	WWDG_NVICPOS=0,
	PVD_NVICPOS=1,
	TAMPER_NVICPOS=2,
	RTC_NVICPOS=3,
	FLASH_NVICPOS=4,
	RCC_NVICPOS=5,
	EXTI0_NVICPOS=6,
	EXTI1_NVICPOS=7,
	EXTI2_NVICPOS=8,
	EXTI3_NVICPOS=9,
	EXTI4_NVICPOS=10,
	DMA1_Channel1_NVICPOS=11,
	DMA1_Channel2_NVICPOS=12,
	DMA1_Channel3_NVICPOS=13,
	DMA1_Channel4_NVICPOS=14,
	DMA1_Channel5_NVICPOS=15,
	DMA1_Channel6_NVICPOS=16,
	DMA1_Channel7_NVICPOS=17,
	ADC1_2_NVICPOS=18,
	CAN1_TX_NVICPOS=19,
	CAN1_RX0_NVICPOS=20,
	CAN1_RX1_NVICPOS=21,
	CAN1_SCE_NVICPOS=22,
	EXTI9_5_NVICPOS=23,
	TIM1_BRK_NVICPOS=24,
	TIM1_UP_NVICPOS=25,
	TIM1_TRG_COM_NVICPOS=26,
	TIM1_CC_NVICPOS=27,
	TIM2_NVICPOS=28,
	TIM3_NVICPOS=29,
	TIM4_NVICPOS=30,
	I2C1_EV_NVICPOS=31,
	I2C1_ER_NVICPOS=32,
	I2C2_EV_NVICPOS=33,
	I2C2_ER_NVICPOS=34,
	SPI1_NVICPOS=35,
	SPI2_NVICPOS=36,
	USART1_NVICPOS=37,
	USART2_NVICPOS=38,
	USART3_NVICPOS=39,
	EXTI15_10_NVICPOS=40,
	RTCAlarm_NVICPOS=41,
	OTG_FS_WKUP_NVICPOS=42,

	TIM5_NVICPOS=50,
	SPI3_NVICPOS=51,
	UART4_NVICPOS=52,
	UART5_NVICPOS=53,
	TIM6_NVICPOS=54,
	TIM7_NVICPOS=55,
	DMA2_Channel1_NVICPOS=56,
	DMA2_Channel2_NVICPOS=57,
	DMA2_Channel3_NVICPOS=58,
	DMA2_Channel4_NVICPOS=59,
	DMA2_Channel5_NVICPOS=60,
	ETH_NVICPOS=61,
	ETH_WKUP_NVICPOS=62,
	CAN2_TX_NVICPOS=63,
	CAN2_RX0_NVICPOS=64,
	CAN2_RX1_NVICPOS=65,
	CAN2_SCE_NVICPOS=66,
	OTG_FS_NVICPOS=67
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
