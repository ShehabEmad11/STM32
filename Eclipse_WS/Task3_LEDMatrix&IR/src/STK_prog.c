/*****************************************/
/* Author  :  Ahmed Assaf & Shehab Emad  */
/* Version :  Ver1.1                     */
/* Date    : 18/5/2021             		 */
/*****************************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "STK_interface.h"
#include "STK_private.h"
#include "STK_config.h"

/*
CountFLAG bit is cleared by the following:
1-Clearing CountFLAG BIT
2-Reading Ctrl Register or Flag
3-Writing VAL Register
*/



/* Declare Callback Global Variable */
static void(*MSTK_CallBack)(void);


/* Define Global Variable for interval mode */
static u8 MSTK_u8ModeOfInterval;



void MSTK_voidInit(void)
{
#if MSTK_CLK_SRC == MSTK_SRC_AHB
    /* Write 1 on Bit2 to Set clock source as (AHB)
	 * Write 0 on Bit1 to Disable STK interrupt
	 * Write 0 to Bit0 to Disable STK */
	MSTK -> CTRL = 0x00000004;

#elif MSTK_CLK_SRC==MSTK_SRC_AHB_8
    /* Write 0 on Bit2 to Set clock source as (AHB/8)
	 * Write 0 on Bit1 to Disable STK interrupt
	 * Write 0 to Bit0 to Disable STK */
	MSTK -> CTRL = 0x00000000;
#else
	asm("nop");
#endif
}

void MSTK_voidSetBusyWait( u32 Copy_u32Ticks )
{
	if(Copy_u32Ticks>MSTK_MAX_LOADTICKS)
		return;

	/* Load ticks to load register */
	MSTK -> LOAD = Copy_u32Ticks;
	
	/* Write 1 to Bit0 to Start Counter */
	SET_BIT(MSTK->CTRL, 0);
	
	/* Wait till flag is raised */
	while( (GET_BIT(MSTK->CTRL,16)) == 0);
	
	/* Write 0 on Bit0 to Disable Counter */
	CLR_BIT(MSTK->CTRL, 0);
	MSTK -> LOAD = 0;
	MSTK -> VAL  = 0;
	
}

void MSTK_voidSetIntervalSingle  ( u32 Copy_u32Ticks, void (*Copy_ptr)(void) )
{
	if(Copy_u32Ticks>MSTK_MAX_LOADTICKS)
		return;

	/* Disable STK Interrupt */
	CLR_BIT(MSTK->CTRL, 1);

	/* Stop Timer */
	CLR_BIT(MSTK->CTRL, 0);
	
	/*Reset Current Timer Value if it already was running
	 *Writing anything to VAL resets it*/
	MSTK -> VAL = 0;

	/* Save CallBack function in Global variable */
	MSTK_CallBack = Copy_ptr;

	/* Set Mode to Single */
	MSTK_u8ModeOfInterval = MSTK_SINGLE_INTERVAL;

	/* Load ticks to load register */
	MSTK -> LOAD = Copy_u32Ticks;
	
	/* Enable STK Interrupt */
	SET_BIT(MSTK->CTRL, 1);
	
	/* Write 1 to Bit0 to Start Counter */
	SET_BIT(MSTK->CTRL, 0);
}

void MSTK_voidSetIntervalPeriodic( u32 Copy_u32Ticks, void (*Copy_ptr)(void) )
{
	if(Copy_u32Ticks>MSTK_MAX_LOADTICKS)
		return;

	/* Load ticks to load register */
	MSTK -> LOAD = Copy_u32Ticks;
	
	/* Start Timer */
	SET_BIT(MSTK->CTRL, 0);
	
	/* Save CallBack */
	MSTK_CallBack = Copy_ptr;
	
	/* Set Mode to Single */
	MSTK_u8ModeOfInterval = MSTK_PERIOD_INTERVAL;
	
	/* Enable STK Interrupt */
	SET_BIT(MSTK->CTRL, 1);
}

void MSTK_voidStopInterval(void)
{
	/* Disable STK Interrupt */
	CLR_BIT(MSTK->CTRL, 1);
	
	/* Stop Timer */
	CLR_BIT(MSTK->CTRL, 0);
	MSTK -> LOAD = 0;
	MSTK -> VAL  = 0;
}

u32 MSTK_u32GetElapsedTime(void)
{
	return ((uint32) (MSTK -> LOAD - MSTK -> VAL));
}

u32  MSTK_u32GetRemainingTime(void)
{
	u32 Local_u32RemainTime;
	
	Local_u32RemainTime = MSTK -> VAL ;
	
	return Local_u32RemainTime;
}


void SysTick_Handler(void)
{
	u8 volatile Local_u8Temporary;
	
	if (MSTK_u8ModeOfInterval == MSTK_SINGLE_INTERVAL)
	{
		/* Disable STK Interrupt */
		CLR_BIT(MSTK->CTRL, 1);
	
		/* Stop Timer */
		CLR_BIT(MSTK->CTRL, 0);

		/*Reset Load Register*/
		MSTK -> LOAD = 0;

		/*Clear Current Value Register and Reset interrupt CountFlag*/
		MSTK -> VAL  = 0;
	}
	
	/* Callback  */
	MSTK_CallBack();
	

	/* Read&Clear interrupt flag ----Already Cleared and will be implicitly cleared again after ISR */
    /*CountFLAG bit is cleared by the following:
	1-Executing ISR
	2-Clearing CountFLAG BIT
	3-Reading Ctrl Register or Flag
	4-Writing VAL Register*/
	Local_u8Temporary = GET_BIT(MSTK->CTRL,16);

	//to write compiler warning
	(u8)Local_u8Temporary;
}
