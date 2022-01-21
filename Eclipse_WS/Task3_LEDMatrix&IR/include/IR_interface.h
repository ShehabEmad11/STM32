/*Author:Shehab Emad
 *Data: 12-5-2021
 *Ver:	1.1 */
 
#ifndef IR_INTERFACE_H
#define	IR_INTERFACE_H

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "IR_database.h"


/*Not needed here external functions shouldn't use ir_type_index direct*/
/*
#define IR_MAXSIGNALBUFFER		(255u)

#if IR_MAXSIGNALBUFFER <= 255
	typedef u8 ir_type_index;
#elif IR_MAXSIGNALBUFFER<=65535
	typedef u16 ir_type_index;
#else
	typedef u32 ir_type_index;
#endif
*/

typedef enum
{
	IR_DATA_NO_VALID_DATA_EMPTYBUF,    				 //0
	IR_DATA_NO_VALID_DATA_PARTIALBUF,    			 //1
	IR_DATA_EXTRACTED_EMPTYBUF,         //2
	IR_DATA_EXTRACTED_PARTIALBUF,		//3
	IR_DATA_REPEATEXTRACTED_EMPTYBUF,	//4
	IR_DATA_REPEATEXTRACTED_PARTIALBUF,	//5
	IR_DATA_OVERFLOWBUF=77u,            //77
	IR_LOGICERROR=88,					//88
	IR_DATA_NON_RECEIVED=99u,			//99
	IR_IMPOSSIBLETRET=199u				//199
}IR_enuExtractResult_t;


extern void HIR_voidEnable(u8 copy_u8ExtiLine,u8 copy_u8ExtiPort);
extern void HIR_voidReceiveFrameNEC(void);

extern IR_enuExtractResult_t HIR_u8ExtractDataFromBuffer(u8 *copy_u8RetPtrAddress,u8 *copy_u8RetPtrData);

/*Interfaces to Read/Reset Signal buffer which stores time of each signal*/
extern u8 HIR_u8ReadRawSignalBuffer(u32* DestArr);
extern void HIR_voidResetRawSignalBuffer(void);
extern u8 HIR_voidSetUsedBufferAddress(volatile u32* copy_PtrToBuffer,u16 BuffSize);

/*Interfaces to check/clear frame's start or repeated frame is received*/
extern u8 HIR_u8GetIsStart(void);
extern u8 HIR_u8GetIsRepeat(void);
extern void HIR_voidClrIsStart(void);
extern void HIR_voidClrIsRepeat(void);

#endif
