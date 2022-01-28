/*Author:Shehab Emad
 *Data: 12-5-2021
 *Ver:	1.1*/
 
#ifndef IR_PRIVATE_H
#define IR_PRIVATE_H


#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "IR_config.h"


#define _handleIncrement(Length) 	\
{\
	/*Increment the current buffer position */  \
	/*increment the buffer with 33 signal as it was valid frame*/  \
	if( (u32)local_staticCurrentBufferIndex + (u32)Length  > (u32)globalstatic_irDataCounter) \
	{ \
		if(globalstatic_u8OverFlowFlag==TRUE )  \
		{  \
			/*Clear Overflow Flag only if the frame was VALID && Overflow is raised*/  \
			globalstatic_u8OverFlowFlag=FALSE;  \
			 \
			/*Calculate how many local_staticCurrentBufferIndex positions consumed by current frame before the end of buffer*/   \
			UsedBufferSlots=IR_MAXSIGNALBUFFER - local_staticCurrentBufferIndex ;  \
			/*Calculate the new local_staticCurrentBufferIndex after overflowing*/   \
			local_staticCurrentBufferIndex=Length - UsedBufferSlots;  \
			 \
			if(local_staticCurrentBufferIndex==globalstatic_irDataCounter)  \
			{                                             \
				/*Set Local all bufferCheckedFlag to indicate that local_staticCurrentBufferIndex matches
				 with globalstatic_irDataCounter */   \
				local_u8AllBufferCheckedFlag=1;     \
			}                                      \
			 \
		}										\
		/*Case of globalstatic_irDataCounter is about to overflow next in next EXTI write interrupt*/   \
		else if(globalstatic_u8OverFlowFlag==FALSE && globalstatic_irDataCounter==(IR_MAXSIGNALBUFFER-1ul))  \
		{   \
			local_staticCurrentBufferIndex=globalstatic_irDataCounter;		\
			/*Set Local all bufferCheckedFlag to indicate that local_staticCurrentBufferIndex matches
			 with globalstatic_irDataCounter */ \
			local_u8AllBufferCheckedFlag=1;		\
		}  \
		 \
	}  \
	else if( (u32)local_staticCurrentBufferIndex + (u32)Length  == (u32)globalstatic_irDataCounter)  \
	{  \
		local_staticCurrentBufferIndex=globalstatic_irDataCounter;  \
		/*Set Local all bufferCheckedFlag to indicate that local_staticCurrentBufferIndex matches
		 with globalstatic_irDataCounter */ \
		local_u8AllBufferCheckedFlag=1;   \
	}  \
	else  \
	{   \
		local_staticCurrentBufferIndex += Length;  \
	}\
}\




#if IR_MAXSIGNALBUFFER<=255
	typedef u8 ir_type_index;
#elif IR_MAXSIGNALBUFFER<=65535
	typedef u16 ir_type_index;
#else
	typedef u32 ir_type_index;
#endif


enum IR_FRAMESTATUS {IR_FRAMESTATUS_INVALID,IR_FRAMESTATUS_VALID,IR_FRAMESTATUS_REPEATED};

static void _voidTimeOutHandler(void);
static void _voidCopyFrame(u32* Dest,u32 *Src,ir_type_index SrcStartIndex);
static u8 _u8CheckSubFrameInversion(ir_type_index copy_u8Start,volatile u32* copy_u32PtrFrame);
static u8 _u8CheckFrameStart(ir_type_index copy_u8Start,volatile u32* copy_u32PtrFrame);
static u8 _u8InterpetBitFrameNEC(volatile u32* copy_u32PtrBit);
static u8 _u8GetFrameData(volatile u32* copy_u32PtrBuffer,ir_type_index copy_start,u8 *copy_u8RetPtrAddress,u8 *copy_u8RetPtrData);
static u8 _u8CheckFrameRepeat(volatile u32* copy_u32PtrFrame);
static u8 _u8CheckFrameRepeatEnd(volatile u32* copy_u32PtrFrame);
static u8 _u8FrameCheckNEC(ir_type_index copy_u8FrameStartIndex,volatile u32* copy_u32PtrFrameBuffer);
static void _voidClearu32BufferRange(u32*Buffer,ir_type_index StartIndex,ir_type_index Length);
static void _voidSetIsStart(void);
static void _voidSetIsRepeat(void);
#if 0
static u8 _u8WriteRawSignalBuffer(u32* SrcArr);
#endif
#endif
