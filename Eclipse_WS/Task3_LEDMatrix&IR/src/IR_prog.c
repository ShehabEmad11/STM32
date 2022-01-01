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

volatile u8 global_u8IRFrameReceivedFlag;
volatile ir_type_index global_irDataCounter,global_irDataCounterRelativeTimeOut,global_irDataCounterRelativeContPress;
volatile u32 global_u32IRArrSignalTime[IR_MAXSIGNALBUFFER];
volatile u8	global_u8IRInterruptVirginityFlag=0,global_u8OverFlowFlag=0;
volatile u32 global_u32IRTicksContPressCount=0;

extern void HIR_voidEnable(u8 copy_u8ExtiLine,u8 copy_u8ExtiPort)
{
	/*The IR peripheral generates noise while not used so it might trigger interrupt on its own*/

	ir_type_index i;
	global_irDataCounter=0;
	global_irDataCounterRelativeTimeOut=0;
	global_irDataCounterRelativeContPress=0;

	/*Initialize Signal Buffer*/
	for(i=0;i<IR_MAXSIGNALBUFFER;i++)
	{	
		global_u32IRArrSignalTime[i]=0;
	}
	
	MSTK_voidInit();

	MEXTI_voidEnable(copy_u8ExtiLine,copy_u8ExtiPort,MEXTI_FALLING_EDGE,HIR_voidReceiveFrameNEC);
}	

/*This Function Receive Frames of NEC in global 32-bit array using EXTI,MSTK*/
extern void HIR_voidReceiveFrameNEC(void)
{
	/*This Function Receive Frames of NEC in global 32-bit array using EXTI,MSTK*/

	static u8 local_staticSetOverflowNext=0;


	/*if not first time to fire interrupt*/
	if(global_u8IRInterruptVirginityFlag!=0)
	{
		global_u32IRArrSignalTime[global_irDataCounter]=MSTK_u32GetElapsedTime();
		global_u32IRTicksContPressCount += global_u32IRArrSignalTime[global_irDataCounter];

		if( local_staticSetOverflowNext )
		{
			/*Flag overflow (it will be cleared when overlapping data is read*/
			global_u8OverFlowFlag=1;
			/*Reset Signal*/
			local_staticSetOverflowNext=0;
		}

		if(global_irDataCounter<IR_MAXSIGNALBUFFER)
		{
			global_irDataCounter++;
		}
		/*If About to overflow (next call)*/
		else if(global_irDataCounter==IR_MAXSIGNALBUFFER)
		{
			/*Flag Next Call to signaloverflow*/
			local_staticSetOverflowNext=1;

			/*Reset Counter to start over*/
			global_irDataCounter=0;
		}
		else/*impossible*/
		{
			asm("nop");
		}

		if(global_u32IRTicksContPressCount>=120000)
		{
			_voidReceptionHandler(IR_CONTEXT_CONTPRESS);
			global_u32IRTicksContPressCount=0;
		}
	}

	global_u8IRInterruptVirginityFlag=1;
	/*start/restart time and if no EXTI interrupt fire again until MAXTICKS OUT--> run handler*/
	MSTK_voidSetIntervalSingle(IR_MAXTICKSTOOUT,_voidTimeOutHandler);
}






/*
-This Function Checks A buffer for valid Frame and if one exists it returns it
 it should be recalled from the calling function once if it returns one of the following codes: IR_DATA_EXTRACTED_PARTIALBUF , IR_DATA_REPEATEXTRACTED_PARTIALBUF
 it returns the following codes in general:
*First:
     * Return IR_DATA_OVERFLOWBUF==77 if buffer overflow occurs
*Second
	 * Return IR_DATA_NON_RECEIVED==99 if there is no flag of received data was raised(Functional Call without check if global_u8IRFrameReceivedFlag is raised)
*Third
	 * Return IR_LOGICERROR==88
*Fourth:
	 * Return IR_DATA_NO_VALID_DATA==0 if there isn't a valid IR_Frame
*Fifth:
	 * Return IR_DATA_EXTRACTED_EMPTYBUF==1 we have got Valid Frame extracted and no more data in buffer to be extracted
*Sixth:
	 * Return IR_DATA_EXTRACTED_PARTIALBUF==2 if we have got valid frame extracted and more frames needs to be checked Still in buffer
*Seventh:
	 * Return IR_DATA_REPEATEXTRACTED_EMPTYBUF==3 if we have got valid repeated frame extracted and no more data in buffer to be extracted
*Eighth:
	 * Return IR_DATA_REPEATEXTRACTED_PARTIALBUF==4 if we have got valid repeated frame extracted and more data Frames in buffer to be checked
*Ninth:
	 * Return IR_IMPOSSIBLETRET as default which couldn't happen*/
extern u8 HIR_u8ExtractDataFromBuffer(volatile u32* copy_u32PtrBuffer,
									  u8 *copy_u8RetPtrAddress,u8 *copy_u8RetPtrData)
{
	static ir_type_index local_staticCurrentBufferIndex=0;
	static u8 local_u8StaticDataExtractedFlag=0;
	static u32* local_u32StaticPtrBuffer=0;
	u8 local_u8AllBufferCheckedFlag=0;
	u8 local_u8FrameStatus=IR_FRAMESTATUS_INVALID;

	volatile u32 tempBuf[33]={0};


	/*This Function depends on being Called/Re-called if their is still data in buffer unchecked
	 * But what if when we recall the function it is called for different buffer? then:
	   we have to reset the static flags and static indexes */
	/*If the function used for some buffer before:*/
	if(local_u32StaticPtrBuffer!=0)
	{
		/*And if the new buffer not the same as the old one....*/
		if(local_u32StaticPtrBuffer!=copy_u32PtrBuffer)
		{
			/*Reset Flags and indexes*/
			local_staticCurrentBufferIndex=0;
			local_u8StaticDataExtractedFlag=0;
		}
	}

	while((u32)local_staticCurrentBufferIndex + 1ul <= (u32)IR_MAXSIGNALBUFFER)
	{

		/*THE Received flag is SET by MSTK_Interrupt callback under some conditions
		  and cleared when the buffer is fully read by this function*/
		if( global_u8IRFrameReceivedFlag==0)
		{
			/*Function called while their is no data received
			 * -the caller forgot to check received data flag-*/
			return IR_DATA_NON_RECEIVED;
		}

		/*Get The Current Frame Status (VALID,Repeated,UnValid)*/
		_voidCopyFrame((u32*)tempBuf,(u32*)copy_u32PtrBuffer,local_staticCurrentBufferIndex);
		local_u8FrameStatus=HIR_u8FrameCheckNEC(0,tempBuf);

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
	if(local_staticCurrentBufferIndex>=IR_MAXSIGNALBUFFER)
	{

		/*Reset Current Buffer Index*/
		local_staticCurrentBufferIndex=0;
		/*Set Local all bufferCheckedFlag to indicate that all buffer is checked*/
		local_u8AllBufferCheckedFlag=1;
		/*Reset the static buffer to indicate that the next function call is new call
		 * with new buffer even it is the same buffer we start over*/
		local_u32StaticPtrBuffer=0;

		if(global_u8OverFlowFlag==0)
		{
			/*Clear Received Flag to indicate no more data in buffer to be extracted*/
			global_u8IRFrameReceivedFlag=0;
			/*Clear Flag to indicate that the next interrupt fire is considered now the first time*/
			global_u8IRInterruptVirginityFlag=0;


		EXTI_voidMaskLine(MEXTI_1);
			MSTK_voidStopInterval();
			global_u32IRTicksContPressCount=0;

			global_irDataCounterRelativeTimeOut=0;
			global_irDataCounterRelativeContPress=0;

			HIR_voidResetFrameBuffer();
			global_irDataCounter=0;
		EXTI_voidUNMaskLine(MEXTI_1);

			/*Relative handled in _voidReceptionHandler*/
//			global_irDataCounterRelativeTimeOut=0;
//			global_irDataCounterRelativeContPress=0

		}


		/*The Next should be called from the calling function not Here*/
		//HIR_voidResetFrameBuffer(copy_u32PtrBuffer,copy_ptrDataCounter);
		//global_irDataCounter=0;
		//global_irDataCounterRelativeTimeOut=0;
	}

#if 1
	/* it is not possible to not receive any data and didn't finish all buffer and you reached here
	 * so no case in which DataExtractedFlag and BufferCheckedFlag both are zero */

	/* Case 1: we finished the buffer and we didn't got data in this run
	 * return IR_DATA_NO_VALID_DATA*/
	if((local_u8FrameStatus==IR_FRAMESTATUS_INVALID) && local_u8AllBufferCheckedFlag==1)
	{
		local_u32StaticPtrBuffer=0;
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
	else if(global_u8OverFlowFlag ==1)
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

#endif


#if 0
	/*it is not possible to not receive any data and didn't finish all buffer and you reached here
	 * so no case in which DataExtractedFlag and BufferCheckedFlag both are zero */
	/*First Case:
	 *If we this was valid frame which we returned by now and we have checked all the buffer:
	 *Then reset DataExtractedFlag & return IR_DATA_EXTRACTED_EMPTYBUF ;*/
	if(local_u8StaticDataExtractedFlag && local_u8AllBufferCheckedFlag)
	{
		local_u8StaticDataExtractedFlag=0;
		return IR_DATA_EXTRACTED_EMPTYBUF;
	}
	/*Second Case we finished the buffer and we didn't got data
	 * return IR_DATA_NOT_EXTRACTED*/
	else if(local_u8StaticDataExtractedFlag==0 && local_u8AllBufferCheckedFlag==1)
	{
		local_u32StaticPtrBuffer=0;
		return IR_DATA_NOT_EXTRACTED_EMPTYBUF;
	}
	/*Third Case:
	 *If we got valid frame which we returned by now and we didn't finish checking all the buffer:
	 *Then we return IR_DATA_EXTRACTED_PARTIALBUF to be indicated by calling function to recall again*/
	else if(local_u8StaticDataExtractedFlag==1 && local_u8AllBufferCheckedFlag==0)
	{
		/*Keep the DataExtracted Flag un reset to indicate when re-enter function in next call
		 *It is only reset in case of finishing buffer and extracted data
		 or in case of from the beginning no extract of data happened*/
		return IR_DATA_EXTRACTED_PARTIALBUF;
	}
	else
		asm("nop");

#endif

	/*Default which will not occur*/
	return IR_IMPOSSIBLETRET;
}


static void _voidCopyFrame(u32* Dest,u32 *Src,u8 SrcStartIndex)
{
	u16 localIter=0,localBufBegin=0;

	/*overlapping will occur*/
	if(  ((u32)SrcStartIndex+(u32)IR_FRAMEBITLENGTH ) > ((u32)IR_MAXSIGNALBUFFER) )
	{
		/*Overflow was reported*/
		if( (global_u8OverFlowFlag==1) )
		{
			/*In case of Copying at the end of buffer*/
			for(localIter=0; localIter<IR_FRAMEBITLENGTH; localIter++)
			{
				if(SrcStartIndex+localIter <= IR_MAXSIGNALBUFFER )
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
			global_u8OverFlowFlag=0;
		}
		//no overflow was reported
		else
		{
			for(localIter=0; localIter<IR_FRAMEBITLENGTH; localIter++)
			{
				/*Fill destination with 0s*/
				Dest[localIter]=0u;
			}
		}
	}
	else
	{
		/*Case of normal copying*/
		memcpy( (void*)Dest, (void*)(Src+SrcStartIndex) , (IR_FRAMEBITLENGTH*4) );
	}

}


extern u8 HIR_u8FrameCheckNEC(ir_type_index copy_irFrameStartIndex,volatile u32* copy_u32PtrBuffer)
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

static void _voidReceptionHandler(u8 Context)
{
	static u8 localstaticFirstTimeOverflow=1;


	/*Make Sure that u had at least 33 data signals received since last timeout
	 *to avoid entering if condition mistakenly when u have for example 35 frame due to a
	  noise occurred after last frame received
	 *The relative counter must be reset with global counter */
	if( !global_u8OverFlowFlag )
	{
		/*In Case of no overflow has occurred, the relative position is as following:
		 * Formula: RelativePos= CurrentPos - LastPos
		 */
		if(Context==IR_CONTEXT_TIMEOUT)
			global_irDataCounterRelativeTimeOut = global_irDataCounter - global_irDataCounterRelativeTimeOut;

		else if(Context==IR_CONTEXT_CONTPRESS)
			global_irDataCounterRelativeContPress = global_irDataCounter - global_irDataCounterRelativeContPress;
		/*Set Default value when no longer overflow*/
		localstaticFirstTimeOverflow=1;
	}
	else if(global_u8OverFlowFlag)
	{
		if(localstaticFirstTimeOverflow)
		{
			/*In Case of overflow occurred to check that we received 33 signalbits from last time we use the following
			 * Formula:  RelativePos=  MAX_ToOverflow - LastPos +  CurrentPostition*/
			if(Context==IR_CONTEXT_TIMEOUT)
				global_irDataCounterRelativeTimeOut=IR_MAXSIGNALBUFFER - global_irDataCounterRelativeTimeOut  +  global_irDataCounter;
			else if(Context==IR_CONTEXT_CONTPRESS)
				global_irDataCounterRelativeContPress=IR_MAXSIGNALBUFFER - global_irDataCounterRelativeContPress  +  global_irDataCounter;

			localstaticFirstTimeOverflow=0;
		}
		else
		{
			if(Context==IR_CONTEXT_TIMEOUT)
				global_irDataCounterRelativeTimeOut = global_irDataCounter - global_irDataCounterRelativeTimeOut;
			else if(Context==IR_CONTEXT_CONTPRESS)
				global_irDataCounterRelativeContPress = global_irDataCounter - global_irDataCounterRelativeContPress;

		}
	}

	/*In case that we didn't receive any EXTI (low edge) interrupt for IR_MAXTICKSTOOUT(120ms)
	 * and before this period we had received at least IR_FRAMEBITLENGTH (33) signalBits
	 * Then we signal possible frame Reception */
	if(Context==IR_CONTEXT_TIMEOUT && global_irDataCounterRelativeTimeOut>=IR_FRAMEBITLENGTH)
	{
		global_u32IRTicksContPressCount=0;

		/*as we received 33 new Bits this could be frame so we signal frame*/
		global_u8IRFrameReceivedFlag=1;
		/*Make Relative equal to absolute to count again*/
		global_irDataCounterRelativeTimeOut=global_irDataCounter;
		global_irDataCounterRelativeContPress=global_irDataCounter;
		MGPIO_voidSetPinValue(GPIOA, PIN10,GPIO_LOW);
	}
	/*In case that we keep receiving CONTINUOUS Exti Signal (low edge) interrupt for IR_MAXTICKSCONTPRESS(150ms)
	 * and during this period we had received at least IR_REPEATEDFRAMEBITLENGTH (2) signalBits
	 * Then we signal possible frame Reception */
	else if(Context==IR_CONTEXT_CONTPRESS && global_irDataCounterRelativeContPress>=IR_REPEATEDFRAMEBITLENGTH)
	{
		/*as we received 33 new Bits this could be frame so we signal frame*/
		global_u8IRFrameReceivedFlag=1;
		/*Make Relative equal to absolute to count again*/
		global_irDataCounterRelativeTimeOut=global_irDataCounter;
		global_irDataCounterRelativeContPress=global_irDataCounter;
		MGPIO_voidSetPinValue(GPIOA, PIN10,GPIO_LOW);
	}
	/*timeout occurred without receiving 33+ signals (maybe exti fired by noise)*/
	else
	{
		asm("nop");
		MGPIO_voidSetPinValue(GPIOA, PIN10,GPIO_HIGH);
	}

	return;
}


static void _voidTimeOutHandler(void)
{
	/*The BUS is steady since last negative edge for more than max ticks defined */
	asm("nop");
	_voidReceptionHandler(IR_CONTEXT_TIMEOUT);
}




extern void HIR_voidResetFrameBuffer(void)
{
	ir_type_index i;

	for(i=0;i<IR_MAXSIGNALBUFFER;i++)
		global_u32IRArrSignalTime[i]=0;
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



#if 0
/*This function will be canceled as i consider it application function not driver
extern u8 HIR_u8CheckAndGetData(volatile u32* copy_u32PtrBuffer,u8 *copy_u8RetPtrAddrQueue
								,u8 *copy_u8RetPtrDataQueue,u8 copy_u8CurrentQueueIndex)*/
{
	/*If No Data Found the function will return*/


	u8 local_u8Flag,j;
	local_u8Flag=0;
	j=0;

	if(global_u8IRFrameReceivedFlag)
	{
		do{
			local_u8Flag=HIR_u8ExtractDataFromBuffer(copy_u32PtrBuffer, copy_u8RetPtrAddrQueue+copy_u8CurrentQueueIndex
																	  , copy_u8RetPtrDataQueue+copy_u8CurrentQueueIndex);
			copy_u8CurrentQueueIndex++;
		}while(local_u8Flag==125);
	}

	return local_u8Flag;
}
#endif
