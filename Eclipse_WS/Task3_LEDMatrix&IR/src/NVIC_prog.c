/***********************************/
/*Author: Shehab emad*/
/*Version 1.0*/
/*Data:19/8/2020*/
/**************************************/
#include"STD_TYPES.h"
#include"BIT_MATH.h"

#include"NVIC_interface.h"
#include"NVIC_private.h"
#include"NVIC_config.h"



extern void MNVIC_voidEnableInterrupt(u8 Copy_u8IntNumber)
{
	/*Set ext. Interrupts 0 to 59  the rest of interrupts are not in our MCU*/
	if(Copy_u8IntNumber < 60)
	{
		/*Access Array and shift bits according to Interrupt's number*/
		NVIC_ISER[(Copy_u8IntNumber/32)]=(1<<(Copy_u8IntNumber%32));
		/*We didn't use SET_BIT because writing 0 has no effect
		so we don't need to use the or operation to set the bits*/
	}
	else
	{
		/*Return Error Wrong Interrupt Number*/
	}
}

extern void MNVIC_voidDisableInterrupt(u8 Copy_u8IntNumber)
{
	/*Disable ext. Interrupts 0 to 59  the rest of interrupts are not in our MCU*/
	if(Copy_u8IntNumber < 60)
	{
		NVIC_ICER[(Copy_u8IntNumber/32)]=(1<<(Copy_u8IntNumber%32));
		/*We didn't use SET_BIT because writing 0 has no effect
		so we don't need to use the or operation to set the bits*/
	}

	else
	{
		/*Return Error*/
	}
}


extern void MNVIC_voidSetPendingFlagInterrupt(u8 Copy_u8IntNumber)
{
	/*Set Pending flag of ext. Interrupts 0 to 59  the rest of interrupts are not in our MCU*/
	if(Copy_u8IntNumber < 60)
	{
		NVIC_ISPR[(Copy_u8IntNumber/32)]=(1<<(Copy_u8IntNumber%32));
		/*We didn't use SET_BIT because writing 0 has no effect
		so we don't need to use the or operation to set the bits*/
	}

	else
	{
		/*Return Error*/
	}
}


extern void MNVIC_voidClearPendingFlagInterrupt(u8 Copy_u8IntNumber)
{
	/*Clear Pending flag of ext. Interrupts 0 to 59  the rest of interrupts are not in our MCU*/
	if(Copy_u8IntNumber < 60)
	{
		NVIC_ICPR[(Copy_u8IntNumber/32)]=(1<<(Copy_u8IntNumber%32));
		/*We didn't use SET_BIT because writing 0 has no effect
		so we don't need to use the or operation to set the bits*/
	}

	else
	{
		/*Return Error*/
	}
}

extern u8 MNVIC_u8GetActiveFlag(u8 Copy_u8IntNumber)
{
	u8 Local_u8Result=0;

	/*Set Clear flag of ext. Interrupts 0 to 59*/
	if(Copy_u8IntNumber < 60)
	{
		Local_u8Result=GET_BIT(NVIC_IABR[(Copy_u8IntNumber/32)],(Copy_u8IntNumber%32));
	}
	/*(note from ext. int. 60 to 63 is not in in our MCu)*/
	else
	{
		/*Return Error*/
	}
	return Local_u8Result;
}



/*This function is for test purposes only*/
extern void MNVIC_voidSetPeriority(s8 Copy_s8IntNumber , u8 Copy_u8GroupPriority ,u8 Copy_u8SubPriority)
{
	if(Copy_u8GroupPriority>GROUP_PRIORITY_MAX || Copy_u8SubPriority>SUB_PRIORITY_MAX)
	{
		return;
	}

	/*Input of Copy_u32Group could be:GROUP_4_SUB0,GROUP_3_SUB1,GROUP_2_SUB2,GROUP_1_SUB3,GROUP_0_SUB4
	 * 256 is equivalent to 0x100 	  0x05FA0300 ,0x05FA0400  ,0x05FA0500  ,0x05FA0600  ,0x05FA0700
	 * If GroupSubDistribution is:
	 * 1) GROUP_4_SUB0  --->		Group Range [0,15] 		Sub Range[NONE]
	 * 2) GROUP_3_SUB1  --->		Group Range [0,7] 		Sub Range[0,2]
	 * 3) GROUP_2_SUB2  --->		Group Range [0,4] 		Sub Range[0,4]
	 * 4) GROUP_1_SUB3  --->		Group Range [0,2] 		Sub Range[0,7]
	 * 1) GROUP_0_SUB4  --->		Group Range [None] 		Sub Range[0,15]*/

	u8 Local_u8Priority=Copy_u8SubPriority | (Copy_u8GroupPriority<<((MNVIC_GROUP_SUB_DISTRIBUTION - 0x05FA0300)/256) );

	/*NVIC_IPR is array bytes accessed*/
	NVIC_IPR[Copy_s8IntNumber]=(Local_u8Priority << 4) ;

}


/*This function is for test purposes only*/
extern void MNVIC_voidInit(void)
{
	/* it is not correct to call SCB in NVIC (register of another peripheral direct here*/
	#define SCB_AIRCR	*((volatile u32*) 0xE000ED0C)


	/*Define MNVIC Periorirt configuration*/
	SCB_AIRCR=MNVIC_GROUP_SUB_DISTRIBUTION;
}





#if 0

extern void MNVIC_voidEnableInterrupt(u8 Copy_u8IntNumber)
{
	/*Set ext. Interrupts 0 to 31*/
	if(Copy_u8IntNumber <= 31)
	{
		NVIC_ISER0=(1<<	Copy_u8IntNumber);
		/*We didn't use SET_BIT because writing 0 has no effect 
		so we don't need to use the or operation to set the bits*/
	}
		/*Set Interrupts 32 to 63 (note from ext. int. 60 to 63 is not in in our MCu)*/
	else if(Copy_u8IntNumber <=59)
	{
		NVIC_ISER1=(1<<	(Copy_u8IntNumber-32));
	}
	
	else
	{
		/*Return Error*/
	}
}

extern void MNVIC_voidDisableInterrupt(u8 Copy_u8IntNumber)
{
	/*Clear ext. Interrupts 0 to 31*/
	if(Copy_u8IntNumber <= 31)
	{
		NVIC_ICER0=(1<<	Copy_u8IntNumber);
		/*We didn't use SET_BIT because writing 0 has no effect 
		so we don't need to use the or operation to set the bits*/
	}
		/*Clear ext. Interrupts 32 to 63 (note from ext. int. 60 to 63 is not in in our MCu)*/
	else if(Copy_u8IntNumber <=59)
	{
		NVIC_ICER1=(1<<	(Copy_u8IntNumber-32));
	}
	
	else
	{
		/*Return Error*/
	}
}


extern void MNVIC_voidSetPendingFlagInterrupt(u8 Copy_u8IntNumber)
{
	/*Set Pending flag of ext. Interrupts 0 to 31*/
	if(Copy_u8IntNumber <= 31)
	{
		NVIC_ISPR0=(1<<	Copy_u8IntNumber);
		/*We didn't use SET_BIT because writing 0 has no effect 
		so we don't need to use the or operation to set the bits*/
	}
		/*Set Pending flag of ext. Interrupts 32 to 63 (note from ext. int. 60 to 63 is not in in our MCu)*/
	else if(Copy_u8IntNumber <=59)
	{
		NVIC_ISPR1=(1<<	(Copy_u8IntNumber-32));
	}
	
	else
	{
		/*Return Error*/
	}
}


extern void MNVIC_voidClearPendingFlagInterrupt(u8 Copy_u8IntNumber)
{
	/*Set Clear flag of ext. Interrupts 0 to 31*/
	if(Copy_u8IntNumber <= 31)
	{
		NVIC_ICPR0=(1<<	Copy_u8IntNumber);
		/*We didn't use SET_BIT because writing 0 has no effect 
		so we don't need to use the or operation to set the bits*/
	}
		/*Clear Pending flag of ext. Interrupts 32 to 63 (note from ext. int. 60 to 63 is not in in our MCu)*/
	else if(Copy_u8IntNumber <=59)
	{
		NVIC_ICPR1=(1<<	(Copy_u8IntNumber-32));
	}
	
	else
	{
		/*Return Error*/
	}
}


extern u8 MNVIC_u8GetActiveFlag(u8 Copy_u8IntNumber)
{
	u8 Local_u8Result=0;
	
	/*Set Clear flag of ext. Interrupts 0 to 31*/
	if(Copy_u8IntNumber <= 31)
	{
		Local_u8Result=GET_BIT(NVIC_IABR0,Copy_u8IntNumber);
	}
		/*Clear Pending flag of ext. Interrupts 32 to 63 (note from ext. int. 60 to 63 is not in in our MCu)*/
	else if(Copy_u8IntNumber <=59)
	{
		Local_u8Result=GET_BIT(NVIC_IABR1,Copy_u8IntNumber);
	} 
	
	else
	{
		/*Return Error*/
	}
	return Local_u8Result;
}


/*This function is for test purposes only*/
extern void MNVIC_voidSetPeriority(u8 Copy_u8PeripheralIdx, u8 Copy_u8Priority)
{
	if(Copy_u8PeripheralIdx < 60)
	{
		NVIC_IPR[Copy_u8PeripheralIdx]=Copy_u8Priority;
	}
	else
	{
		/*Error*/
	}
}

/*This function is for test purposes only*/
extern void MNVIC_voidInit(void)
{
	/* it is not correct to call SCB in NVIC (register of another peripheral direct here*/
	#define SCB_AIRCR	*((volatile u32*) 0xE000ED0C)

	/*Define MNVIC Periorirt configuration*/
	SCB_AIRCR=MNVIC_GROUP_SUB_DISTRIBUTION;
}
#endif

#if 0
extern void MNVIC_voidSetPeriority(s8 Copy_s8IntID , u8 Copy_u8GroupPriority ,u8 Copy_u8SubPriority , u8 Copy_u32Group)
{
	/*Input of Copy_u32Group could be:Group3,Group4,Group6,Group7 -->0x05FA0300,0x05FA0400,0x05FA0500,0x05FA0600,0x05FA0700*/
	u8 Local_u8Priority=Copy_u8SubPriority | (Copy_u8GroupPriority<<((Copy_u32Group- 0x05FA0300)/256) );
	/*Check intterrupt type Core(internal) or external peripheral*/
	if(Copy_s8IntID >= 0)
	{
		IPR[Copy_s8IntID]=Local_u8Priority<<4;
	}
	SCB_AIRCR=Copy_u32Group;
	else if(Copy_s8IntID < 0)
	{
		
	}
	
}
#endif
