/***********************************/
/*Author: Shehab emad*/
/*Version 1.1*/
/*Data:6/5/2021*/
/**************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "AFIO_interface.h"
#include "NVIC_interface.h"
#include "EXTI_interface.h"
#include "EXTI_config.h"
#include "EXTI_private.h"

#define NULL		(void * )0

//static void (* EXTI0_CallBack) (void) = NULL ;


static void (*MEXTIArrayPtrCallBacks[16]) (void)={NULL};

/*To generate the interrupt, the interrupt line should be configured and enabled. This is done
 by programming the two trigger registers with the desired edge detection and by enabling
 the interrupt request by writing a ‘1’ to the corresponding bit in the interrupt mask register.
 When the selected edge occurs on the external interrupt line, an interrupt request is
 generated. The pending bit corresponding to the interrupt line is also set. This request is
 reset by writing a ‘1’ in the pending register.
 
 To configure the 20 lines as interrupt sources, use the following procedure:
• Configure the mask bits of the 20 Interrupt lines (EXTI_IMR) 
• Configure the Trigger Selection bits of the Interrupt lines (EXTI_RTSR and EXTI_FTSR)
• Configure the enable and mask bits that control the NVIC IRQ channel mapped to the
  External Interrupt Controller (EXTI) so that an interrupt coming from one of the 20 lines
  can be correctly acknowledged.
  
-20 EXTI lines but the lines 16 to 19 are used by USB,RTC,PVD and Ethernet(only on connectivity lines devices)
*/
 
/* This Function Enable EXTIx=copy_u8Line on PortY=copy_u8Port
 while PIN is already same as EXTI's Number by default*/
extern void MEXTI_voidEnable(u8 copy_u8Line,u8 copy_u8Port,u8 copy_u8TriggerMode,void(*copy_PtrCallBack)(void))
{
	/*Chose Exti0 interrupt position as default*/
	u8 local_u8NVIC_EXTIPos=6;

	/*We Start Counting from EXTI0 so we subtract one from maximum configured*/
	if(copy_u8Line>(MAXEXTINUMBERS-1))
		/*error wrong EXTI number*/
		return;

	/*Make sure PORTC is only selected for LINES 13,14,15 "PORTC have only PINS 13,14,15 on STM32" */
	if(  (copy_u8Port==GPIOC) && (copy_u8Line < 13) )
	{
		/*Error*/
		return;
	}	
	
	/*Make Sure the port is one of the available ports on Kit and assign the port to its corresponding Binary value
	  It could be that GPIOA defined in other library as 8 for example not 0 so we redefine the port to be 
	  compatible */
	if(copy_u8Port==GPIOA)
		copy_u8Port=0b0000;
	else if(copy_u8Port==GPIOB)
		copy_u8Port=0b0001;
	else if(copy_u8Port==GPIOC)
		copy_u8Port=0b0010;
	else/*error*/
		return;

	
	/*We have to Enable RCC of NVIC first and initialize EXTILineNumber
    this should be Happen In main but i will only initialize EXTI Line here*/
	if(copy_u8Line<=4)
		local_u8NVIC_EXTIPos=copy_u8Line+6;
	else if(copy_u8Line<=9)		//between [5,9]
		local_u8NVIC_EXTIPos=23;	
	else if(copy_u8Line<=15)		//between [10,15]
		local_u8NVIC_EXTIPos=40;
	else
		/*Error*/
		return;
	
	/*Return if the Edge MOde is not correctly specified*/
	if(copy_u8TriggerMode!=MEXTI_RISING_EDGE  &&
	   copy_u8TriggerMode!=MEXTI_FALLING_EDGE &&
	   copy_u8TriggerMode!=MEXTI_RISING_FALLING_EDGE)
		return;

	/*Select the Trigger mode*/
	EXTI_voidSelectTriggerMode(copy_u8Line,copy_u8Port,copy_u8TriggerMode);

	/*We have to Enable AFIO_clock from RCC*/
	MRCC_voidEnableClock(MRCC_APB2,0);

	/*Select which EXTI Pin you will use from AFIO Control register */
	MAFIO_voidSetEXTIConfig(copy_u8Line,copy_u8Port);
	//AFIO->EXTICR[copy_u8Line/4]= copy_u8Port<<((copy_u8Line%4)*4);
	
	MEXTI_voidSetCallBack(copy_u8Line, copy_PtrCallBack);

	/*Un mask register from Interrupt Mask Register IMR*/
	EXTI_voidUNMaskLine(copy_u8Line);
		
	/*Enable Corresponding EXTI from NVIC*/	
    MNVIC_voidEnableInterrupt(local_u8NVIC_EXTIPos);	
}

extern void MEXTI_voidDisable(u8 copy_u8Line)
{
	/*We Start Counting from EXTI0 so we subtract one from maximum configured*/
	if(copy_u8Line>(MAXEXTINUMBERS-1))
		/*error wrong EXTI number*/
		return;		

	/*mask register from Interrupt Mask Register IMR*/
	EXTI_voidMaskLine(copy_u8Line);
		
	/*Disable Corresponding EXTI from NVIC*/	
	/*we shouldn't disable interrupt from NVIC as some EXTI share the same
	 * NVIC position */
    //MNVIC_voidDisableInterrupt(local_u8NVIC_EXTIPos);
}



extern void EXTI_voidUNMaskLine(u8 copy_u8Line)
{
	/*UNmask register from Interrupt Mask Register IMR*/
	SET_BIT(EXTI->IMR,copy_u8Line);
}

extern void EXTI_voidMaskLine(u8 copy_u8Line)
{
	/*mask register from Interrupt Mask Register IMR*/
	CLR_BIT(EXTI->IMR,copy_u8Line);
}

extern void EXTI_voidSelectTriggerMode(u8 copy_u8Line,u8 copy_u8Port,u8 copy_u8TriggerMode)
{
	/*Select Trigger Mode*/
	switch(copy_u8TriggerMode)
	{
		case MEXTI_RISING_EDGE :
			/*We Have to define the PIN direction in main but we will initialize it here too as input floating*/
			MGPIO_voidSetPinDirection(copy_u8Port,copy_u8Line,INPUT_PULL_DOWN);
			SET_BIT(EXTI->RTSR,copy_u8Line);
			CLR_BIT(EXTI->FTSR,copy_u8Line);
			break;
		
		case MEXTI_FALLING_EDGE :
			MGPIO_voidSetPinDirection(copy_u8Port,copy_u8Line,INPUT_PULL_UP);
			SET_BIT(EXTI->FTSR,copy_u8Line);
			CLR_BIT(EXTI->RTSR,copy_u8Line);
			break;
			
		case MEXTI_RISING_FALLING_EDGE :
			MGPIO_voidSetPinDirection(copy_u8Port,copy_u8Line,INPUT_FLOATING);
			SET_BIT(EXTI->FTSR,copy_u8Line);
			SET_BIT(EXTI->RTSR,copy_u8Line);
			break;
		
		default:
			return;		/*Wrong EDGE selection*/
	}
}

static u8 _u8GetPendingFlag(u8 copy_u8Line)
{
	return (GET_BIT(EXTI->PR,copy_u8Line));
}

static void _voidClearPendingFlag(u8 copy_u8Line)
{
	EXTI->PR=1<<copy_u8Line;
}

/*For Software Interrupt*/
extern void MEXTI_voidSwTrigger(u8 copy_u8Line)
{
	/*Enable Software Interrupt Interrupt*/
	SET_BIT(EXTI->SWIER , copy_u8Line);
}


extern void MEXTI_voidSetCallBack(u8 copy_u8Line, void (*copy_PtrVoidCallBack) (void) )
{
	if(copy_u8Line>15)
		return;
	
	MEXTIArrayPtrCallBacks[copy_u8Line]=copy_PtrVoidCallBack;

}

void EXTI0_IRQHandler(void)
{
	/*Call the Call back for EXTI0 */
	MEXTIArrayPtrCallBacks[MEXTI_0]();

	/*	Clear EXTI0 pending Bit */
	_voidClearPendingFlag(MEXTI_0);
}

void EXTI1_IRQHandler(void)
{
	/*	Clear EXTI1 pending Bit */
	_voidClearPendingFlag(MEXTI_1);
	/*Call the Call back for EXTI1 */
	MEXTIArrayPtrCallBacks[MEXTI_1]();
}
void EXTI2_IRQHandler(void)
{
	/*	Clear EXTI2 pending Bit */
	_voidClearPendingFlag(MEXTI_2);
	/*Call the Call back for EXTI0 */
	MEXTIArrayPtrCallBacks[MEXTI_2]();
}
void EXTI3_IRQHandler(void)
{
	/*	Clear EXTI3 pending Bit */
	_voidClearPendingFlag(MEXTI_3);
	/*Call the Call back for EXTI3 */
	MEXTIArrayPtrCallBacks[MEXTI_3]();
}
void EXTI4_IRQHandler(void)
{
	/*	Clear EXTI4 pending Bit */
	_voidClearPendingFlag(MEXTI_4);
	/*Call the Call back for EXTI4 */
	MEXTIArrayPtrCallBacks[MEXTI_4]();
}

void EXTI9_5_IRQHandler(void)
{
	u8 EXTI_i;
	for(EXTI_i=MEXTI_5; EXTI_i<=MEXTI_9; EXTI_i++)
	{
		if(_u8GetPendingFlag(EXTI_i))
		{
			/*	Clear EXTI_i pending Bit */
			_voidClearPendingFlag(EXTI_i);
			/*Call the Call back for EXTI_i */
			MEXTIArrayPtrCallBacks[EXTI_i]();
		}
	}
}

void EXTI15_10_IRQHandler(void)
{
	u8 EXTI_i;
	for(EXTI_i=MEXTI_10; EXTI_i<=MEXTI_15; EXTI_i++)
	{
		if(_u8GetPendingFlag(EXTI_i))
		{
			/*	Clear MEXTI_i pending Bit */
			_voidClearPendingFlag(EXTI_i);
			/*Call the Call back for MEXTI_i */
			MEXTIArrayPtrCallBacks[EXTI_i]();
		}
	}
}
