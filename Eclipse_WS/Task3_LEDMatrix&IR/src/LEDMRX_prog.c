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

extern void HLEDMRX_voidDisplay(u8* copy_u8PtrCharRow,u32 copy_u32DisplayTime)
{
	u8 i,j;
	u32 local_u32Counter=0;
	while(local_u32Counter<copy_u32DisplayTime)
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
			MTIMR2to5_voidSetBusyWait(2,2500);
			local_u32Counter+=2500;
		}
	}
	HLEDMRX_voidDisableAllColumn();
	HLEDMRX_voidDisableAllRow();
}


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
}
