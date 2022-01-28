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


/*TODO:Check the case where global_irDatacounter is not aligned with localstaic_bufferindex
	   could be due to, Invalid signals which is checked every 1ms in case they came after valid repeat/start frame */
//TODO:Check the behavior of Partial returns not completely checked cause drop in frames
//TODO: try to remove isStart and isRepeat dependencies



#define BSW_CONTEXT					(MTIMx_CONTEXT0)
#define	BSW_CYCLIC_PERIOD_Ms		(2500ul)			 //(minimum for LEDMRX))
#define APP_CONTEXT					(MTIMx_CONTEXT1)
#define	APP_CYCLIC_PERIOD_Ms		(1000ul)
#define TIMER2_PERIOD_MS			(500ul)
/*Check that configured time cycles is more than TIMx Period or is not multiple of it*/
#if ((BSW_CYCLIC_PERIOD_Ms < TIMER2_PERIOD_MS)  ||  (APP_CYCLIC_PERIOD_Ms < TIMER2_PERIOD_MS) || (BSW_CYCLIC_PERIOD_Ms % TIMER2_PERIOD_MS != 0) || (APP_CYCLIC_PERIOD_Ms % TIMER2_PERIOD_MS != 0))
#error Wrong TASKs cyclic time Configured
#endif


uint8 AppPlayFlag=0;

#if 0
static void vid_clearBuffer(void* buff,u8 buffsize,u8 sizedatatype)
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
#endif

static void App_voidReadAndReact(u8 Data)
{
	#define PAUSE 							(0u)
	#define PLAY  							(1u)
	#define APP_HAMADABALL					(1u)
	#define APP_TEXTDISP					(2u)
	#define APP_COUNTERINCREMENT			(3u)
	#define APPLICATION						APP_HAMADABALL

#if APPLICATION == APP_HAMADABALL
	#define MIN_TIMEPERSEQUENCE				(50000ul)
	#define STEP_TIMEPERSEQUENCE			(10000ul)
	#define MAX_TIMEPERSEQUENCE				(100000ull)
#elif APPLICATION== APP_TEXTDISP
	#define MIN_TIMEPERSEQUENCE				(320000ull)
	#define STEP_TIMEPERSEQUENCE			(160000ull)
	#define MAX_TIMEPERSEQUENCE				(1120000ull)
#endif

	#define DISPLAYSPEEDTIME 				(250000ul)

#if 0
	#define MIN_TIMEPERSHIFTSEQUENCE		(320000ull)
	#define STEP_TIMEPERSHIFTSEQUENCE		(160000ull)
	#define MAX_TIMEPERSHIFTSEQUENCE		(1120000ull)
#endif

	u8 *LEDMRX_CNTR[10]={LEDMRX_0,LEDMRX_1,LEDMRX_2,LEDMRX_3,LEDMRX_4,LEDMRX_5,LEDMRX_6,LEDMRX_7,LEDMRX_8,LEDMRX_9};

	static u8 LEDMRX_Dance1[8]={28, 144, 84, 58, 84, 144, 28, 0};
	static u8 LEDMRX_Dance2[8]={48, 144, 84, 58, 84, 144, 24, 0};
	static u8 LEDMRX_Dance3[8]={24, 144, 84, 58, 84, 144, 48, 0};

	static u8 LEDMRX_Ball0[8]={16, 8, 234, 61, 106, 200, 128, 64};
	static u8 LEDMRX_Ball1[8]={16, 8, 234, 61, 106, 72, 64, 16};
	static u8 LEDMRX_Ball2[8]={16, 8, 234, 61, 42, 40, 32, 4};
	static u8 LEDMRX_Ball3[8]={16, 8, 234, 61, 42, 232, 128, 1};

	static u8 LEDMRX_BallFall1[8]={16, 8, 234, 61, 42, 232, 128, 4};
	static u8 LEDMRX_BallFall2[8]={16, 8, 234, 61, 42, 232, 128, 16};
	static u8 LEDMRX_BallFall3[8]={16, 8, 234, 61, 42, 232, 128, 64};

	static u8 LEDMRX_SPlus[8]={72, 84, 84, 36, 0, 16, 56, 16};
	static u8 LEDMRX_SMinus[8]={72, 84, 84, 36, 0, 16, 16, 16};

	static uint8 local_u8PlayState=PAUSE;

	static uint32 local_u32TimePerFrame=MAX_TIMEPERSEQUENCE;


	static uint32 LocalTimer=0;
	static u8 SeqID=1,LastSeqID=0;
	static u8 SpeedStep=0;
	static u8 SpeedFlag=FALSE;
	static u32 LocalSpeedTimer=0;

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

		case HIR_PLAY_PAUSE:
			local_u8PlayState ^= 1;
			if(local_u8PlayState==PLAY)
				AppPlayFlag=1;
			else
				AppPlayFlag=0;
			LocalTimer=0;
			break;

		case HIR_REWIND:
#if APPLICATION==APP_HAMADABALL || APPLICATION==APP_TEXTDISP
			if(local_u32TimePerFrame<MAX_TIMEPERSEQUENCE)
			{
				if(SpeedStep>0)
					SpeedStep--;
				else
					SpeedStep=0;

				local_u32TimePerFrame+=STEP_TIMEPERSEQUENCE;
				HLEDMRX_voidDisplayAsync(LEDMRX_CNTR[SpeedStep]);
			}
			else
			{
				local_u32TimePerFrame=MAX_TIMEPERSEQUENCE;
				HLEDMRX_voidDisplayAsync(LEDMRX_CNTR[SpeedStep]);
			}

			SpeedFlag=TRUE;
			LocalSpeedTimer=0;
#endif
			/*multiples of 20,000*/
	//		HLEDMRX_voidDisplay(LEDMRX_Dance2, 100000);
			break;

		case HIR_FORWARD:
#if APPLICATION==APP_HAMADABALL || APPLICATION==APP_TEXTDISP
			if(local_u32TimePerFrame>MIN_TIMEPERSEQUENCE)
			{
				if(SpeedStep<9)
					SpeedStep++;
				else
					SpeedStep=9;

				local_u32TimePerFrame-=STEP_TIMEPERSEQUENCE;
				HLEDMRX_voidDisplayAsync(LEDMRX_CNTR[SpeedStep]);
			}
			else
			{
				local_u32TimePerFrame=MIN_TIMEPERSEQUENCE;
				HLEDMRX_voidDisplayAsync(LEDMRX_CNTR[SpeedStep]);
			}
			SpeedFlag=TRUE;
			LocalSpeedTimer=0;
#endif

			break;

		case HIR_VOLDOWN:
#if APPLICATION==APP_COUNTERINCREMENT
			if(i>0)
				i--;
			else
				i=9;
			HLEDMRX_voidDisplayAsync(LEDMRX_CNTR[i]);
			if(local_u32TimePerFrame>APP_CYCLIC_PERIOD_Ms)
			{
				local_u32TimePerFrame-=APP_CYCLIC_PERIOD_Ms;
			}
#endif
			break;
		case HIR_VOLUP:
#if APPLICATION==APP_COUNTERINCREMENT
			i++;
			if(i>9)
				i=0;
			HLEDMRX_voidDisplayAsync(LEDMRX_CNTR[i]);

			if(local_u32TimePerFrame<APP_CYCLIC_PERIOD_Ms*1000)
			{
				local_u32TimePerFrame+=APP_CYCLIC_PERIOD_Ms;
			}
#endif
			break;

		default:
			break;
	}


#if APPLICATION == (APP_HAMADABALL || APPLICATION==APP_TEXTDISP)
	if(SpeedFlag==TRUE)
	{
		if(LocalSpeedTimer<DISPLAYSPEEDTIME)
		{
			LocalSpeedTimer+=APP_CYCLIC_PERIOD_Ms;
		}
		else
		{
			LocalSpeedTimer=DISPLAYSPEEDTIME;
			SpeedFlag=FALSE;
		}
	}
	else if(AppPlayFlag)
	{
		if(LocalTimer<=local_u32TimePerFrame)
		{
			LocalTimer+=APP_CYCLIC_PERIOD_Ms;
		}
		else
		{
			if(SeqID<8)
				SeqID++;
			else
				SeqID=1;

			LocalTimer=0;
		}

		if(SeqID!=LastSeqID)
		{
			LastSeqID=SeqID;
			switch(SeqID)
			{
#if APPLICATION==APP_HAMADABALL
				case 1:
					/*multiples of 20,000*/
					HLEDMRX_voidRequestStop();
					HLEDMRX_voidDisplayAsync(LEDMRX_Ball0);
					break;

				case 2:
					/*multiples of 20,000*/
					HLEDMRX_voidRequestStop();
					HLEDMRX_voidDisplayAsync(LEDMRX_Ball1);
					break;
				case 3:
					/*multiples of 20,000*/
					HLEDMRX_voidRequestStop();
					HLEDMRX_voidDisplayAsync(LEDMRX_Ball2);
					break;
				case 4:
					/*multiples of 20,000*/
					HLEDMRX_voidRequestStop();
					HLEDMRX_voidDisplayAsync(LEDMRX_Ball3);
					break;
				case 5:
					/*multiples of 20,000*/
					HLEDMRX_voidRequestStop();
					HLEDMRX_voidDisplayAsync(LEDMRX_BallFall1);
					break;
				case 6:
					/*multiples of 20,000*/
					HLEDMRX_voidRequestStop();
					HLEDMRX_voidDisplayAsync(LEDMRX_BallFall2);
					break;
				case 7:
					/*multiples of 20,000*/
					HLEDMRX_voidRequestStop();
					HLEDMRX_voidDisplayAsync(LEDMRX_BallFall3);
					break;

#elif APPLICATION==APP_TEXTDISP
				case 1:
					/*multiples of 20,000*/
					HLEDMRX_voidRequestStop();
					HLEDMRX_voidDisplayShiftingAsync(LEDMRX_I,(uint32)local_u32TimePerFrame);
					break;

				case 2:
					/*multiples of 20,000*/
					HLEDMRX_voidRequestStop();
					HLEDMRX_voidDisplayShiftingAsync(LEDMRX_L,(uint32)local_u32TimePerFrame);
					break;
				case 3:
					/*multiples of 20,000*/
					HLEDMRX_voidRequestStop();
					HLEDMRX_voidDisplayShiftingAsync(LEDMRX_O,(uint32)local_u32TimePerFrame);
					break;
				case 4:
					/*multiples of 20,000*/
					HLEDMRX_voidRequestStop();
					HLEDMRX_voidDisplayShiftingAsync(LEDMRX_V,(uint32)local_u32TimePerFrame);
					break;
				case 5:
					/*multiples of 20,000*/
					HLEDMRX_voidRequestStop();
					HLEDMRX_voidDisplayShiftingAsync(LEDMRX_E,(uint32)local_u32TimePerFrame);
					break;
				case 6:
					/*multiples of 20,000*/
					HLEDMRX_voidRequestStop();
					HLEDMRX_voidDisplayShiftingAsync(LEDMRX_Y,(uint32)local_u32TimePerFrame);
					break;
				case 7:
					/*multiples of 20,000*/
					HLEDMRX_voidRequestStop();
					HLEDMRX_voidDisplayShiftingAsync(LEDMRX_O,(uint32)local_u32TimePerFrame);
					break;
				case 8:
					/*multiples of 20,000*/
					HLEDMRX_voidRequestStop();
					HLEDMRX_voidDisplayShiftingAsync(LEDMRX_U,(uint32)local_u32TimePerFrame);
					break;
#endif
			}
		}
	}
	else
	{
		HLEDMRX_voidRequestStop();
	}
#endif

	return;
}



/*USED APPROACH:
 *
 The following assumptions and info are used regarding the IR frames reception
 1)HIR_u8ExtractDataFromBuffer takes max ~800Ms if it only checks one Buffer index only
 2)We received new frame which takes 67.5ms(from start to end)
 3)We received a repeated frame which takes 108ms (if we hold press on key)
 4)we couldn't receive more than 1 frame in < 108ms
 So From app context we check if:
 * 1)a valid start of IR frame is received then:
  	 1.1)since valid start is 13.5ms and whole frame is 67.5ms then we wait
  	     for 55ms and then read the received frame
  	 1.2)React on received frame if valid by displaying it asynchronous (no blocking)
 * 2)a valid repeat of IR frame is received
 * 	 2.1)then frame is already received we don't need to wait and we read it
 * 	 2.2)act on it by displaying it asynchronous (no blocking)
 */
void main()
{
	u8 volatile DataFlag=0,local_tempLastDataQ=0;
	u8 AddresQ,DataQ=0,local_u8ExtractIRDataFlag=0;
	u8 j=0,i=0;
	TIMxContext_t local_strCurrAlarmInfo;

	uint32 local_u32BSWBaseTicksCounter=0,local_u32APPBaseTicksCounter=0;		//Every increment= TIM2_BASETICK_Ms
	uint32 local_u32DelayCounter=0;
	u8 startdelayflag=0;

	/*Enable HSE system clock*/
	MRCC_voidInitSysClock();

	/*Enable GPIOA,B,C clock form APB2 Bus*/
	MRCC_voidEnableClock(MRCC_APB2,2);
	MRCC_voidEnableClock(MRCC_APB2,3);
	MRCC_voidEnableClock(MRCC_APB2,4);


	/*Enable AFIO*/
	MRCC_voidEnableClock(MRCC_APB2,0);
	/*Enable Timer2*/
	MRCC_voidEnableClock(MRCC_APB1,0);


	MSTK_voidInit();
	MTIMR2to5_voidInit(MTIMER_2, 7);


	HIR_voidEnable(MEXTI_1,GPIOB);

	HLEDMRX_voidInit();

	MNVIC_voidInit();

	MNVIC_voidEnableInterrupt(NVICPOS_TIM2);



	/*Set Timer each 500Ms*/
	MTIMR2to5_voidSetTimerPeriodic(MTIMER_2, TIMER2_PERIOD_MS, MTIMR2to5_voidHandleTIM2CycAlarms);


	/*ALARM for Core Functions (BSW main functions)*/
	MTIMR2to5_voidSetCycAlarm_Ms(MTIMER_2,BSW_CONTEXT, BSW_CYCLIC_PERIOD_Ms);


	/*Alarm for Application*/
	MTIMR2to5_voidSetCycAlarm_Ms(MTIMER_2,APP_CONTEXT, APP_CYCLIC_PERIOD_Ms);


	while (1)
	{
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
				asm("nop");//Error
				return;
			}

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


			/*Keep pooling to check if valid start is received*/
			if(HIR_u8GetIsStart())
			{
				HIR_voidClrIsStart();

				/*Start Delay Counter*/
				startdelayflag=1;
				local_u32DelayCounter=0;
			}

			//Each count is 1ms
			if(local_u32DelayCounter==56)
			{
				startdelayflag=0;
				local_u32DelayCounter=0;
				local_u8ExtractIRDataFlag=1;
			}

			if(local_u8ExtractIRDataFlag)
			{
				/*Read Raw signals and extract Pure Data*/
				DataFlag=HIR_u8ExtractDataFromBuffer(&AddresQ, &DataQ);
				if(DataFlag==IR_DATA_NO_VALID_DATA_EMPTYBUF)
				{
					/*Don't increment index of DataQ as it hasn't been updated (because of invalid data)*/
					i++;
					local_u8ExtractIRDataFlag=0;
				}
				else if(DataFlag==IR_DATA_NO_VALID_DATA_PARTIALBUF)
				{
					/*Don't increment index of DataQ as it hasn't been updated (because of invalid data)*/
					i++;
				}
				else if(DataFlag==IR_DATA_EXTRACTED_EMPTYBUF)
				{
					/*Save Last Received Data*/
					local_tempLastDataQ=DataQ;
					App_voidReadAndReact(DataQ);
					/*Break loop no data left*/
					i++;
					j++;
					local_u8ExtractIRDataFlag=0;
					continue;
				}
				else if(DataFlag==IR_DATA_EXTRACTED_PARTIALBUF)
				{
					/*Save Last Received Data*/
					local_tempLastDataQ=DataQ;
					App_voidReadAndReact(DataQ);
					/*Don't Break loop keep getting data*/
					i++;
					j++;
#if 0
					local_u8ExtractIRDataFlag=0;
					continue;
#endif
				}
				else if(DataFlag==IR_DATA_REPEATEXTRACTED_EMPTYBUF)
				{
					/*Handle following cases:
					 * 1-Case where last data received is still in DataQ buffer
					 * 2-Case where we reseted DataQ and we received repeated signal
					   (i.e First new data equals last data received before clearing buffer)*/
					DataQ=local_tempLastDataQ;
					/*Ignore repeated*/
					//TODO:Handle repeated
					DataQ=0;
					/*Break loop no data left*/
					i++;
					j++;
					local_u8ExtractIRDataFlag=0;
					continue;
					//break;
				}
				else if(DataFlag==IR_DATA_REPEATEXTRACTED_PARTIALBUF)
				{
					/*Handle following cases:
					 * 1-Case where last data received is still in buffer
					 * 2-Case where we reseted DataQ and we received repeated signal
					   (i.e First new data equals last data received before clearing buffer)*/
					DataQ=local_tempLastDataQ;
					/*Ignore repeated*/
					//TODO:Handle repeated
					DataQ=0;
					/*Don't Break loop keep getting data*/
					i++;
					j++;

#if 0
					local_u8ExtractIRDataFlag=0;
					continue;
#endif
				}

				else if(DataFlag==IR_LOGICERROR || DataFlag==IR_IMPOSSIBLETRET || DataFlag==IR_DATA_NON_RECEIVED)
					asm("nop");  //error



			}//condition checks for valid data
			App_voidReadAndReact(0);
		}//App alarm condition
	}//main while 1
}//main end

