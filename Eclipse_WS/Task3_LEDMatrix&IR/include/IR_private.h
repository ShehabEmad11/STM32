/*Author:Shehab Emad
 *Data: 12-5-2021
 *Ver:	1.1*/
 
#ifndef IR_PRIVATE_H
#define IR_PRIVATE_H


#include "STD_TYPES.h"
#include "BIT_MATH.h"


#define IR_MAXSIGNALBUFFER			(255u)
#define IR_CONTEXT_TIMEOUT			(0)
#define IR_CONTEXT_CONTPRESS		(1)

#if IR_MAXSIGNALBUFFER<=255
	typedef u8 ir_type_index;
#elif IR_MAXSIGNALBUFFER<=65535
	typedef u16 ir_type_index;
#else
	typedef u32 ir_type_index;
#endif


enum IR_FRAMESTATUS {IR_FRAMESTATUS_INVALID,IR_FRAMESTATUS_VALID,IR_FRAMESTATUS_REPEATED};

static void _voidReceptionHandler(u8 Context);
static void _voidTimeOutHandler(void);
static void _voidCopyFrame(u32* Dest,u32 *Src,u8 SrcStartIndex);
static u8 _u8CheckSubFrameInversion(ir_type_index copy_u8Start,volatile u32* copy_u32PtrFrame);
static u8 _u8CheckFrameStart(ir_type_index copy_u8Start,volatile u32* copy_u32PtrFrame);
static u8 _u8InterpetBitFrameNEC(volatile u32* copy_u32PtrBit);
static u8 _u8GetFrameData(volatile u32* copy_u32PtrBuffer,ir_type_index copy_start,u8 *copy_u8RetPtrAddress,u8 *copy_u8RetPtrData);
static u8 _u8CheckFrameRepeat(volatile u32* copy_u32PtrFrame);

static u8 _u8CheckFrameRepeat2nd(volatile u32* copy_u32PtrFrame);
static void _voidClearu32BufferRange(u32*Buffer,u16 StartIndex,u16 Length);
#endif
