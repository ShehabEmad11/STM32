/***********************************/
/*Author: Shehab emad*/
/*Version 1.3*/
/*Data:19/7/2021*/
/**************************************/
 
#ifndef LEDMRX_INTERFACE_H
#define	LEDMRX_INTERFACE_H


#include "STD_TYPES.h"
#include "BIT_MATH.h"

typedef enum
{
	HLEDMRX_STATUS_FREE,HLEDMRX_STATUS_BUSY
}HLEDMRX_enuStatus;


extern void HLEDMRX_voidInit(void);
extern void HLEDMRX_voidEnableRow(u8 copy_u8RowNumber);
extern void HLEDMRX_voidDisableRow(u8 copy_u8RowNumber);
extern void HLEDMRX_voidEnableColumn(u8 copy_u8ColumnNumber);
extern void HLEDMRX_voidDisableColumn(u8 copy_u8ColumnNumber);
extern void HLEDMRX_voidDisableAllRow(void);
extern void HLEDMRX_voidDisableAllColumn(void);
extern void HLEDMRX_voidDisplay(u8* copy_u8PtrCharRow,u32 copy_u32DisplayTime);
extern void HLEDMRX_voidDisplayShifting(u8* copy_u8PtrCharRow,u32 copy_u32ShiftTime);

extern void HLEDMRX_voidDisplayAsync(u8* copy_u8PtrCharRow,u32 copy_u32DisplayTime);
extern HLEDMRX_enuStatus HLEDMRX_GetDisplayStatus(void);


#endif
