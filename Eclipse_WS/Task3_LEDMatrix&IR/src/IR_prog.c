/*Author:Shehab Emad
 *Data: 12-5-2021
 *Ver:	1.1*/

#include"string.h"
#include"STD_TYPES.h"
#include"BIT_MATH.h"

#include "STK_interface.h"
#include "EXTI_interface.h"

#include "IR_interface.h"
#include "IR_private.h"
#include "IR_config.h"

	/*the array containing signal times which each represent one bit*/

/*
  NEC Frame is as following
  A 9ms leading pulse burst							LOW   (9000Ms)
  A 4.5ms space										HIGH  (4500Ms)	
  The 8-bit address for the receiving device		8*LOW(562.5Ms) + 8*HIGH(562.5Ms or 1678.5Ms)
  The 8-bit logical inverse of the address			8*LOW(562.5Ms) + 8*HIGH(562.5Ms or 1678.5Ms)
  The 8-bit command									8*LOW(562.5Ms) + 8*HIGH(562.5Ms or 1678.5Ms)
  The 8-bit logical inverse of the command			8*LOW(562.5Ms) + 8*HIGH(562.5Ms or 1678.5Ms)
  pulse burst to signify the end of message			LOW

  if EXTI interrupts are received on LOW edge -NEC line default is high- then we will get 1+8*4+1=34 interrupt
*/

static volatile u32 globalstatic_u32IRArrSignalTime[IR_MAXSIGNALBUFFER];
static volatile u8 globalstatic_u8IRIsStart,globalstatic_u8IRIsRepeat,globalstatic_u8IRInterruptVirginityFlag,globalstatic_u8OverFlowFlag;
/*Initialize the ptr by default to globalstatic_u32IRArrSignalTime unless otherwise setted by external function*/
static volatile u32* globalstatic_ptru32Buffer=globalstatic_u32IRArrSignalTime;
static volatile ir_type_index globalstatic_irDataCounter;


extern void HIR_voidEnable(u8 copy_u8ExtiLine,u8 copy_u8ExtiPort)
{
	/*The IR peripheral generates noise while not used so it might trigger interrupt on its own*/

	ir_type_index i;

	/*Initialize Variables*/
	globalstatic_irDataCounter=0;
	globalstatic_u8IRIsStart=FALSE;
	globalstatic_u8IRIsRepeat=FALSE;
	globalstatic_u8IRInterruptVirginityFlag=TRUE;
	globalstatic_u8OverFlowFlag=FALSE;

	/*Initialize Signal Buffer*/
	for(i=0;i<IR_MAXSIGNALBUFFER;i++)
	{	
		globalstatic_ptru32Buffer[i]=0;
	}
	
	//MSTK module should be initialized
	//MSTK_voidInit();

	MEXTI_voidEnable(copy_u8ExtiLine,copy_u8ExtiPort,MEXTI_FALLING_EDGE,HIR_voidReceiveFrameNEC);
}	

/*This Function Receive Frames of NEC in global 32-bit array using EXTI,MSTK*/
extern void HIR_voidReceiveFrameNEC(void)
{
	/*This Function Receive Frames of NEC in global 32-bit array using EXTI,MSTK*/

	static u8 local_staticSetOverflowNext=FALSE;


	/*if not first time to fire interrupt*/
	if(globalstatic_u8IRInterruptVirginityFlag!=TRUE)
	{
		if( local_staticSetOverflowNext == TRUE)
		{
			/*Flag overflow (it will be cleared when overlapping data is read*/
			globalstatic_u8OverFlowFlag=TRUE;
			/*Reset Signal*/
			local_staticSetOverflowNext=FALSE;
			/*Reset Counter to start over*/
			globalstatic_irDataCounter=0;
		}

		globalstatic_ptru32Buffer[globalstatic_irDataCounter]=MSTK_u32GetElapsedTime();

		if(_u8CheckFrameStart(globalstatic_irDataCounter,globalstatic_ptru32Buffer))
			_voidSetIsStart();

		if(_u8CheckFrameRepeatEnd(&globalstatic_ptru32Buffer[globalstatic_irDataCounter]))
			_voidSetIsRepeat();

		if(globalstatic_irDataCounter<(IR_MAXSIGNALBUFFER-1ul))
		{
			globalstatic_irDataCounter++;
		}
		/*If About to overflow (next call)*/
		else if(globalstatic_irDataCounter==(IR_MAXSIGNALBUFFER-1ul))
		{
			/*Set Flag to Raise Overflow flag in Next function Call*/
			local_staticSetOverflowNext=TRUE;
		}
		else/*impossible*/
		{
			asm("nop");
		}
	}

	globalstatic_u8IRInterruptVirginityFlag=FALSE;

	MSTK_voidSetIntervalSingle(IR_MAXTICKSTOOUT,_voidTimeOutHandler);
}




/*
-This Function Checks A buffer for valid Frame and if one exists it returns it
 it should be recalled from the calling function once if it returns one of the following codes: IR_DATA_EXTRACTED_PARTIALBUF , IR_DATA_REPEATEXTRACTED_PARTIALBUF
 it returns the following codes in general:
*First:
	 * Return IR_DATA_NO_VALID_DATA_EMPTYBUF==0 if all buffer checked & there isn't a valid IR_Frame
*Second:
	 * Return IR_DATA_NO_VALID_DATA_PARTIALBUF==1 if all buffer checked & there isn't a valid IR_Frame
*Third:
	 * Return IR_DATA_EXTRACTED_EMPTYBUF==2 we have got Valid Frame extracted and no more data in buffer to be extracted
*Fourth:
	 * Return IR_DATA_EXTRACTED_PARTIALBUF==3 if we have got valid frame extracted and more frames needs to be checked Still in buffer
*Fifth:
	 * Return IR_DATA_REPEATEXTRACTED_EMPTYBUF==4 if we have got valid repeated frame extracted and no more data in buffer to be extracted
*Sixth:
	 * Return IR_DATA_REPEATEXTRACTED_PARTIALBUF==5 if we have got valid repeated frame extracted and more data Frames in buffer to be checked
*Seventh
	 * Return IR_LOGICERROR==88
*Eighth:
	 * Return IR_IMPOSSIBLETRET as default which couldn't happen
*Ninth:	  __CANCELLED__
     * Return IR_DATA_OVERFLOWBUF==77 if buffer overflow occurs
*Tenth:   __CANCELLED__
	 * Return IR_DATA_NON_RECEIVED==99 if there is no flag of received data was raised(Functional Call without check if global_u8IRFrameReceivedFlag is raised)*/
extern IR_enuExtractResult_t HIR_u8ExtractDataFromBuffer(u8 *copy_u8RetPtrAddress,u8 *copy_u8RetPtrData)
{
	static ir_type_index local_staticCurrentBufferIndex=0;
	ir_type_index UsedBufferSlots=0;
	u8 local_u8AllBufferCheckedFlag=0;
	u8 local_u8FrameStatus=IR_FRAMESTATUS_INVALID;
	volatile u32 tempBuf[33]={0};


		/*THE Received flag is SET by MSTK_Interrupt callback under some conditions
		  and cleared when the buffer is fully read by this function*/


		/*Copy the Next 33 signals from buffer*/
		_voidCopyFrame((u32*)tempBuf,(u32*)globalstatic_ptru32Buffer,local_staticCurrentBufferIndex);
		/*Get The Current Frame Status (VALID,Repeated,UnValid)*/
		local_u8FrameStatus=_u8FrameCheckNEC(0,tempBuf);

		if(local_u8FrameStatus==IR_LOGICERROR)
			return IR_LOGICERROR;

		/*if the current index is start of Valid*/
		if(local_u8FrameStatus==IR_FRAMESTATUS_VALID)
		{
			/*Return The Address and Data of the current frame through input arguments*/
			if(IR_LOGICERROR !=_u8GetFrameData(tempBuf,0,copy_u8RetPtrAddress, copy_u8RetPtrData))
			{
				/*Clear SignalBuffer Range for extracted data to avoid re-extract same values
				 * Clearing here is better than clearing after buffer filled to avoid
				 * critical sections*/
				_voidClearu32BufferRange((u32*)globalstatic_ptru32Buffer,local_staticCurrentBufferIndex,IR_FRAMEBITLENGTH);

				/*Increment the current buffer position and we have to break the loop and
				  go back to the calling function with flag raised to allow it to store data
				  and allow it then to recall again so that we continue extract
				  rest of data still in the buffer*/
				/*increment the buffer with 33 signal as it was valid frame*/
				if( (u32)local_staticCurrentBufferIndex + (u32)IR_FRAMEBITLENGTH  >= (u32)IR_MAXSIGNALBUFFER)
				{
					if(globalstatic_u8OverFlowFlag==TRUE)
					{
						/*Clear Overflow Flag only if the frame was VALID && Overflow is raised*/
						globalstatic_u8OverFlowFlag=FALSE;
					}
					else
						asm("nop");    //error

					/*Calculate how many buffer positions consumed by current frame before the end of buffer*/
					UsedBufferSlots=IR_MAXSIGNALBUFFER - local_staticCurrentBufferIndex ;
					/*Calculate the new local_staticCurrentBufferIndex after overflowing*/
					local_staticCurrentBufferIndex=IR_FRAMEBITLENGTH - UsedBufferSlots;
//					local_staticCurrentBufferIndex=IR_MAXSIGNALBUFFER;
					/*Set Local all bufferCheckedFlag to indicate that all buffer is checked*/
					local_u8AllBufferCheckedFlag=1;
				}
				else
				{
					local_staticCurrentBufferIndex += IR_FRAMEBITLENGTH;
				}
//				break;
			}
			else
			{
				return IR_LOGICERROR;
			}
		}
		else if(local_u8FrameStatus==IR_FRAMESTATUS_REPEATED)
		{
			/*Clear SignalBuffer Range for extracted data to avoid re-extract same values
			 * Clearing here is better than clearing after buffer filled to avoid
			 * critical sections*/
			_voidClearu32BufferRange((u32*)globalstatic_ptru32Buffer,local_staticCurrentBufferIndex,IR_REPEATEDFRAMEBITLENGTH);


			/*Increment the current buffer position and we have to break the loop and
			  go back to the calling function with flag raised to allow it to store data
			  and allow it then to recall again so that we continue extract
			  rest of data still in the buffer*/
			/*increment the buffer with 2 signal as it was repeated frame*/
			if( (u32)local_staticCurrentBufferIndex+ (u32)IR_REPEATEDFRAMEBITLENGTH  >= (u32)IR_MAXSIGNALBUFFER)
			{
				if(globalstatic_u8OverFlowFlag==TRUE)
				{
					/*Clear Overflow Flag only if the frame was repeated && Overflow is raised*/
					globalstatic_u8OverFlowFlag=FALSE;
				}
				else
					asm("nop");    //error

				/*Calculate how many buffer positions consumed by current frame before the end of buffer*/
				UsedBufferSlots=IR_MAXSIGNALBUFFER - local_staticCurrentBufferIndex ;
				/*Calculate the new local_staticCurrentBufferIndex after overflowing*/
				local_staticCurrentBufferIndex=IR_REPEATEDFRAMEBITLENGTH - UsedBufferSlots;
//				local_staticCurrentBufferIndex=IR_MAXSIGNALBUFFER;
				/*Set Local all bufferCheckedFlag to indicate that all buffer is checked*/
				local_u8AllBufferCheckedFlag=1;
			}
			else
			{
				local_staticCurrentBufferIndex += IR_REPEATEDFRAMEBITLENGTH;
			}
//			break;
		}
		else if(local_u8FrameStatus==IR_FRAMESTATUS_INVALID)
		{
			/*if we proceeded to here inside loop without break---> this means the following:
			 *The frame was non-valid and we didn't send back any data through input arguments
			  so we have to increment current buffer position and continue checking buffer in loop */
			if( (u32)local_staticCurrentBufferIndex + 1ul  >= (u32)IR_MAXSIGNALBUFFER)
			{
				/*Set Local all bufferCheckedFlag to indicate that all buffer is checked*/
				local_u8AllBufferCheckedFlag=1;
				if(globalstatic_u8OverFlowFlag==TRUE)
				{
					/*Clear Overflow Flag only if the frame was repeated && Overflow is raised*/
					globalstatic_u8OverFlowFlag=FALSE;
				}
				else
					asm("nop");    //error

				/*Reset buffer index if we are at the end of the buffer*/
				local_staticCurrentBufferIndex=0ul;
			}
			else
			{
				local_staticCurrentBufferIndex ++;
			}
		}
		else
			asm("nop");//error
	/*
	 *after loop ends and this should be checked by calling function -indicated by returned value-
	 *There are two reasons for while loop to end
	 * 1-we extracted at least one valid data and broke the loop
	 * 2-Signal Buffer fully checked and no valid data found
	 * 3-the signal buffer was fully checked and data already extracted if available and we verify
         this in the next if condition then we will have to reset the following flags:
	     received data flag,data pos, interrupt first occurrence , buffer pos.
	     and we have to indicate that no more data in buffer*/



	/* Case 1: We didn't got valid data */
	if((local_u8FrameStatus==IR_FRAMESTATUS_INVALID))
	{
		/* Case 1.1: we finished the buffer and we didn't got valid data
		 * return IR_DATA_NO_VALID_DATA_EMPTYBUF*/
		if(local_u8AllBufferCheckedFlag==1)
		{
			return IR_DATA_NO_VALID_DATA_EMPTYBUF;
		}
		/* Case 1.2: we finished the buffer and we didn't got valid data
		 * return IR_DATA_NO_VALID_DATA_PARTIALBUF*/
		else if(local_u8AllBufferCheckedFlag==0)
		{
			return IR_DATA_NO_VALID_DATA_PARTIALBUF;
		}
	}
	/*Case 2: If this was valid frame which we returned by now */
	else if(local_u8FrameStatus==IR_FRAMESTATUS_VALID)
	{
		if(local_u8AllBufferCheckedFlag)
		{
			/*Case 2.1:
			 * VALID Frame Returned by now and we have checked all the buffer:
			 *Then reset DataExtractedFlag & return IR_DATA_EXTRACTED_EMPTYBUF*/
			return IR_DATA_EXTRACTED_EMPTYBUF;
		}
		else if(local_u8AllBufferCheckedFlag==0)
		{
			/*Case 2.2
			 *Valid frame which we returned by now BUT we didn't finish checking all the buffer:
			 *Then we return IR_DATA_EXTRACTED_PARTIALBUF to be indicated by calling function to recall again
			 *Keep the DataExtracted Flag Set to indicate when re-enter function in next call
			  It is only reset in case of finishing buffer and extracted data
			  or in case of from the beginning no extract of data happened*/
			return IR_DATA_EXTRACTED_PARTIALBUF;
		}
	}
	/* Case 3: This was repeated frame which we returned by now*/
	else if((local_u8FrameStatus==IR_FRAMESTATUS_REPEATED))
	{
		if(local_u8AllBufferCheckedFlag)
		{
			/*Case 3.1:
			 * Repeated Frame Returned by now and we have checked all the buffer:
			 *Then reset DataExtractedFlag & return IR_DATA_REPEATEXTRACTED_EMPTYBUF*/
			return IR_DATA_REPEATEXTRACTED_EMPTYBUF;
		}
		else if(local_u8AllBufferCheckedFlag==0)
		{
			/*Case 3.2
			 *Repeated frame which we returned by now BUT we didn't finish checking all the buffer:
			 *Then we return IR_DATA_REPEATEXTRACTED_PARTIALBUF to be indicated by calling function to recall again
			 *Keep the DataExtracted Flag Set to indicate when re-enter function in next call
			  It is only reset in case of finishing buffer and extracted data
			  or in case of from the beginning no extract of data happened*/
			return IR_DATA_REPEATEXTRACTED_PARTIALBUF;
		}
	}
	else
	{
		asm("nop");
		return IR_LOGICERROR;
	}

	/*Default which will not occur*/
	return IR_IMPOSSIBLETRET;
}

static void _voidSetIsStart(void)
{
	globalstatic_u8IRIsStart=1;
}
static void _voidSetIsRepeat(void)
{
	globalstatic_u8IRIsRepeat=1;
}

extern void HIR_voidClrIsStart(void)
{
	globalstatic_u8IRIsStart=0;
}
extern void HIR_voidClrIsRepeat(void)
{
	globalstatic_u8IRIsRepeat=0;
}

extern u8 HIR_u8GetIsStart(void)
{
	return globalstatic_u8IRIsStart;
}
extern u8 HIR_u8GetIsRepeat(void)
{
	return globalstatic_u8IRIsRepeat;
}

/*Not Used for now*/
#if 0
static u8 _u8WriteRawSignalBuffer(u32* SrcArr)
{
	ir_type_index i;
	u8 retVal=E_OK;
	for(i=0;i<IR_MAXSIGNALBUFFER;i++)
	{
		if(NULL != (SrcArr+i))
			globalstatic_u32IRArrSignalTime[i]=SrcArr[i];
		else
			retVal=E_NOT_OK;
	}
	return retVal;
}
#endif
extern u8 HIR_u8ReadRawSignalBuffer(u32* DestArr)
{
	ir_type_index i;
	u8 retVal=E_OK;
	for(i=0;i<IR_MAXSIGNALBUFFER;i++)
	{
		if(NULL != (DestArr+i))
			DestArr[i]=globalstatic_ptru32Buffer[i];
		else
			retVal=E_NOT_OK;
	}
	return retVal;
}

extern u8 HIR_voidSetUsedBufferAddress(volatile u32* copy_PtrToBuffer,u16 BuffSize)
{
	if(copy_PtrToBuffer == NULL || BuffSize>IR_MAXSIGNALBUFFER)
	{
		globalstatic_ptru32Buffer=globalstatic_u32IRArrSignalTime;
		return E_NOT_OK;
	}
	else
	{
		globalstatic_ptru32Buffer=copy_PtrToBuffer;
	}
	return E_OK;
}

extern void HIR_voidResetRawSignalBuffer(void)
{
	memset((void*)globalstatic_ptru32Buffer,(int)0,IR_MAXSIGNALBUFFER*sizeof(u32));
}

static void _voidClearu32BufferRange(u32* Buffer,ir_type_index StartIndex,ir_type_index Length)
{
	u16 localIter=0,localBufBegin=0;

	/*In normal case -no crossing of buffer end boundaries*/
	if(  ((u32)StartIndex+(u32)Length ) <= ((u32)IR_MAXSIGNALBUFFER) )
	{
		memset( (void*)(Buffer+StartIndex), 0 , (u16)(Length*sizeof(u32)) );
	}
	/*in case overlapping(crossing of buffer end boundaries) will occur*/
	else
	{
		/*In case of Copying across buffer boundaries @the end of buffer*/
		for(localIter=0; localIter<Length; localIter++)
		{
			//before buffer end
			if(StartIndex+localIter < IR_MAXSIGNALBUFFER )
			{
				//memset( (void*)(Buffer+StartIndex), 0 , (u16)(Length*sizeof(u32)) );
				Buffer[StartIndex+localIter]=0ul;
			}
			//after buffer end
			else
			{
				//could use this formula buffer[Start+Iter-Length]=0;
				Buffer[localBufBegin]=0;
				localBufBegin++;
			}
		}
	}

}

static void _voidCopyFrame(u32* Dest,u32 *Src,ir_type_index SrcStartIndex)
{
	u16 localIter=0,localBufBegin=0;

	/*in case no buffer boundaries breached (no overlapping)*/
	if(  ( (u32)SrcStartIndex + (u32)IR_FRAMEBITLENGTH ) <= ( (u32)IR_MAXSIGNALBUFFER ) )
	{
		/*Case of normal copying:*/
		memcpy( (void*)Dest, (void*)(Src+SrcStartIndex) , (IR_FRAMEBITLENGTH*sizeof(u32)) );
	}
	/*in case overlapping will occur*/
	else
	{
		/*Overflow was reported*/
		if(globalstatic_u8OverFlowFlag==TRUE)
		{
			/*In case of Copying across buffer boundaries @the end of buffer*/
			for(localIter=0; localIter<IR_FRAMEBITLENGTH; localIter++)
			{
				/*as long as range not exceeding last element index*/
				if(SrcStartIndex+localIter < IR_MAXSIGNALBUFFER )
				{
					Dest[localIter]=Src[SrcStartIndex+localIter];
				}
				else
				{
					Dest[localIter]=Src[localBufBegin];
					localBufBegin++;
				}
			}
			/*Clear Overflow Flag*/
			/*Will be cleaned from calling function not here to fix the following issue:
			 * when overflow happens in middle of frame transmission (i.e when app context
			 * is still waiting before go read and extract data then lets say if buffer is 40 wide
			 * and current unread buffer index at index[20] then we received a noise bits then
			 * Transmission starts at index [30] so
			 * we got startbit[30] --some bits--, overflow ,--somebits-- and ends at index[22]
			 * then app go check while overflow flag is raised it starts checking from index [20]
			 * then copyframe function reset flag once index[20] is checked then what happens is that
			 * when reaching index[30] the overflow flag is equal zero and the function will not
			 * copy all frame it will only copies from index[30] to index[39] and rest of frame will be zero
			 */
			//globalstatic_u8OverFlowFlag=0;
		}
		//no overflow was reported
		else
		{
			//no overflow reported but it was because it is repeated frame (Length=2) & fitting before buffer end
			if(  ((u32)SrcStartIndex+(u32)IR_REPEATEDFRAMEBITLENGTH ) <= ((u32)IR_MAXSIGNALBUFFER) )
			{
				for(localIter=0; localIter<IR_REPEATEDFRAMEBITLENGTH; localIter++)
				{
					Dest[localIter]=Src[SrcStartIndex+localIter];
				}
			}
			/*no overflow was reported & it is not repeated frame fitting before ends
			 * Possible Cases:
			 * 1-Last element at buffer
			 * 2-Frame not yet completely received and it is started before end of buffer @33-)
			 */
			else
			{
				for(localIter=0; localIter<IR_FRAMEBITLENGTH; localIter++)
				{
					/*Fill destination with 0s*/
					Dest[localIter]=0u;
				}
			}
		}
	}
}


static u8 _u8FrameCheckNEC(ir_type_index copy_irFrameStartIndex,volatile u32* copy_u32PtrBuffer)
{
	ir_type_index local_irFrameAddressStartIndex,local_irFrameDataStartIndex;

	/*Take Care not to assign more than one bit into the Struct Flags*/

	struct local_structFrameFlagsNEC
	{
		u8 startflag:1;
		u8 addressflag:1;
		u8 dataflag:1;
		u8 validframeflag:1;
		u8 repeatflag:1;

	}local_FrameFlagObject={0,0,0,0,0};


	/*As long as there could be frame fitted in the last bits of buffer*/
	if(copy_irFrameStartIndex+IR_FRAMEBITLENGTH <= IR_MAXSIGNALBUFFER)
	{
		local_irFrameAddressStartIndex=copy_irFrameStartIndex+1;
		local_irFrameDataStartIndex=copy_irFrameStartIndex+17;
	}
	else
	{
		asm("nop");
		return IR_FRAMESTATUS_INVALID;
	}



	local_FrameFlagObject.validframeflag=0;

	local_FrameFlagObject.startflag=_u8CheckFrameStart(copy_irFrameStartIndex, copy_u32PtrBuffer);
	local_FrameFlagObject.addressflag=_u8CheckSubFrameInversion(local_irFrameAddressStartIndex, copy_u32PtrBuffer);
	local_FrameFlagObject.dataflag=_u8CheckSubFrameInversion(local_irFrameDataStartIndex, copy_u32PtrBuffer);
	local_FrameFlagObject.validframeflag=(local_FrameFlagObject.startflag&local_FrameFlagObject.addressflag
									&local_FrameFlagObject.dataflag);

	local_FrameFlagObject.repeatflag=_u8CheckFrameRepeat(copy_u32PtrBuffer+copy_irFrameStartIndex);

	/*Both Valid and Repeat Flag can't be equal to one*/
	if(local_FrameFlagObject.validframeflag==1 &&  local_FrameFlagObject.repeatflag==1)
	{
		return IR_LOGICERROR;
	}

	if(local_FrameFlagObject.validframeflag)
	{
		return IR_FRAMESTATUS_VALID;
	}
	else if(local_FrameFlagObject.repeatflag)
	{
		return IR_FRAMESTATUS_REPEATED;
	}

	return(IR_FRAMESTATUS_INVALID);
}

static void _voidTimeOutHandler(void)
{
	/*The BUS is steady since last negative edge for more than max ticks defined */
	asm("nop");
	globalstatic_u8IRInterruptVirginityFlag=TRUE;

}


static u8 _u8CheckSubFrameInversion(ir_type_index copy_irStart,volatile u32* copy_u32PtrFrame)
{
	u8 local_u8flag;
	ir_type_index i,local_ircounter;


	local_ircounter=0;

	for(i=copy_irStart;i<copy_irStart+8;i++)
	{
		if(   ((copy_u32PtrFrame[i]+copy_u32PtrFrame[i+8]) > (IR_SIGNAL0_LOWER+IR_SIGNAL1_LOWER))
		   && ((copy_u32PtrFrame[i]+copy_u32PtrFrame[i+8]) <(IR_SIGNAL0_HIGHER+IR_SIGNAL1_HIGHER))    )
			local_ircounter++;
		else
			local_ircounter=0;
	}

	if(local_ircounter==8)
		local_u8flag=1;
	else
		local_u8flag=0;

	return(local_u8flag);
}


static u8 _u8CheckFrameStart(ir_type_index copy_irStart,volatile u32* copy_u32PtrFrame)
{
	/*Take Care Not to return more than one Bit as this will be assigned into Struct bitfield*/

	u8 local_u8flag;


	if( (copy_u32PtrFrame[copy_irStart]>IR_STARTBIT_LOWER) && (copy_u32PtrFrame[copy_irStart]<IR_STARTBIT_HIGHER) )
		local_u8flag=1;
	else
		local_u8flag=0;

	return(local_u8flag);
}

static u8 _u8CheckFrameRepeat(volatile u32* copy_u32PtrFrame)
{
	/*Take Care Not to return more than one Bit as this will be assigned into Struct bitfield*/

	u8 local_u8flag;
	ir_type_index i=0;

#define FIRSTREPEATFRAME_CONDITION		    ((copy_u32PtrFrame[i]>IR_SIGN_REPEAT_FIRST_LOWER0) && (copy_u32PtrFrame[i]<IR_SIGN_REPEAT_FIRST_HIGHER0))
#define NONFIRSTREPEATFRAME_CONDITION		((copy_u32PtrFrame[i]>IR_SIGN_REPEAT_NONFIRST_LOWER0) && (copy_u32PtrFrame[i]<IR_SIGN_REPEAT_NONFIRST_HIGHER0))
#define REPEATCODEBURSE_CONDITION			((copy_u32PtrFrame[i+1]>IR_SIGN_REPEAT_LOWER1) && (copy_u32PtrFrame[i+1]<IR_SIGN_REPEAT_HIGHER1))

	if( REPEATCODEBURSE_CONDITION && (FIRSTREPEATFRAME_CONDITION || NONFIRSTREPEATFRAME_CONDITION) )
		local_u8flag=1;
	else
		local_u8flag=0;

	return(local_u8flag);
}

static u8 _u8CheckFrameRepeatEnd(volatile u32* copy_u32PtrFrame)
{
	/*Any Repeat pattern(either 41000 or 97000 must be followed by 11000)
	 * i.e 11000 can be used to indicate repeat sequence	 */

	/*Take Care Not to return more than one Bit as this will be assigned into Struct bitfield*/

	u8 local_u8flag;
	ir_type_index i=0;

	#define REPEAT_CONDITION			((copy_u32PtrFrame[i]>IR_SIGN_REPEAT_LOWER1) && (copy_u32PtrFrame[i]<IR_SIGN_REPEAT_HIGHER1))

	if( REPEAT_CONDITION )
		local_u8flag=1;
	else
		local_u8flag=0;

	return(local_u8flag);
}

static u8 _u8InterpetBitFrameNEC(volatile u32* copy_u32PtrBit)
{
	/*Take Care Not to return more than one Bit as this will be assigned into bitfield*/
	if((*copy_u32PtrBit>IR_SIGNAL0_LOWER) && (*copy_u32PtrBit<IR_SIGNAL0_HIGHER))
	{
		return 0;
	}
	else if((*copy_u32PtrBit>IR_SIGNAL1_LOWER) && (*copy_u32PtrBit<IR_SIGNAL1_HIGHER))
	{
		return 1;
	}

	/*error*/
	return IR_LOGICERROR;
}

static u8 _u8GetFrameData(volatile u32* copy_u32PtrBuffer,ir_type_index copy_start,
						u8 *copy_u8RetPtrAddress,u8 *copy_u8RetPtrData)
{
	u8 i;
	volatile u8 local_u8BitValue=IR_LOGICERROR;

	for(i=0;i<8;i++)
	{
		local_u8BitValue=_u8InterpetBitFrameNEC(copy_u32PtrBuffer+copy_start+1+i);
		if(local_u8BitValue!=IR_LOGICERROR)
		{
			ASSIGN_BIT(*(copy_u8RetPtrAddress),i,local_u8BitValue);

			local_u8BitValue=_u8InterpetBitFrameNEC(copy_u32PtrBuffer+copy_start+17+i);

			if(local_u8BitValue!=IR_LOGICERROR)
			{
				ASSIGN_BIT(*(copy_u8RetPtrData),i,local_u8BitValue);
			}
			else
				return IR_LOGICERROR;
		}
		else
			return IR_LOGICERROR;
	}

	return 1;
}





//Before chanGing function to only check one buffer pos per entry
#if 0
extern u8 HIR_u8ExtractDataFromBuffer(u8 *copy_u8RetPtrAddress,u8 *copy_u8RetPtrData)
{
#define EFRAME 			13500, 1106, 1107, 1131, 1131, 1134, 1159, 1081, 1186, 2244, 2193, 2270, 2246, 2271, 2248, 2192, 2297, 2218, 1157, 2222, 1131, 1104, 1160, 2272, 1159, 1107, 2245, 1135, 2243, 2219, 2272, 1107, 2220
	u32 testbuff[33]={EFRAME};
#include "TIMER2to5_interface.h"
	void temp (void)
	{
		asm("nop");
		return;
	}
	u16 mstkelapsed1=0;
	static ir_type_index local_staticCurrentBufferIndex=0;
	static u8 local_u8StaticDataExtractedFlag=0;
#if 0
	static u32* local_u32StaticPtrBuffer=0;
#endif
	u8 local_u8AllBufferCheckedFlag=0;
	u8 local_u8FrameStatus=IR_FRAMESTATUS_INVALID;
	volatile u32 tempBuf[33]={0};

#if 0
	/*This Function depends on being Called/Re-called if their is still data in buffer unchecked
	 * But what if when we recall the function it is called for different buffer? then:
	   we have to reset the static flags and static indexes */
	/*If the function used for some buffer before:*/
	if(local_u32StaticPtrBuffer!=0)
	{
		/*And if the new buffer not the same as the old one....*/
		if(local_u32StaticPtrBuffer!=globalstatic_u32IRArrSignalTime)
		{
			/*Reset Flags and indexes*/
			local_staticCurrentBufferIndex=0;
			local_u8StaticDataExtractedFlag=0;
		}
	}
#endif

	while((u32)local_staticCurrentBufferIndex + 1ul <= (u32)IR_MAXSIGNALBUFFER)
	{

		/*THE Received flag is SET by MSTK_Interrupt callback under some conditions
		  and cleared when the buffer is fully read by this function*/
#if 0
		if( global_u8IRFrameReceivedFlag==0)
		{
			/*Function called while their is no data received
			 * -the caller forgot to check received data flag-*/
			return IR_DATA_NON_RECEIVED;
		}
#endif
		/*Get The Current Frame Status (VALID,Repeated,UnValid)*/
		_voidCopyFrame((u32*)tempBuf,(u32*)testbuff,local_staticCurrentBufferIndex);
		local_u8FrameStatus=_u8FrameCheckNEC(0,tempBuf);

		if(local_u8FrameStatus==IR_LOGICERROR)
			return IR_LOGICERROR;

		/*if the current index is start of Valid*/
		if(local_u8FrameStatus==IR_FRAMESTATUS_VALID)
		{
			/*Return The Address and Data of the current frame through input arguments*/
			if(_u8GetFrameData(tempBuf,0,copy_u8RetPtrAddress, copy_u8RetPtrData)!=IR_LOGICERROR)
			{
				/*Raise flag to indicate that Data Extracted and sent to input arguments*/
				local_u8StaticDataExtractedFlag=1;

				/*Clear SignalBuffer Range for extracted data to avoid re-extract same values
				 * Clearing here is better than clearing after buffer filled to avoid
				 * critical sections*/
				_voidClearu32BufferRange((u32*)globalstatic_u32IRArrSignalTime,local_staticCurrentBufferIndex,IR_FRAMEBITLENGTH);

				/*Increment the current buffer position and we have to break the loop and
				  go back to the calling function with flag raised to allow it to store data
				  and allow it then to recall again so that we continue extract
				  rest of data still in the buffer*/
				/*increment the buffer with 33 signal as it was valid frame*/
				if( (u32)local_staticCurrentBufferIndex+ (u32)IR_FRAMEBITLENGTH  >= (u32)IR_MAXSIGNALBUFFER)
				{
					local_staticCurrentBufferIndex=IR_MAXSIGNALBUFFER;
				}
				else
				{
					local_staticCurrentBufferIndex += IR_FRAMEBITLENGTH;
				}
				break;
			}
			else
			{
				return IR_LOGICERROR;
			}
		}
		else if(local_u8FrameStatus==IR_FRAMESTATUS_REPEATED)
		{
			/*Raise flag to indicate that Data Extracted But not sent to input Arguments as it is repeated frame*/
			local_u8StaticDataExtractedFlag=1;

			/*Clear SignalBuffer Range for extracted data to avoid re-extract same values
			 * Clearing here is better than clearing after buffer filled to avoid
			 * critical sections*/
			_voidClearu32BufferRange((u32*)globalstatic_u32IRArrSignalTime,local_staticCurrentBufferIndex,IR_REPEATEDFRAMEBITLENGTH);


			/*Increment the current buffer position and we have to break the loop and
			  go back to the calling function with flag raised to allow it to store data
			  and allow it then to recall again so that we continue extract
			  rest of data still in the buffer*/
			/*increment the buffer with 2 signal as it was repeated frame*/
			if( (u32)local_staticCurrentBufferIndex+ (u32)IR_REPEATEDFRAMEBITLENGTH  >= (u32)IR_MAXSIGNALBUFFER)
			{
				local_staticCurrentBufferIndex=IR_MAXSIGNALBUFFER;
			}
			else
			{
				local_staticCurrentBufferIndex += IR_REPEATEDFRAMEBITLENGTH;
			}
			break;
		}
		else
		{
			/*if we proceeded to here inside loop without break---> this means the following:
			 *The frame was non-valid and we didn't send back any data through input arguments
			  so we have to increment current buffer position and continue checking buffer in loop */
			if( (u32)local_staticCurrentBufferIndex + 1ul  >= (u32)IR_MAXSIGNALBUFFER)
			{
				local_staticCurrentBufferIndex=IR_MAXSIGNALBUFFER;
			}
			else
			{
				local_staticCurrentBufferIndex ++;
			}
		}
	}

	/*
	 *after loop ends and this should be checked by calling function -indicated by returned value-
	 *There are two reasons for while loop to end
	 * 1-we extracted at least one valid data and broke the loop
	 * 2-Signal Buffer fully checked and no valid data found
	 * 3-the signal buffer was fully checked and data already extracted if available and we verify
         this in the next if condition then we will have to reset the following flags:
	     received data flag,data pos, interrupt first occurrence , buffer pos.
	     and we have to indicate that no more data in buffer*/
	if(local_staticCurrentBufferIndex+1ul > IR_MAXSIGNALBUFFER)
	{

		/*Reset Current Buffer Index*/
		local_staticCurrentBufferIndex=0;
		/*Set Local all bufferCheckedFlag to indicate that all buffer is checked*/
		local_u8AllBufferCheckedFlag=1;
#if 0
		/*Reset the static buffer to indicate that the next function call is new call
		 * with new buffer even it is the same buffer we start over*/
		local_u32StaticPtrBuffer=0;
#endif
		if(globalstatic_u8OverFlowFlag==0)
		{
			/*Clear Received Flag to indicate no more data in buffer to be extracted*/
			//global_u8IRFrameReceivedFlag=0;
			/*Clear Flag to indicate that the next interrupt fire is considered now the first time*/
			globalstatic_u8IRInterruptVirginityFlag=TRUE;
		}
	}

	/* it is not possible to not receive any data and didn't finish all buffer and you reached here
	 * so no case in which DataExtractedFlag and BufferCheckedFlag both are zero */

	/* Case 1: we finished the buffer and we didn't got data in this run
	 * return IR_DATA_NO_VALID_DATA*/
	if((local_u8FrameStatus==IR_FRAMESTATUS_INVALID) && local_u8AllBufferCheckedFlag==1)
	{
#if 0
		local_u32StaticPtrBuffer=0;
#endif
		return IR_DATA_NO_VALID_DATA;
	}
	/*Case 2: If this was valid frame which we returned by now */
	else if( (local_u8FrameStatus==IR_FRAMESTATUS_VALID) && local_u8StaticDataExtractedFlag )
	{
		if(local_u8AllBufferCheckedFlag)
		{
			/*Case 2.1:
			 * VALID Frame Returned by now and we have checked all the buffer:
			 *Then reset DataExtractedFlag & return IR_DATA_EXTRACTED_EMPTYBUF*/
			local_u8StaticDataExtractedFlag=0;
			return IR_DATA_EXTRACTED_EMPTYBUF;
		}
		else if(local_u8AllBufferCheckedFlag==0)
		{
			/*Case 2.2
			 *Valid frame which we returned by now BUT we didn't finish checking all the buffer:
			 *Then we return IR_DATA_EXTRACTED_PARTIALBUF to be indicated by calling function to recall again
			 *Keep the DataExtracted Flag Set to indicate when re-enter function in next call
			  It is only reset in case of finishing buffer and extracted data
			  or in case of from the beginning no extract of data happened*/
			return IR_DATA_EXTRACTED_PARTIALBUF;
		}
	}
	/* Case 3: This was repeated frame which we returned by now*/
	else if((local_u8FrameStatus==IR_FRAMESTATUS_REPEATED) && local_u8StaticDataExtractedFlag )
	{
		if(local_u8AllBufferCheckedFlag)
		{
			/*Case 3.1:
			 * Repeated Frame Returned by now and we have checked all the buffer:
			 *Then reset DataExtractedFlag & return IR_DATA_REPEATEXTRACTED_EMPTYBUF*/
			local_u8StaticDataExtractedFlag=0;
			return IR_DATA_REPEATEXTRACTED_EMPTYBUF;
		}
		else if(local_u8AllBufferCheckedFlag==0)
		{
			/*Case 3.2
			 *Repeated frame which we returned by now BUT we didn't finish checking all the buffer:
			 *Then we return IR_DATA_REPEATEXTRACTED_PARTIALBUF to be indicated by calling function to recall again
			 *Keep the DataExtracted Flag Set to indicate when re-enter function in next call
			  It is only reset in case of finishing buffer and extracted data
			  or in case of from the beginning no extract of data happened*/
			return IR_DATA_REPEATEXTRACTED_PARTIALBUF;
		}
	}
#if 0
	else if(globalstatic_u8OverFlowFlag ==1)
	{
		/*THE Received flag is SET by MSTK_Interrupt callback under some conditions
		  and cleared when the buffer is fully read by this function
		 *The following condition handles case:
		   *-Data received and written to buffer until buffer overflow occur disabled as it is handled by another method*/
		return IR_DATA_OVERFLOWBUF;
	}
#endif
	else
	{
		asm("nop");
		return IR_LOGICERROR;
	}

	/*Default which will not occur*/
	return IR_IMPOSSIBLETRET;
}

#endif
