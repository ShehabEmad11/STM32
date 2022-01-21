#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "STK_interface.h"
#include "NVIC_interface.h"
#include "EXTI_interface.h"
#include "IR_interface.h"
#include "TIMER2to5_interface.h"
#include "LEDMRX_interface.h"
#include "LEDMRX_database.h"
#include "IR_config.h"



#define MODE 0
#define TESTMODE 5
#if MODE==TESTMODE

#define EFRAME 			13754, 1106, 1107, 1131, 1131, 1134, 1159, 1081, 1186, 2244, 2193, 2270, 2246, 2271, 2248, 2192, 2297, 2218, 1157, 2222, 1131, 1104, 1160, 2272, 1159, 1107, 2245, 1135, 2243, 2219, 2272, 1107, 2220
#define MFRAME 			13685, 1134, 1134, 1133, 1133, 1134, 1133, 1134, 1134, 2248, 2248, 2247, 2246, 2248, 2246, 2246, 2246, 1133, 2246, 2246, 1133, 1134, 1133, 2246, 1134, 2246, 1134, 1134, 2247, 2246, 2246, 1133, 2247
#define ZEROFRAME 		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0
#define REPEATFRAME1	40743,11397
#define REPEATFRAME2	97281,11389

	volatile u32 testbuff[IR_MAXSIGNALBUFFER]={EFRAME,MFRAME,0,0,0};
	volatile u32 testTargetBuff[IR_MAXSIGNALBUFFER]={EFRAME,MFRAME,EFRAME,MFRAME,EFRAME};
	u8 testDataQ[]={'E',HIR_MODE,'E',HIR_MODE,0,HIR_MODE};

	u32 srcbuff[255]={13657,
			1127,
			1127,
			1127,
			1124,
			1127,
			1126,
			1126,
			1128,
			2236,
			2237,
			2214,
			2236,
			2262,
			2238,
			2237,
			2213,
			2265,
			1126,
			2211,
			1153,
			1103,
			1150,
			2237,
			1101,
			1127,
			2237,
			1153,
			2238,
			2238,
			2216,
			1150,
			2238};
	u32 testbuff[255]={0};

#endif


//u32 FRAME_P[40]={0,0,0,13754, 1106, 1107, 1131, 1131, 1134, 1159, 1081, 1186, 2244, 2193, 2270, 2246, 2271, 2248, 2192, 2297, 2218, 1157, 2222, 1131, 1104, 1160, 2272, 1159, 1107, 2245, 1135, 2243, 2219, 2272, 1107, 2220 ,0 ,0 ,0};
//u32 FRAME_M[40]={0,0,0,13685, 1134, 1134, 1133, 1133, 1134, 1133, 1134, 1134, 2248, 2248, 2247, 2246, 2248, 2246, 2246, 2246, 1133, 2246, 2246, 1133, 1134, 1133, 2246, 1134, 2246, 1134, 1134, 2247, 2246, 2246, 1133, 2247 ,0 ,0 ,0};

u32 FRAME_P[40]={2246, 2271, 2248, 2192, 2297, 2218, 1157, 2222, 1131, 1104, 1160, 2272, 1159, 1107, 2245, 1135, 2243, 2219, 2272, 1107, 2220,0,0,0,200,0,0,0,13754, 1106, 1107, 1131, 1131, 1134, 1159, 1081, 1186, 2244, 2193, 2270};
u32 FRAME_M[40]={2246, 2248, 2246, 2246, 2246, 1133, 2246, 2246, 1133, 1134, 1133, 2246, 1134, 2246, 1134, 1134, 2247, 2246, 2246, 1133, 2247,0,0,0,0,400,0,0,13685, 1134, 1134, 1133, 1133, 1134, 1133, 1134, 1134, 2248, 2248, 2247};


extern volatile u32 testbuff[40];


void _vidSettestbuff(u8 frameno)
{
	u16 i=0;
	u32* src=NULL;

	if(frameno==0)
		src=FRAME_P;
	else
		src=FRAME_M;

	for(i=0;i<40;i++)
		testbuff[i]=src[i];
}
u8 frameno=0;
#define APP_HAMADABALL	(1)
#define APP_TEXTDISP	(2)
#define APPLICATION		3


#define BSW_CONTEXT					(MTIMx_CONTEXT0)
#define	BSW_CYCLIC_PERIOD_Ms		(2500ul)			 //(minimum for LEDMRX))
#define APP_CONTEXT					(MTIMx_CONTEXT1)
#define	APP_CYCLIC_PERIOD_Ms		(1000ul)

/*Check that configured time cycles is more than TIMx Period or is not multiple of it*/
#if ((BSW_CYCLIC_PERIOD_Ms < 500ul)  ||  (APP_CYCLIC_PERIOD_Ms < 500ul) || (BSW_CYCLIC_PERIOD_Ms % 500ul != 0) || (APP_CYCLIC_PERIOD_Ms % 500ul != 0))
#error Wrong TASKs cyclic time Configured
#endif

#define TESTING

uint8 AppPlayFlag=0;

void vid_clearBuffer(void* buff,u8 buffsize,u8 sizedatatype)
{
	u8 i;
	for(i=0;i<buffsize;i++)
	{
		if(sizedatatype==1)
			*((u8*)buff+i)=0;
		else if(sizedatatype==2)
			*((u16*)buff+i)=0;
		else if(sizedatatype==4)
			*((u32*)buff+i)=0;
	}
}



static void App_voidReadAndReact(u8 Data)
{
	switch(Data)
	{
		case HIR_POWER:
			/*multiples of 20,000*/
			HLEDMRX_voidRequestStop();
			HLEDMRX_voidDisplayAsync(LEDMRX_P);
			break;

		case HIR_MODE:
			/*multiples of 20,000*/
			HLEDMRX_voidRequestStop();
			HLEDMRX_voidDisplayAsync(LEDMRX_M);
			break;
	}
	return;
}

void temp(void)
{
	asm("nop");
	return;
}


/*USED APPROACH:
 The following assumptions and info are used regarding the IR frames reception
 1)We received new frame which takes 67.5ms(from start to end)
 2)We received a repeated frame which takes 108ms (if we hold press on key)
 3)we couldn't receive more than 1 frame in <108ms
 So From app context we check if:
 * 1)a valid start of IR frame is received then:
  	 1.1)since valid start is 13.5ms and whole frame is 67.5ms then we wait
  	     for 55ms and then read the received frame
  	 1.2)React on received frame if valid by displaying it for 108ms
 * 2)a valid repeat of IR frame is received
 * 	 2.1)then frame is already received we don't need to wait and we read it
 * 	 2.2)act on it by displaying it for 108ms
 */
void main()
 {
	//This array to be removed after debugging and check on local var no need to array
	u8 volatile DataFlagArr[50]={0},local_tempLastDataQ=0;
	u8 AddresQ,DataQ[1]={0},local_u8ExtractIRDataFlag=0;
	u8 j=0,i=0;
//	volatile u16 local_u16msCounter;
	TIMxContext_t local_strCurrAlarmInfo;

	uint32 local_u32BSWBaseTicksCounter=0,local_u32APPBaseTicksCounter=0;		//Every increment= TIM2_BASETICK_Ms
	uint32 local_u32DelayCounter=0;
	u8 startdelayflag=0;
	u8 loopcntr=0;

	volatile u16 mstkelapsed1=0,mstkelapsed2=0;

	/*Enable HSE system clock*/
	MRCC_voidInitSysClock();
	/*Enable GPIOA,B,C clock form APB2 Bus*/
	MRCC_voidEnableClock(MRCC_APB2,2);
	MRCC_voidEnableClock(MRCC_APB2,3);
	MRCC_voidEnableClock(MRCC_APB2,4);


	/*Enable AFIO*/
	MRCC_voidEnableClock(MRCC_APB2,0);
	/*Enable Timer2*/
//	MRCC_voidEnableClock(MRCC_APB1,0);
	/*Enable Timer5*/
	//MRCC_voidEnableClock(MRCC_APB1,3);
	//TODO: handle MRCC_APB1,3 not setting TIM5
	//TODO: handle that buffer check takes ~ 10ms while BSW alarm needs 2.5ms
	//TODO: HIR_u8ExtractDataFromBuffer takes max ~800Ms if while loop become one iteration only
	//TODO: Application doesn't check buffer with appropriate timing
	//TODO:
	/* BUG: when overflow happens in middle of frame transmission (i.e when app context
	 * is still waiting before go read and extract data then lets say if buffer is 40 wide
	 * and current unread buffer index at index[20] then we received a noise bits then
	 * Transmission starts at index [30] so
	 * we got startbit[30] --some bits--, overflow ,--somebits-- and ends at index[22]
	 * then app go check while overflow flag is raised it starts checking from index [20]
	 * then copyframe function reset flag once index[20] is checked then what happens is that
	 * when reaching index[30] the overflow flag is equal zero and the function will not
	 * copy all frame it will only copies from index[30] to index[39] and rest of frame will be zero
	 */
	//TODO: clear overflow flag after making sure of all
	//TODO: re-check of localstaticindex increment step after overflow case
	//TODO: check boundaries of IR_MAXSIGNAL & #define IR_type



	*((volatile u32 *)0x4002101C)=(u32)0b101;

	MSTK_voidInit();

	MTIMR2to5_voidInit(MTIMER_2, 7);
	MTIMR2to5_voidInit(MTIMER_4, 7);


	MGPIO_voidSetPinDirection(GPIOA,PIN10, OUTPUT_SPEED_2MHZ_PP);

	HIR_voidEnable(MEXTI_1,GPIOB);

	HLEDMRX_voidInit();

	MNVIC_voidInit();

	MNVIC_voidEnableInterrupt(NVICPOS_TIM2);
	MNVIC_voidEnableInterrupt(NVICPOS_TIM4);

	//TODO: check if next function call to be moved------>Could be moved to TIM init or as system init and Tim2 be considered OS timer which don't needs input arguments at call
	//TODO: Get 500 from TIMx config/interface
	/*Set Timer each 500Ms*/
	MTIMR2to5_voidSetTimerPeriodic(MTIMER_2, 500, MTIMR2to5_voidHandleTIM2CycAlarms);


	/*ALARM for Core Functions (BSW main functions)*/
	MTIMR2to5_voidSetCycAlarm_Ms(MTIMER_2,BSW_CONTEXT, BSW_CYCLIC_PERIOD_Ms);


	/*Alarm for Application*/
	MTIMR2to5_voidSetCycAlarm_Ms(MTIMER_2,APP_CONTEXT, APP_CYCLIC_PERIOD_Ms);

#undef TESTING
#if defined TESTING
	if(HIR_voidSetUsedBufferAddress(&testbuff[0],sizeof(testbuff)/sizeof(u32) != E_OK);
		return;
#endif
#if 1
	//HLEDMRX_voidDisplayAsync(LEDMRX_E);
	//HLEDMRX_voidDisplayShiftingAsync(LEDMRX_F,10000000);

#if 0
	while(1)
	{
		MGPIO_voidSetPinValue(GPIOA, PIN10, GPIO_HIGH);
	}
#endif

	while (1)
	{
#if 0
		MTIMR2to5_voidSetTimerSingle(MTIMER_4, 65500, temp);
		/*Read Raw signals and extract Pure Data*/
		DataFlagArr[i]=HIR_u8ExtractDataFromBuffer(&AddresQ, &DataQ[j]);


		MTIMR2to5_u8GetElapsedTime(MTIMER_4, (u16*)(&mstkelapsed1));

		if(mstkelapsed1>800)
			asm("nop");
		continue;
#endif
/*===========================================Run BSW=======================================*/
/*===========================================Run BSW=======================================*/
/*===========================================Run BSW=======================================*/

		if(E_NOT_OK==MTIMR2to5_u8GetAlarmInfo(MTIMER_2, BSW_CONTEXT, &local_strCurrAlarmInfo))
		{
			asm("nop");
			return; //error
		}

		if(TRUE == local_strCurrAlarmInfo.IsAlarmFired)
		{
			if(TRUE == local_strCurrAlarmInfo.IsMissedShot)
			{
				MTIMR2to5_voidClrAlarmMissedShot(MTIMER_2, BSW_CONTEXT);
				//mstkelapsed=MSTK_u32GetElapsedTime();
				asm("nop");//Error
				//return;
			}
//			MSTK_voidSetIntervalSingle(1000000, temp);

			/*Clear Alarm Fired*/
			MTIMR2to5_voidClrAlarmFired(MTIMER_2, BSW_CONTEXT);

			/*Clear Local Flag*/
			local_strCurrAlarmInfo.IsAlarmFired = FALSE;

			/*Each increment corresponds to value BSW ALARM BASETICK_Ms passed*/
			local_u32BSWBaseTicksCounter++;

			/*Run BSW runnables*/
			HLEDMRX_voidMainFunction();
		}


/*======================================Run Application===================================*/
/*======================================Run Application===================================*/
/*======================================Run Application===================================*/


		if(E_NOT_OK==MTIMR2to5_u8GetAlarmInfo(MTIMER_2, APP_CONTEXT, &local_strCurrAlarmInfo))
		{
			asm("nop");
			return; //error
		}
		if(TRUE == local_strCurrAlarmInfo.IsAlarmFired)
		{
			if(TRUE == local_strCurrAlarmInfo.IsMissedShot)
			{
				MTIMR2to5_voidClrAlarmMissedShot(MTIMER_2, APP_CONTEXT);
				asm("nop");//Error
				return;
			}

			/*Clear Alarm Fired*/
			MTIMR2to5_voidClrAlarmFired(MTIMER_2, APP_CONTEXT);

			/*Clear Local Flag*/
			local_strCurrAlarmInfo.IsAlarmFired = FALSE;

			/*Each increment corresponds to value APP ALARM BASETICK_Ms passed*/
			local_u32APPBaseTicksCounter++;


			if(startdelayflag)
				local_u32DelayCounter++;



			/*Every 10 ms*/
			//if(local_u32APPBaseTicksCounter %2 ==0);

//			*DataQ=HIR_POWER;
//			App_voidReadAndReact(DataQ);

			/*Keep pooling to check if valid start is received*/
			if(HIR_u8GetIsStart())
			{
				HIR_voidClrIsStart();
//				HIR_voidClrIsRepeat();

				/*Start Delay Counter*/
				startdelayflag=1;
				local_u32DelayCounter=0;
#if 0
				do
				{
					/*Wait for 55ms*/
					MTIMR2to5_voidSetBusyWait(2, 1000);
					local_u16msCounter++;
				}while(local_u16msCounter<55);
				local_u16msCounter=0;

				local_u8ExtractIRDataFlag=1;
#endif
			}
#if 0
			/*Keep pooling to check if valid Repeat is received*/
			else if(HIR_u8GetIsRepeat())
			{
				/*Clear Flag*/
				HIR_voidClrIsRepeat();
				local_u8ExtractIRDataFlag=1;
			}
#endif

			//Each count is 1ms
			if(local_u32DelayCounter==56)
			{
				startdelayflag=0;
				local_u32DelayCounter=0;
				local_u8ExtractIRDataFlag=1;
			}

			if(local_u8ExtractIRDataFlag)
			{
				//while(1)
				//{
#if 0
					MTIMR2to5_voidSetTimerSingle(MTIMER_4, 65500, temp);
#endif
					/*Read Raw signals and extract Pure Data*/
					DataFlagArr[i]=HIR_u8ExtractDataFromBuffer(&AddresQ, &DataQ[j]);
#if 0
//					loopcntr++;
					//MSTK_voidSetIntervalSingle(1000000, temp);
					if(loopcntr==1)
					{
						MTIMR2to5_u8GetElapsedTime(MTIMER_4, (u16*)(&mstkelapsed1));
						asm("nop");
					}
					if(loopcntr==2)
					{
						MTIMR2to5_u8GetElapsedTime(MTIMER_4, (u16*)(&mstkelapsed2));
						asm("nop");
					}
#endif
#if 1
					if(DataFlagArr[i]==IR_DATA_NO_VALID_DATA_EMPTYBUF)
					{
						/*Don't increment index of DataQ as it hasn't been updated (because of invalid data)*/
						i++;
						local_u8ExtractIRDataFlag=0;
						//break;
					}
					else if(DataFlagArr[i]==IR_DATA_NO_VALID_DATA_PARTIALBUF)
					{
						/*Don't increment index of DataQ as it hasn't been updated (because of invalid data)*/
						i++;
						//break;
					}
					else if(DataFlagArr[i]==IR_DATA_EXTRACTED_EMPTYBUF)
					{
						/*Save Last Received Data*/
						local_tempLastDataQ=DataQ[j];
						App_voidReadAndReact(DataQ[j]);
						/*Break loop no data left*/
						i++;
						j++;
						local_u8ExtractIRDataFlag=0;
						_vidSettestbuff(frameno);
						TOG_BIT(frameno,0);
						//break;
					}
					else if(DataFlagArr[i]==IR_DATA_EXTRACTED_PARTIALBUF)
					{
						/*Save Last Received Data*/
						local_tempLastDataQ=DataQ[j];
						App_voidReadAndReact(DataQ[j]);
						/*Don't Break loop keep getting data*/
						i++;
						j++;

						_vidSettestbuff(frameno);
						TOG_BIT(frameno,0);
					}
					else if(DataFlagArr[i]==IR_DATA_REPEATEXTRACTED_EMPTYBUF)
					{
						/*Handle following cases:
						 * 1-Case where last data received is still in DataQ buffer
						 * 2-Case where we reseted DataQ and we received repeated signal
						   (i.e First new data equals last data received before clearing buffer)*/
						DataQ[j]=local_tempLastDataQ;
						/*Ignore repeated*/
						//TODO:Handle repeated
						DataQ[j]=0;
						//DataQ[j]=RepeatIncSeq%11;
						//RepeatIncSeq++;
				//		App_voidReadAndReact(DataQ);
						/*Break loop no data left*/
						i++;
						j++;
						local_u8ExtractIRDataFlag=0;
						//break;
					}
					else if(DataFlagArr[i]==IR_DATA_REPEATEXTRACTED_PARTIALBUF)
					{
						/*Handle following cases:
						 * 1-Case where last data received is still in buffer
						 * 2-Case where we reseted DataQ and we received repeated signal
						   (i.e First new data equals last data received before clearing buffer)*/
						DataQ[j]=local_tempLastDataQ;
						/*Ignore repeated*/
						//TODO:Handle repeated
						DataQ[j]=0;
						//DataQ[j]=RepeatIncSeq%11;
						//RepeatIncSeq++;
					//	App_voidReadAndReact(DataQ);
						/*Don't Break loop keep getting data*/
						i++;
						j++;
					}

					else if(DataFlagArr[i]==IR_LOGICERROR || DataFlagArr[i]==IR_IMPOSSIBLETRET || DataFlagArr[i]==IR_DATA_NON_RECEIVED)
						asm("nop");  //error


					if(i>49)
					{
						//Reset buffer index
						i=0;
						/*Reset Buffer*/
						vid_clearBuffer((void*)DataFlagArr,(sizeof(DataFlagArr)/sizeof(DataFlagArr[0])) ,sizeof(DataFlagArr[0]) );
					}
					if(j>0)
					{
						//Reset DataQ index
						j=0;
						/*Reset DataQbuffer*/
						vid_clearBuffer((void*)DataQ, (sizeof(DataQ)/sizeof(DataQ[0])) ,sizeof(DataQ[0]) );
					}
#endif
				//}//inner while1
#if 0
				local_u8ExtractIRDataFlag=0;

				if(i>49)
				{
					//Reset buffer index
					i=0;
					/*Reset Buffer*/
					vid_clearBuffer( (void*)DataFlagArr,(sizeof(DataFlagArr)/sizeof(DataFlagArr[0])) ,sizeof(DataFlagArr[0]) );
				}
				if(j>0)
				{
					//Reset DataQ index
					j=0;
					/*Reset DataQbuffer*/
					vid_clearBuffer((void*)DataQ,(sizeof(DataQ)/sizeof(DataQ[0])) ,sizeof(DataQ[0]));
				}
#endif
//				mstkelapsed=MSTK_u32GetElapsedTime();
//				asm("nop");
			}//condition checks for valid data
#if 0
			App_voidReadAndReact(DataQ);
#endif
	}//App alarm condition

}//main while 1


#endif






#if 0
	while(1)
	{
		/*Keep pooling to check if valid start is received*/
		if(HIR_u8GetIsStart())
		{
			HIR_voidClrIsStart();
			do
			{
				/*Wait for 55ms*/
				MTIMR2to5_voidSetBusyWait(2, 1000);
				local_u16msCounter++;
			}while(local_u16msCounter<55);
			local_u16msCounter=0;
			local_u8ExtractIRDataFlag=1;
		}
		/*Keep pooling to check if valid Repeat is received*/
		else if(HIR_u8GetIsRepeat())
		{
			/*Clear Flag*/
			HIR_voidClrIsRepeat();
			local_u8ExtractIRDataFlag=1;
		}

		if(local_u8ExtractIRDataFlag)
		{
			while(1)
			{
				/*Read Raw signals and extract Pure Data*/
				DataFlagArr[i]=HIR_u8ExtractDataFromBuffer(&AddresQ, &DataQ[j]);

				if(DataFlagArr[i]==IR_DATA_NO_VALID_DATA)
				{
					/*Don't increment index of DataQ as it hasn't been updated (because of invalid data)*/
					i++;
					break;
				}
				else if(DataFlagArr[i]==IR_DATA_EXTRACTED_EMPTYBUF)
				{
					/*Save Last Received Data*/
					local_tempLastDataQ=DataQ[j];
					App_voidReadAndReact(DataQ);
					/*Break loop no data left*/
					i++;
					j++;
					break;
				}
				else if(DataFlagArr[i]==IR_DATA_REPEATEXTRACTED_EMPTYBUF)
				{
					/*Handle following cases:
					 * 1-Case where last data received is still in DataQ buffer
					 * 2-Case where we reseted DataQ and we received repeated signal
					   (i.e First new data equals last data received before clearing buffer)*/
					DataQ[j]=local_tempLastDataQ;
					/*Ignore repeated*/
					//TODO:Handle repeated
					DataQ[j]=0;
					//DataQ[j]=RepeatIncSeq%11;
					//RepeatIncSeq++;
					App_voidReadAndReact(DataQ);
					/*Break loop no data left*/
					i++;
					j++;
					break;
				}
				else if(DataFlagArr[i]==IR_DATA_REPEATEXTRACTED_PARTIALBUF)
				{
					/*Handle following cases:
					 * 1-Case where last data received is still in buffer
					 * 2-Case where we reseted DataQ and we received repeated signal
					   (i.e First new data equals last data received before clearing buffer)*/
					DataQ[j]=local_tempLastDataQ;
					/*Ignore repeated*/
					//TODO:Handle repeated
					DataQ[j]=0;
					//DataQ[j]=RepeatIncSeq%11;
					//RepeatIncSeq++;
					App_voidReadAndReact(DataQ);
					/*Don't Break loop keep getting data*/
					i++;
					j++;
				}
				else if(DataFlagArr[i]==IR_DATA_EXTRACTED_PARTIALBUF)
				{
					/*Save Last Received Data*/
					local_tempLastDataQ=DataQ[j];
					App_voidReadAndReact(DataQ);
					/*Don't Break loop keep getting data*/
					i++;
					j++;
				}
				else if(DataFlagArr[i]==IR_LOGICERROR || DataFlagArr[i]==IR_IMPOSSIBLETRET || DataFlagArr[i]==IR_DATA_NON_RECEIVED)
					asm("nop");  //error

				if(i>49)
				{
					//Reset buffer index
					i=0;
					/*Reset Buffer*/
					vid_clearBuffer((void*)DataFlagArr,(sizeof(DataFlagArr)/sizeof(DataFlagArr[0])) ,sizeof(DataFlagArr[0]) );
				}
				if(j>0)
				{
					//Reset DataQ index
					j=0;
					/*Reset DataQbuffer*/
					vid_clearBuffer((void*)DataQ, (sizeof(DataQ)/sizeof(DataQ[0])) ,sizeof(DataQ[0]) );
				}
			}//inner while1

			local_u8ExtractIRDataFlag=0;

			if(i>49)
			{
				//Reset buffer index
				i=0;
				/*Reset Buffer*/
				vid_clearBuffer( (void*)DataFlagArr,(sizeof(DataFlagArr)/sizeof(DataFlagArr[0])) ,sizeof(DataFlagArr[0]) );
			}
			if(j>0)
			{
				//Reset DataQ index
				j=0;
				/*Reset DataQbuffer*/
				vid_clearBuffer((void*)DataQ,(sizeof(DataQ)/sizeof(DataQ[0])) ,sizeof(DataQ[0]));
			}
		}//condition checks for valid data

		App_voidReadAndReact(DataQ);
	}//main while(1)
	asm("nop"); 	//can't break the main while 1
#endif

}

