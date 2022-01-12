/*Author:Shehab Emad
 *Data: 12-5-2021
 *Ver:	1.1 */
 
#ifndef IR_INTERFACE_H
#define	IR_INTERFACE_H

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "IR_database.h"

#define IR_MAXSIGNALBUFFER		(255u)

#if IR_MAXSIGNALBUFFER <= 255
	typedef u8 ir_type_index;
#elif IR_MAXSIGNALBUFFER<=65535
	typedef u16 ir_type_index;
#else
	typedef u32 ir_type_index;
#endif


enum IR_DATAEXTRACTION_RESULT
{
	IR_DATA_NO_VALID_DATA,    			 //0
	IR_DATA_EXTRACTED_EMPTYBUF,         //1
	IR_DATA_EXTRACTED_PARTIALBUF,		//2
	IR_DATA_REPEATEXTRACTED_EMPTYBUF,	//3
	IR_DATA_REPEATEXTRACTED_PARTIALBUF,	//4
	IR_DATA_OVERFLOWBUF=77u,            //77
	IR_LOGICERROR=88,					//88
	IR_DATA_NON_RECEIVED=99u,			//99
	IR_IMPOSSIBLETRET=199u				//199
};


extern void HIR_voidEnable(u8 copy_u8ExtiLine,u8 copy_u8ExtiPort);
extern void HIR_voidReceiveFrameNEC(void);
extern u8 HIR_u8FrameCheckNEC(ir_type_index copy_u8FrameStartIndex,volatile u32* copy_u32PtrFrameBuffer);
extern u8 HIR_u8ExtractDataFromBuffer(u8 *copy_u8RetPtrAddress,u8 *copy_u8RetPtrData);

/*Interfaces to Read/Reset Signal buffer which stores time of each signal*/
extern u8 HIR_u8ReadRawSignalBuffer(u32* DestArr);
extern void HIR_voidResetRawSignalBuffer(void);

/*Interfaces to check/clear frame's start or repeated frame is received*/
extern void HIR_voidClrIsStart(void);
extern void HIR_voidClrIsRepeat(void);
extern u8 HIR_u8GetIsStart(void);
extern u8 HIR_u8GetIsRepeat(void);

#endif
