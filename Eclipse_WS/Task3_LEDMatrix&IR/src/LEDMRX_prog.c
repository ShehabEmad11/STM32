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

//TODO: to be reset after finishing
static uint8 * globalstatic_ptru8Char=NULL;//u32 copy_u32DisplayTime
static uint32 globalstatic_u32DisplayTimeMs=0;

extern HLEDMRX_enuStatus_t HLEDMRX_GetDisplayStatus(void)
{
	return(LEDMRX_enuStatus);
}

extern void HLEDMRX_voidMainFunction()
{
	/*This Function must be called cyclic every < LEDMRXDELAYms value
	 *This function is the main LEDMRX function and used to avoid Blockage(using of setBusyWait) in Displaying*/
	typedef enum {STATUS_NODELAY,STATUS_DELAY}enuDisplayDelay_t;
	#define LEDMRX_CONTEXT		(MTIMx_CONTEXT0)
	//TODO: converted to 2500 Ms after editing SetAlarmfunction to be in Ms
	#define LEDMRXDELAYms		(3)
	static enuDisplayDelay_t localstatic_enuDisplayDelayStatus=STATUS_NODELAY;
	static uint8 columnIter;//,breakflag;
	static uint8* localstatic_Ptru8CurrChar=NULL;
	uint8 rowIter,local_u8IsCurrDelayAlarmFired=FALSE;

	if( (NULL == globalstatic_ptru8Char) || (globalstatic_u32DisplayTimeMs == 0ul) )
	{
		return;
	}

	if(LEDMRX_enuStatus==HLEDMRX_STATUS_FREE)
	{
		/*For First Time only*/
		if(NULL==localstatic_Ptru8CurrChar)
			localstatic_Ptru8CurrChar=globalstatic_ptru8Char;

		columnIter=0;
		LEDMRX_enuStatus=HLEDMRX_STATUS_BUSY;
	}

	/*In case of LEDMRX input changed during runtime we will display last received*/
	if(localstatic_Ptru8CurrChar != globalstatic_ptru8Char)
	{
		columnIter=0;
		/*Reset Delay Status to avoic checking on delay for this function call*/
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
		columnIter++;
		if(columnIter >= HLEDMRX_COLUMNDIMENSION)
			columnIter=0;
		//TODO: remove 1000 after editing SetAlarmfunction to be in Ms
		if(globalstatic_u32DisplayTimeMs > LEDMRXDELAYms*1000 )
			globalstatic_u32DisplayTimeMs -= LEDMRXDELAYms*1000;
		else
			globalstatic_u32DisplayTimeMs=0;
	}

	while(globalstatic_u32DisplayTimeMs>0)
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
			/*Set Alarm from timer2 context 0 for 3ms)*/
			//TODO: LEDMRXDELAYms to be 2500Ms not 3ms  after editing SetAlarmfunction to be in Ms
			MTIMR2to5_voidSetAlarm_ms(MTIMER_2 , LEDMRX_CONTEXT, LEDMRXDELAYms);

			localstatic_enuDisplayDelayStatus=STATUS_DELAY;
			return; //return from function to be entered again in its periodic cycle
		}//Columns' loop
	}//Displaytime's loop

	/*If we reached here this means that display time finished and character displayed successfully*/
	HLEDMRX_voidDisableAllColumn();
	HLEDMRX_voidDisableAllRow();
	globalstatic_u32DisplayTimeMs = 0ul;
	globalstatic_ptru8Char=NULL;
	localstatic_Ptru8CurrChar=NULL;
	LEDMRX_enuStatus=HLEDMRX_STATUS_FREE;
}


extern void HLEDMRX_voidDisplayAsync(u8* copy_u8PtrCharRow,u32 copy_u32DisplayTime)
{
	/*This function is used to Request Display of a LEDMRX character
	 * it must be used in conjunction with HLEDMRX_voidMainFunction*/

	/*if no going display operations,calculate number of times to display any char on LEDMRX (note that 20,000Ms (20ms) is the minimum Display time*/
	if(copy_u32DisplayTime < HLEDMRX_MIN_DISPLAY_TIME)
		return;
#if 0
	if( (NULL != globalstatic_ptru8Char) || (globalstatic_u32DisplayTimeMs != 0ul) )
	{
		return;
	}
#endif

	globalstatic_u32DisplayTimeMs = copy_u32DisplayTime;
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

extern void HLEDMRX_voidDisplayShifting(u8* copy_u8PtrCharRow,u32 copy_u32DisplayTime)
{
	u8 i,j,local_u8ShiftCounter;
	u32 local_u32PerShiftTime,local_u32temp=0;

    local_u32PerShiftTime = copy_u32DisplayTime / (MIN_NUMOFSHIFTS);

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


