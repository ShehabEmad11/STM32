/***********************************/
/*Author: SHehab emad*/
/*Version 1.0*/
/*Data:8/8/2020*/
/**************************************/
#ifndef RCC_INTERFACE_H
#define RCC_INTERFACE_H

#define		 MRCC_AHB 	0
#define 	 MRCC_APB1 	1
#define		 MRCC_APB2 	2


void MRCC_voidEnableClock(u8 Copy_u8BusId, u8 Copy_u8PerId);
void MRCC_voidDisableClock(u8 Copy_u8BusId, u8 Copy_u8PerId);
void MRCC_voidInitSysClock(void);


#endif
