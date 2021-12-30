/***********************************/
/*Author: Shehab emad*/
/*Version 1.2*/
/*Data:6/5/2021*/

/*Added PULL UP vs Pull DOwn to PIN definition modes and to PIN Direction function*/


/**************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "GPIO_interface.h"
#include "GPIO_private.h"
#include "GPIO_config.h"


extern void MGPIO_voidSetPinDirection(volatile u8 copy_u8PORT , volatile u8 copy_u8PIN , u8 copy_u8Mode)
{
	switch(copy_u8PORT)
	{
		case GPIOA :
					if(copy_u8Mode==INPUT_PULL_UP)
					{
						SET_BIT(GPIOA_ODR,copy_u8PIN);
						copy_u8Mode=INPUT_PULL_UP_DOWN;
					}
					else if(copy_u8Mode==INPUT_PULL_DOWN)
					{
						CLR_BIT(GPIOA_ODR,copy_u8PIN);
						copy_u8Mode=INPUT_PULL_UP_DOWN;
					}

					if(copy_u8PIN <= 7)
					{
						/*Reset Mode by writing zero to the MODE and CNF bits
						without writing whole register*/
						GPIOA_CRL &= ~ ( (0b1111) << (copy_u8PIN *4) );

						/*Writing Mode by writing MODE and CNF bits
						without writing whole register*/
						GPIOA_CRL |=  ( (copy_u8Mode) << (copy_u8PIN*4) );
					}
					else if(copy_u8PIN <= 15)
					{
						copy_u8PIN -=8;
						GPIOA_CRH &= ~ ( (0b1111) << (copy_u8PIN*4) );
						GPIOA_CRH |=  ( (copy_u8Mode) << (copy_u8PIN*4) );
					}
					break;
					
		case GPIOB:
					if(copy_u8Mode==INPUT_PULL_UP)
					{
						SET_BIT(GPIOB_ODR,copy_u8PIN);
						copy_u8Mode=INPUT_PULL_UP_DOWN;
					}
					else if(copy_u8Mode==INPUT_PULL_DOWN)
					{
						CLR_BIT(GPIOB_ODR,copy_u8PIN);
						copy_u8Mode=INPUT_PULL_UP_DOWN;
					}

					if(copy_u8PIN <= 7)
					{
						GPIOB_CRL &= ~ ( (0b1111) << (copy_u8PIN*4) );		
						GPIOB_CRL |=  ( (copy_u8Mode) << (copy_u8PIN*4) );
						
						if(copy_u8Mode==INPUT_PULL_UP_DOWN)
							CLR_BIT(GPIOB_ODR,copy_u8PIN);
						else if(copy_u8Mode==INPUT_PULL_UP)
							SET_BIT(GPIOB_ODR,copy_u8PIN);

					}
					else if(copy_u8PIN <= 15)
					{
						copy_u8PIN -=8;
						GPIOB_CRH &= ~ ( (0b1111) << (copy_u8PIN*4) );
						GPIOB_CRH |=  ( (copy_u8Mode) << (copy_u8PIN*4) );
						
						if(copy_u8Mode==INPUT_PULL_UP_DOWN)
							CLR_BIT(GPIOB_ODR,copy_u8PIN+8);
						else if(copy_u8Mode==INPUT_PULL_UP)
							SET_BIT(GPIOB_ODR,copy_u8PIN+8);

					}
					break;
					
		case GPIOC:
					if(copy_u8Mode==INPUT_PULL_UP)
					{
						SET_BIT(GPIOB_ODR,copy_u8PIN);
						copy_u8Mode=INPUT_PULL_UP_DOWN;
					}
					else if(copy_u8Mode==INPUT_PULL_DOWN)
					{
						CLR_BIT(GPIOB_ODR,copy_u8PIN);
						copy_u8Mode=INPUT_PULL_UP_DOWN;
					}

					if(copy_u8PIN <= 7)
					{
						GPIOC_CRL &= ~ ( (0b1111) << (copy_u8PIN*4) );		
						GPIOC_CRL |=  ( (copy_u8Mode) << (copy_u8PIN*4) );
						
						if(copy_u8Mode==INPUT_PULL_UP_DOWN)
							CLR_BIT(GPIOC_ODR,copy_u8PIN);
						else if(copy_u8Mode==INPUT_PULL_UP)
							SET_BIT(GPIOC_ODR,copy_u8PIN);

					}
					else if(copy_u8PIN <= 15)
					{
						copy_u8PIN -=8;
						GPIOC_CRH &= ~ ( (0b1111) << (copy_u8PIN*4) );
						GPIOC_CRH |=  ( (copy_u8Mode) << (copy_u8PIN*4) );
						
						if(copy_u8Mode==INPUT_PULL_DOWN)
							CLR_BIT(GPIOC_ODR,copy_u8PIN+8);
						else if(copy_u8Mode==INPUT_PULL_UP)
							SET_BIT(GPIOC_ODR,copy_u8PIN+8);
					}
					break;
					
		default:	
					break;
	}
}


extern void MGPIO_voidSetPinValue(volatile u8 copy_u8PORT , volatile u8 copy_u8PIN ,volatile u8 copy_u8Value)
{
	switch(copy_u8PORT)
	{
		case GPIOA:
					if(copy_u8Value == GPIO_HIGH)
					{
						GPIOA_BSR=1<<copy_u8PIN;
					}
					else if(copy_u8Value == GPIO_LOW)
					{
						GPIOA_BRR=1<<copy_u8PIN;
					}
					break;
		
		case GPIOB:
					if(copy_u8Value == GPIO_HIGH)
					{
						GPIOB_BSR=1<<copy_u8PIN;
					}
					else if(copy_u8Value == GPIO_LOW)
					{
						GPIOB_BRR=1<<copy_u8PIN;
					}
					break;
		
		case GPIOC:
					if(copy_u8Value == GPIO_HIGH)
					{
						GPIOC_BSR=1<<copy_u8PIN;
					}
					else if(copy_u8Value == GPIO_LOW)
					{
						GPIOC_BRR=1<<copy_u8PIN;
					}
					break;
		
		default :
					break;
	}
}

extern void MGPIO_voidWritePortValue(volatile u8 copy_u8PORT ,volatile u16 copy_u16Value)
{
	switch(copy_u8PORT)
	{
		case GPIOA:
					GPIOA_ODR=copy_u16Value;		break;
		case GPIOB:
					GPIOB_ODR=copy_u16Value;		break;
		case GPIOC:
					GPIOC_ODR=copy_u16Value;		break;
		default :
					break;
	}
}




extern u8 MGPIO_u8GetPinValue(volatile u8 copy_u8PORT , volatile u8 copy_u8PIN)
{
	volatile u8 LOC_u8Result=0;
	switch(copy_u8PORT)
	{
	case GPIOA:
					LOC_u8Result=GET_BIT(GPIOA_IDR,copy_u8PIN);
					break;	
	
	case GPIOB:
					LOC_u8Result=GET_BIT(GPIOB_IDR,copy_u8PIN);
					break;	
	
	case GPIOC:
					LOC_u8Result=GET_BIT(GPIOC_IDR,copy_u8PIN);
					break;
	default:
					break;
	}
	return LOC_u8Result;
}


extern void MGPIO_voidTogglePin(volatile u8 copy_u8PORT , volatile u8 copy_u8PIN)
{
	volatile u8 Local_u8CurrentValue=0;
	switch(copy_u8PORT)
	{
		case GPIOA:
					Local_u8CurrentValue=GET_BIT(GPIOA_ODR,copy_u8PIN);
					if(Local_u8CurrentValue == GPIO_HIGH)
					{
						GPIOA_BSR=1<<(copy_u8PIN+16);
					}
					else
					{
						GPIOA_BSR=1<<(copy_u8PIN);
					}
					break;

		case GPIOB:
					Local_u8CurrentValue=GET_BIT(GPIOB_ODR,copy_u8PIN);
					if(Local_u8CurrentValue == GPIO_HIGH)
					{
						GPIOB_BSR=1<<(copy_u8PIN+16);
					}
					else
					{
						GPIOB_BSR=1<<(copy_u8PIN);
					}
					break;

		case GPIOC:
					Local_u8CurrentValue=GET_BIT(GPIOC_ODR,copy_u8PIN);
					if(Local_u8CurrentValue == GPIO_HIGH)
					{
						GPIOC_BSR=1<<(copy_u8PIN+16);
					}
					else
					{
						GPIOC_BSR=1<<(copy_u8PIN);
					}
					break;
		default :
					break;
	}
}




extern void MGPIO_voidLockPinConfig(volatile u8 copy_u8PORT ,u16 copy_u16SelectedPINsCode)
{
	volatile u8 Local_u8LockKeyStatus=0;
	
	switch(copy_u8PORT)
	{
		case GPIOA:	
					/*Lock Key Writing Sequence with LOCKPINS*/
					/*Enable Bit16 while keeping Pins to be locked set*/
					GPIOA_LCK=0x10000 | (u32)copy_u16SelectedPINsCode;
					/*Disable Bit16 while keeping Pins to be locked set*/
					GPIOA_LCK=0x0FFFF & (u32)copy_u16SelectedPINsCode;
					/*Enable Bit16 while keeping Pins to be locked set*/
					GPIOA_LCK=0x10000 | (u32)copy_u16SelectedPINsCode;
					/*Read BIT 16*/
					Local_u8LockKeyStatus=GPIOA_LCK>>16;
					/*Read BIT 16 it should return 1 now*/
					Local_u8LockKeyStatus=GPIOA_LCK>>16;
					
					if(Local_u8LockKeyStatus==1)
					{
						/*Nice*/
						return;
					}		
					else
					{
						/*error*/
						return;
					}
					break;

		case GPIOB:
				/*Lock Key Writing Sequence with LOCKPINS*/
				/*Enable Bit16 while keeping Pins to be locked set*/
				GPIOB_LCK=0x10000 | (u32)copy_u16SelectedPINsCode;
				/*Disable Bit16 while keeping Pins to be locked set*/
				GPIOB_LCK=0x0FFFF & (u32)copy_u16SelectedPINsCode;
				/*Enable Bit16 while keeping Pins to be locked set*/
				GPIOB_LCK=0x10000 | (u32)copy_u16SelectedPINsCode;
				/*Read BIT 16*/
				Local_u8LockKeyStatus=GPIOB_LCK>>16;
				/*Read BIT 16 it should return 1 now*/
				Local_u8LockKeyStatus=GPIOB_LCK>>16;

				if(Local_u8LockKeyStatus==1)
				{
					/*Nice*/
					return;
				}
				else
				{
					/*error*/
					return;
				}
				break;

		case GPIOC:
				/*Lock Key Writing Sequence with LOCKPINS*/
				/*Enable Bit16 while keeping Pins to be locked set*/
				GPIOC_LCK=0x10000 | (u32)copy_u16SelectedPINsCode;
				/*Disable Bit16 while keeping Pins to be locked set*/
				GPIOC_LCK=0x0FFFF & (u32)copy_u16SelectedPINsCode;
				/*Enable Bit16 while keeping Pins to be locked set*/
				GPIOC_LCK=0x10000 | (u32)copy_u16SelectedPINsCode;
				/*Read BIT 16*/
				Local_u8LockKeyStatus=GPIOC_LCK>>16;
				/*Read BIT 16 it should return 1 now*/
				Local_u8LockKeyStatus=GPIOC_LCK>>16;

				if(Local_u8LockKeyStatus==1)
				{
					/*Nice*/
					return;
				}
				else
				{
					/*error*/
					return;
				}
				break;

		default:
				/*error */ 	break;
	}
}



extern u8 MGPIO_u8GetPinConfigLockStatus(volatile u8 copy_u8PORT ,volatile u8 copy_u8PIN)
{
	u8 volatile Local_u8LockStatus;

	if(copy_u8PIN>15)
	{
		/*error*/
		return 50;
	}
		
	switch(copy_u8PORT)
	{
		case GPIOA:		
					Local_u8LockStatus=GET_BIT(GPIOA_LCK,copy_u8PIN);
					break;
		case GPIOB:
					Local_u8LockStatus=GET_BIT(GPIOB_LCK,copy_u8PIN);
					break;
		case GPIOC:
					Local_u8LockStatus=GET_BIT(GPIOC_LCK,copy_u8PIN);
					break;
		default:
					/*error */ 	break;
	}
	return Local_u8LockStatus;
}


extern u8 MGPIO_u8GetPortKeyLockStatus(volatile u8 copy_u8PORT)
{	
	u8 volatile Local_u8PortKeyLockStatus;
	switch(copy_u8PORT)
	{
		case GPIOA:		
					Local_u8PortKeyLockStatus=GET_BIT(GPIOA_LCK,16);
					break;
		case GPIOB:		
					Local_u8PortKeyLockStatus=GET_BIT(GPIOB_LCK,16);
					break;
		case GPIOC:
					Local_u8PortKeyLockStatus=GET_BIT(GPIOC_LCK,16);
					break;			
		default:
					/*Error*/ 	break;
	}
					
	return Local_u8PortKeyLockStatus;
}

