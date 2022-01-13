/***********************************/
/*Author: Shehab emad*/
/*Version 1.4*/
/*Data:19/7/2021*/

/*Based on Timer2 instead on MSTK*/
/*Disabled all rows and columns after exit*/
/**************************************/

#include"STD_TYPES.h"
#include"BIT_MATH.h"

#include "GPIO_interface.h"
#include "TIMER2to5_interface.h"
#include "LEDMRX_interface.h"
#include "LEDMRX_config.h"
#include "LEDMRX_private.h"


static const u8 Row_u8ArrayPORT[HLEDMRX_ROWDIMENSION]={ROWPORTS};
static const u8 Row_u8ArrayPIN[HLEDMRX_ROWDIMENSION]={ROWPINS};

static const u8 Column_u8ArrayPORT[HLEDMRX_COLUMNDIMENSION]={COLUMNPORTS};
static const u8 Column_u8ArrayPIN[HLEDMRX_COLUMNDIMENSION]={COLUMNPINS};


extern void HLEDMRX_voidInit(void)
{
	u8 i;
	
	for(i=0;i<HLEDMRX_ROWDIMENSION;i++)
	{
		MGPIO_voidSetPinDirection(Row_u8ArrayPORT[i],Row_u8ArrayPIN[i],HLEDMRX_MAX_FREQUENCY);
	}
	for(i=0;i<HLEDMRX_COLUMNDIMENSION;i++)
	{
		MGPIO_voidSetPinDirection(Column_u8ArrayPORT[i],Column_u8ArrayPIN[i],HLEDMRX_MAX_FREQUENCY);
	}
	
	MTIMR2to5_voidInit(MTIMER_2, 7);
	
	HLEDMRX_voidDisableAllColumn();
	HLEDMRX_voidDisableAllRow();


	#if 0
	MGPIO_voidSetPinDirection(LEDMRX_ANODE0,HLEDMRX_MAX_FREQUENCY);
	MGPIO_voidSetPinDirection(LEDMRX_ANODE1,HLEDMRX_MAX_FREQUENCY);
	MGPIO_voidSetPinDirection(LEDMRX_ANODE2,HLEDMRX_MAX_FREQUENCY);
	MGPIO_voidSetPinDirection(LEDMRX_ANODE3,HLEDMRX_MAX_FREQUENCY);
	MGPIO_voidSetPinDirection(LEDMRX_ANODE4,HLEDMRX_MAX_FREQUENCY);
	MGPIO_voidSetPinDirection(LEDMRX_ANODE5,HLEDMRX_MAX_FREQUENCY);
	MGPIO_voidSetPinDirection(LEDMRX_ANODE6,HLEDMRX_MAX_FREQUENCY);
	MGPIO_voidSetPinDirection(LEDMRX_ANODE7,HLEDMRX_MAX_FREQUENCY);

	MGPIO_voidSetPinDirection(LEDMRX_CATHODE0,HLEDMRX_MAX_FREQUENCY);
	MGPIO_voidSetPinDirection(LEDMRX_CATHODE1,HLEDMRX_MAX_FREQUENCY);
	MGPIO_voidSetPinDirection(LEDMRX_CATHODE2,HLEDMRX_MAX_FREQUENCY);
	MGPIO_voidSetPinDirection(LEDMRX_CATHODE3,HLEDMRX_MAX_FREQUENCY);
	MGPIO_voidSetPinDirection(LEDMRX_CATHODE4,HLEDMRX_MAX_FREQUENCY);
	MGPIO_voidSetPinDirection(LEDMRX_CATHODE5,HLEDMRX_MAX_FREQUENCY);
	MGPIO_voidSetPinDirection(LEDMRX_CATHODE6,HLEDMRX_MAX_FREQUENCY);
	MGPIO_voidSetPinDirection(LEDMRX_CATHODE7,HLEDMRX_MAX_FREQUENCY);
#endif
}

extern void HLEDMRX_voidEnableRow(u8 copy_u8RowNumber)
{
	if(copy_u8RowNumber>=HLEDMRX_ROWDIMENSION)
			return;

	if(HLEDMRX_ROWPOLARITY==ANODE)
	{
		MGPIO_voidSetPinValue(Row_u8ArrayPORT[copy_u8RowNumber],Row_u8ArrayPIN[copy_u8RowNumber],GPIO_HIGH);
	}
	else if(HLEDMRX_ROWPOLARITY==CATHODE)
	{
		MGPIO_voidSetPinValue(Row_u8ArrayPORT[copy_u8RowNumber],Row_u8ArrayPIN[copy_u8RowNumber],GPIO_LOW);
	}
	else
	{
		/*Error*/
	}
}
extern void HLEDMRX_voidDisableRow(u8 copy_u8RowNumber)
{
	if(copy_u8RowNumber>=HLEDMRX_ROWDIMENSION)
			return;

	if(HLEDMRX_ROWPOLARITY==ANODE)
	{
		MGPIO_voidSetPinValue(Row_u8ArrayPORT[copy_u8RowNumber],Row_u8ArrayPIN[copy_u8RowNumber],GPIO_LOW);
	}
	else if(HLEDMRX_ROWPOLARITY==CATHODE)
	{
		MGPIO_voidSetPinValue(Row_u8ArrayPORT[copy_u8RowNumber],Row_u8ArrayPIN[copy_u8RowNumber],GPIO_HIGH);
	}
	else
	{
		/*Error*/
	}
}

extern void HLEDMRX_voidEnableColumn(u8 copy_u8ColumnNumber)
{
	if(copy_u8ColumnNumber>=HLEDMRX_COLUMNDIMENSION)
		return;

	if(HLEDMRX_COLUMNPOLARITY==ANODE)
	{
		MGPIO_voidSetPinValue(Column_u8ArrayPORT[copy_u8ColumnNumber],Column_u8ArrayPIN[copy_u8ColumnNumber],GPIO_HIGH);
	}
	else if(HLEDMRX_COLUMNPOLARITY==CATHODE)
	{
		MGPIO_voidSetPinValue(Column_u8ArrayPORT[copy_u8ColumnNumber],Column_u8ArrayPIN[copy_u8ColumnNumber],GPIO_LOW);
	}
	else
	{
		/*Error*/
	}
}
extern void HLEDMRX_voidDisableColumn(u8 copy_u8ColumnNumber)
{
	if(copy_u8ColumnNumber>=HLEDMRX_COLUMNDIMENSION)
			return;

	if(HLEDMRX_COLUMNPOLARITY==ANODE)
	{
		MGPIO_voidSetPinValue(Column_u8ArrayPORT[copy_u8ColumnNumber],Column_u8ArrayPIN[copy_u8ColumnNumber],GPIO_LOW);
	}
	else if(HLEDMRX_COLUMNPOLARITY==CATHODE)
	{
		MGPIO_voidSetPinValue(Column_u8ArrayPORT[copy_u8ColumnNumber],Column_u8ArrayPIN[copy_u8ColumnNumber],GPIO_HIGH);
	}
	else
	{
		/*Error*/
	}
}

extern void HLEDMRX_voidDisableAllRow(void)
{
	u8 i;
	if(HLEDMRX_ROWPOLARITY==ANODE)
	{		
		for(i=0;i<HLEDMRX_ROWDIMENSION;i++)
		{
			MGPIO_voidSetPinValue(Row_u8ArrayPORT[i],Row_u8ArrayPIN[i],GPIO_LOW);
		}
	}
	else if(HLEDMRX_ROWPOLARITY==CATHODE)
	{
		for(i=0;i<HLEDMRX_COLUMNDIMENSION;i++)
		{
			MGPIO_voidSetPinValue(Row_u8ArrayPORT[i],Row_u8ArrayPIN[i],GPIO_HIGH);
		}
	}
	else
	{
		/*Error*/
	}
}

extern void HLEDMRX_voidDisableAllColumn(void)
{
	u8 i;
	if(HLEDMRX_COLUMNPOLARITY==ANODE)
	{		
		for(i=0;i<HLEDMRX_ROWDIMENSION;i++)
		{
			MGPIO_voidSetPinValue(Column_u8ArrayPORT[i],Column_u8ArrayPIN[i],GPIO_LOW);
		}
	}
	else if(HLEDMRX_COLUMNPOLARITY==CATHODE)
	{
		for(i=0;i<HLEDMRX_COLUMNDIMENSION;i++)
		{
			MGPIO_voidSetPinValue(Column_u8ArrayPORT[i],Column_u8ArrayPIN[i],GPIO_HIGH);
		}
	}
	else
	{
		/*Error*/
	}
}



static HLEDMRX_enuStatus_t LEDMRX_enuStatus=HLEDMRX_STATUS_FREE;
static HLEDMRX_enuDispMode_t LEDMRX_enuCurrentDispMode=MODE_STATIC;
static uint8 * globalstatic_ptru8Char=NULL;
static uint32 globalstatic_u32PerShiftTime=0;
static uint8 globalsatic_u8IsStopRequested=FALSE;

extern HLEDMRX_enuStatus_t HLEDMRX_GetDisplayStatus(void)
{
	return(LEDMRX_enuStatus);
}

extern void HLEDMRX_voidRequestStop(void)
{
	globalsatic_u8IsStopRequested=TRUE;
}

extern void HLEDMRX_voidMainFunction()
{
	/*This Function must be called cyclic every < LEDMRXDELAYms value
	 *This function is the main LEDMRX function and used to avoid Blockage(using of setBusyWait) in Displaying*/

	//TODO: handle display mode switch in runtime
	//TODO: move this #define
	#define LEDMRX_CONTEXT		(MTIMx_CONTEXT0)

	static enuDisplayDelay_t localstatic_enuDisplayDelayStatus=STATUS_NODELAY;
	static uint8 columnIter,localstatic_u8ShiftCounter;
	static uint8* localstatic_Ptru8CurrChar=NULL;
	static uint32 localstatic_u32CurrPerShiftTime;
	uint8 rowIter,local_u8IsCurrDelayAlarmFired=FALSE;

	if( (NULL == globalstatic_ptru8Char) )
	{
		return;
	}

	if(globalsatic_u8IsStopRequested==TRUE)
	{
		HLEDMRX_voidDisableAllColumn();
		HLEDMRX_voidDisableAllRow();
		globalstatic_u32PerShiftTime=0ul;
		localstatic_u32CurrPerShiftTime=0;
		globalstatic_ptru8Char=NULL;
		localstatic_Ptru8CurrChar=NULL;
		localstatic_u8ShiftCounter=0;
		LEDMRX_enuStatus=HLEDMRX_STATUS_FREE;

		globalsatic_u8IsStopRequested=FALSE;
	}


	if (LEDMRX_enuCurrentDispMode==MODE_SHIFTING  && globalstatic_u32PerShiftTime == 0ul)
	{
		return;
	}
	if(LEDMRX_enuStatus==HLEDMRX_STATUS_FREE)
	{
		/*For First Time only*/
		if(NULL==localstatic_Ptru8CurrChar)
			localstatic_Ptru8CurrChar=globalstatic_ptru8Char;

		columnIter=0;
		localstatic_u8ShiftCounter=0;
		localstatic_u32CurrPerShiftTime=globalstatic_u32PerShiftTime;
		LEDMRX_enuStatus=HLEDMRX_STATUS_BUSY;
	}

	/*In case of LEDMRX input changed during runtime we will display last received*/
	if(localstatic_Ptru8CurrChar != globalstatic_ptru8Char)
	{
		columnIter=0;
		/*Reset Delay Status to avoid checking on delay for this function call*/
		localstatic_enuDisplayDelayStatus=STATUS_NODELAY;
		/*Clear Alarm*/
		TIMx_u8ClrAlarmFired(MTIMER_2 , LEDMRX_CONTEXT);
		/*Re-Save Current Character*/
		localstatic_Ptru8CurrChar=globalstatic_ptru8Char;
	}

	if(localstatic_enuDisplayDelayStatus==STATUS_DELAY)
	{
		/*Check if Alarms Fired*/
		if(E_NOT_OK==TIMx_u8IsAlarmFired(MTIMER_2 , LEDMRX_CONTEXT,&local_u8IsCurrDelayAlarmFired))
			return;

		if(local_u8IsCurrDelayAlarmFired==FALSE)
			return;

		/*if we are here then alarm is fired*/
		/*Clear Alarm*/
		TIMx_u8ClrAlarmFired(MTIMER_2 , LEDMRX_CONTEXT);
		local_u8IsCurrDelayAlarmFired=FALSE;
		localstatic_enuDisplayDelayStatus=STATUS_NODELAY;



		//SHIFTMODESPECIFIC
		if(localstatic_u32CurrPerShiftTime > LEDMRXDELAYMs)
			localstatic_u32CurrPerShiftTime -= LEDMRXDELAYMs;
		else
			localstatic_u32CurrPerShiftTime=0;

		columnIter++;
		if(columnIter >= HLEDMRX_COLUMNDIMENSION)
		{
			//SHIFTMODESPECIFIC
			if(localstatic_u32CurrPerShiftTime==0)
			{
				localstatic_u8ShiftCounter++;
				localstatic_u32CurrPerShiftTime=globalstatic_u32PerShiftTime;
			}
			columnIter=0;
		}
		//SHIFTMODESPECIFIC
		if(localstatic_u8ShiftCounter >= HLEDMRX_SHIFTSNUMBER)
			localstatic_u8ShiftCounter=0;
	}

	if(LEDMRX_enuCurrentDispMode==MODE_STATIC)
	{
		while(columnIter<HLEDMRX_COLUMNDIMENSION)
		{
			HLEDMRX_voidDisableAllColumn();
			HLEDMRX_voidDisableAllRow();
			HLEDMRX_voidEnableColumn(columnIter);
			for(rowIter=0; rowIter<HLEDMRX_ROWDIMENSION; rowIter++)
			{
				if(GET_BIT(*(globalstatic_ptru8Char+columnIter),rowIter)==GPIO_HIGH)
					HLEDMRX_voidEnableRow(rowIter);
				else if(GET_BIT(*(globalstatic_ptru8Char+columnIter),rowIter)==GPIO_LOW)
					HLEDMRX_voidDisableRow(rowIter);
				else
					asm("nop");/*Error*/
			}
			/* minimum Time for human eye and brain to realize is 2.5ms*/
			/*Set Alarm from timer2 context0 for 2500Ms)*/
			MTIMR2to5_voidSetAlarm_Ms(MTIMER_2 , LEDMRX_CONTEXT, LEDMRXDELAYMs);

			localstatic_enuDisplayDelayStatus=STATUS_DELAY;
			return; //return from function to be entered again in its periodic cycle
		}//Columns' loop
	}
	else if (LEDMRX_enuCurrentDispMode==MODE_SHIFTING)
	{
		while(localstatic_u8ShiftCounter<HLEDMRX_SHIFTSNUMBER)
		{
			while(columnIter<HLEDMRX_COLUMNDIMENSION)
			{
				HLEDMRX_voidDisableAllColumn();
				HLEDMRX_voidDisableAllRow();
				HLEDMRX_voidEnableColumn(columnIter+localstatic_u8ShiftCounter);
				for(rowIter=0; rowIter<HLEDMRX_ROWDIMENSION; rowIter++)
				{
					if(GET_BIT(*(globalstatic_ptru8Char+columnIter),rowIter)==GPIO_HIGH)
						HLEDMRX_voidEnableRow(rowIter);
					else if(GET_BIT(*(globalstatic_ptru8Char+columnIter),rowIter)==GPIO_LOW)
						HLEDMRX_voidDisableRow(rowIter);
					else
						asm("nop");/*Error*/
				}
				/* minimum Time for human eye and brain to realize is 2.5ms*/
				/*Set Alarm from timer2 context0 for 2500Ms)*/
				MTIMR2to5_voidSetAlarm_Ms(MTIMER_2 , LEDMRX_CONTEXT, LEDMRXDELAYMs);

				localstatic_enuDisplayDelayStatus=STATUS_DELAY;
				return; //return from function to be entered again in its periodic cycle
			}
		}
	}

#if 0
	/*If we reached here, thats mean */
	HLEDMRX_voidDisableAllColumn();
	HLEDMRX_voidDisableAllRow();
	globalstatic_u32PerShiftTime=0ul;
	localstatic_u32CurrPerShiftTime=0;
	globalstatic_ptru8Char=NULL;
	localstatic_Ptru8CurrChar=NULL;
	localstatic_u8ShiftCounter=0;
	LEDMRX_enuStatus=HLEDMRX_STATUS_FREE;
#endif
}


extern void HLEDMRX_voidDisplayAsync(u8* copy_u8PtrCharRow)
{
	/*This function is used to Request Display of a LEDMRX character
	 * it must be used in conjunction with HLEDMRX_voidMainFunction*/


	/*Update Current DisplayMode (this is used later by LEDMRX_main)*/
	LEDMRX_enuCurrentDispMode=MODE_STATIC;

	/*Update GlobalStatic variables with current Character to display and time of display*/
	globalstatic_ptru8Char = copy_u8PtrCharRow;
}

extern void HLEDMRX_voidDisplay(u8* copy_u8PtrCharRow,u32 copy_u32DisplayTime)
{
	u8 i,j;
	uint32 local_u32NumOfDisplayTimes;

	/*calculate number of times to display any char on LEDMRX (note that 20,000Ms (20ms) is the minimum Display time*/
	local_u32NumOfDisplayTimes = copy_u32DisplayTime / HLEDMRX_MIN_DISPLAY_TIME;

	/*Reform Display time to be multiples of Minimum Time Per Word*/
	//copy_u32DisplayTime = copy_u32DisplayTime - (copy_u32DisplayTime % HLEDMRX_MIN_DISPLAY_TIME);

	while(local_u32NumOfDisplayTimes>0)
	{
		for(i=0;i<HLEDMRX_COLUMNDIMENSION;i++)
		{
			HLEDMRX_voidDisableAllColumn();
			HLEDMRX_voidDisableAllRow();
			HLEDMRX_voidEnableColumn(i);
			for(j=0;j<HLEDMRX_ROWDIMENSION;j++)
			{
				if(GET_BIT(*(copy_u8PtrCharRow+i),j)==GPIO_HIGH)
					HLEDMRX_voidEnableRow(j);
				else if(GET_BIT(*(copy_u8PtrCharRow+i),j)==GPIO_LOW)
					HLEDMRX_voidDisableRow(j);
				else
				{
					/*Error*/
				}
			}
			/*Max timer input is 65536 (u16 register)
			 * Min Time for human eye and brain to realize is 2.5ms*/
			MTIMR2to5_voidSetBusyWait(2,2500);
//			local_u32Counter+=2500;
		}
		local_u32NumOfDisplayTimes--;
	}
	HLEDMRX_voidDisableAllColumn();
	HLEDMRX_voidDisableAllRow();
}

extern void HLEDMRX_voidDisplayShiftingAsync(u8* copy_u8PtrCharRow,u32 copy_u32PerShiftTime)
{
	/*This function is used to Request Display of a LEDMRX character
	 * it must be used in conjunction with HLEDMRX_voidMainFunction*/

	if(copy_u32PerShiftTime< HLEDMRX_MIN_DISPLAY_TIME)
		return;

	/*Update Current DisplayMode (this is used later by LEDMRX_main)*/
	LEDMRX_enuCurrentDispMode=MODE_SHIFTING;

	/*Update GlobalStatic variables with current Character to display and time of display*/
	globalstatic_ptru8Char = copy_u8PtrCharRow;
	globalstatic_u32PerShiftTime = copy_u32PerShiftTime;
}



extern void HLEDMRX_voidDisplayShifting(u8* copy_u8PtrCharRow,u32 copy_u32DisplayTime)
{
	u8 i,j,local_u8ShiftCounter;
	u32 local_u32PerShiftTime,local_u32temp=0;

    local_u32PerShiftTime = copy_u32DisplayTime / HLEDMRX_SHIFTSNUMBER;

    if(local_u32PerShiftTime<HLEDMRX_MIN_DISPLAY_TIME)
    {
    	return;
    }

	for(local_u8ShiftCounter=0; local_u8ShiftCounter<HLEDMRX_COLUMNDIMENSION; local_u8ShiftCounter++)
	{
		while(local_u32temp<local_u32PerShiftTime)
		{
			for(i=0;i<HLEDMRX_COLUMNDIMENSION;i++)
			{
				HLEDMRX_voidDisableAllColumn();
				HLEDMRX_voidDisableAllRow();
				HLEDMRX_voidEnableColumn(i+local_u8ShiftCounter);
				for(j=0;j<HLEDMRX_ROWDIMENSION;j++)
				{
					if(GET_BIT(*(copy_u8PtrCharRow+i),j)==GPIO_HIGH)
							HLEDMRX_voidEnableRow(j);
					else if(GET_BIT(*(copy_u8PtrCharRow+i),j)==GPIO_LOW)
						HLEDMRX_voidDisableRow(j);
					else
						/*Error*/
						asm("nop");
				}
				MTIMR2to5_voidSetBusyWait(2,2500);
			}
			local_u32temp += 20000;
		}
		local_u32temp=0;
	}

	HLEDMRX_voidDisableAllColumn();
	HLEDMRX_voidDisableAllRow();
}


#if 0
extern void HLEDMRX_voidDisplayShifting(u8* copy_u8PtrCharRow,u32 copy_u32ShiftTime)
{
	u8 i,j,local_u8ShiftCounter;
	local_u8ShiftCounter=0;
	u32 local_u32ShiftTime;
	local_u32ShiftTime=0;
	while(local_u8ShiftCounter<8)
	{
		for(i=0;i<HLEDMRX_COLUMNDIMENSION;i++)
		{
			HLEDMRX_voidDisableAllColumn();
			HLEDMRX_voidDisableAllRow();
			HLEDMRX_voidEnableColumn(i+local_u8ShiftCounter);
			for(j=0;j<HLEDMRX_ROWDIMENSION;j++)
			{
				if(GET_BIT(*(copy_u8PtrCharRow+i),j)==GPIO_HIGH)
						HLEDMRX_voidEnableRow(j);
				else if(GET_BIT(*(copy_u8PtrCharRow+i),j)==GPIO_LOW)
					HLEDMRX_voidDisableRow(j);
				else
					/*Error*/
					asm("nop");
			}
			MTIMR2to5_voidSetBusyWait(2,2500);
			local_u32ShiftTime+=2500;
		}
		if(local_u32ShiftTime<copy_u32ShiftTime)
			continue;

		local_u32ShiftTime=0;
		local_u8ShiftCounter++;
	}

	HLEDMRX_voidDisableAllColumn();
	HLEDMRX_voidDisableAllRow();
}
#endif


