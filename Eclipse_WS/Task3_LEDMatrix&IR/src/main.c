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

#define APP_HAMADABALL	(1)
#define APP_TEXTDISP	(2)
#define APPLICATION		APP_TEXTDISP

uint8 AppPlayFlag=0;

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




static void App_voidReadAndReact(u8* DataQ)
{
#define PAUSE (0u)
#define PLAY  (1u)

	static uint32 rewindcounter=0;
	static uint8 local_u8PlayState=PAUSE;
	static uint32 local_u32SpeedDelay=100000,local_u32ShiftSpeedDelay=600000;

	switch(*DataQ)
	{
	case HIR_POWER:
		/*multiples of 20,000*/
		HLEDMRX_voidDisplayShifting(LEDMRX_P, 320000);
		break;

	case HIR_MODE:
		/*multiples of 20,000*/
		HLEDMRX_voidDisplay(LEDMRX_M, 100000);
		break;

	case HIR_PLAY_PAUSE:
		local_u8PlayState ^= 1;
		if(local_u8PlayState==PLAY)
			AppPlayFlag=1;
		else
			AppPlayFlag=0;
		break;

	case HIR_REWIND:
		rewindcounter++;
#if APPLICATION==APP_HAMADABALL
		if(local_u32SpeedDelay<160000)
		{
			local_u32SpeedDelay+=20000;
//			HLEDMRX_voidDisplay(LEDMRX_SMinus,100000);
		}
#elif APPLICATION==APP_TEXTDISP
		if(local_u32ShiftSpeedDelay<800000)
		{
			local_u32ShiftSpeedDelay+=100000;
//			HLEDMRX_voidDisplay(LEDMRX_SMinus,100000);
		}
#endif
		else
		{
			asm("nop");
		}
		/*multiples of 20,000*/
//		HLEDMRX_voidDisplay(LEDMRX_Dance2, 100000);
		break;

	case HIR_FORWARD:
#if APPLICATION==APP_HAMADABALL
		if(local_u32SpeedDelay>40000)
		{
			local_u32SpeedDelay-=20000;
//			HLEDMRX_voidDisplay(LEDMRX_SPlus,100000);
		}
#elif APPLICATION==APP_TEXTDISP
		if(local_u32ShiftSpeedDelay>400000)
		{
			local_u32ShiftSpeedDelay-=100000;
//			HLEDMRX_voidDisplay(LEDMRX_SPlus,100000);
		}
#endif
		else
		{
			asm("nop");
		}

		/*multiples of 20,000*/
//		HLEDMRX_voidDisplay(LEDMRX_Dance3, 100000);
		break;



	case HIR_EQUALIZER:
		/*multiples of 20,000*/
		HLEDMRX_voidDisplay(LEDMRX_Ball0, 100000);
		HLEDMRX_voidDisplay(LEDMRX_Ball1, 100000);
		HLEDMRX_voidDisplay(LEDMRX_Ball2, 100000);
		HLEDMRX_voidDisplay(LEDMRX_Ball3, 100000);
		HLEDMRX_voidDisplay(LEDMRX_BallFall1, 100000);
		HLEDMRX_voidDisplay(LEDMRX_BallFall2, 100000);
		HLEDMRX_voidDisplay(LEDMRX_BallFall3, 100000);

		break;

	case HIR_VOLDOWN:
		/*multiples of 20,000*/
		HLEDMRX_voidDisplay(LEDMRX_Ball3, 100000);
		HLEDMRX_voidDisplay(LEDMRX_BallFall1, 100000);
		HLEDMRX_voidDisplay(LEDMRX_BallFall2, 100000);
		HLEDMRX_voidDisplay(LEDMRX_BallFall3, 100000);

		break;

	case HIR_VOLUP:
		/*multiples of 20,000*/
		HLEDMRX_voidDisplay(LEDMRX_Ball0, 100000);
		HLEDMRX_voidDisplay(LEDMRX_Ball1, 100000);
		HLEDMRX_voidDisplay(LEDMRX_Ball2, 100000);
		HLEDMRX_voidDisplay(LEDMRX_Ball3, 100000);


		break;

	case HIR_ZERO:
		/*multiples of 20,000*/
		HLEDMRX_voidDisplay(LEDMRX_0, 100000);
		break;

#if 0
	case 0:
		HLEDMRX_voidDisplay(LEDMRX_0, 100000);
		break;

	case 1:
		HLEDMRX_voidDisplay(LEDMRX_1, 100000);
		break;

	case 2:
		HLEDMRX_voidDisplay(LEDMRX_2, 100000);
		break;

	case 3:
		HLEDMRX_voidDisplay(LEDMRX_3, 100000);
		break;

	case 4:
		HLEDMRX_voidDisplay(LEDMRX_4, 100000);
		break;

	case 5:
		HLEDMRX_voidDisplay(LEDMRX_5, 100000);
		break;

	case 6:
		HLEDMRX_voidDisplay(LEDMRX_6, 100000);
		break;

	case 7:
		HLEDMRX_voidDisplay(LEDMRX_7, 100000);
		break;

	case 8:
		HLEDMRX_voidDisplay(LEDMRX_8, 100000);
		break;

	case 9:
		HLEDMRX_voidDisplay(LEDMRX_9, 100000);
		break;
#endif

	default:
	//	HLEDMRX_voidDisplay(LEDMRX_Ball0, 100000);
	//	MTIMR2to5_voidSetBusyWait(2,60000);
	//	MTIMR2to5_voidSetBusyWait(2,40000);
		break;
	}
	*DataQ=0;

#if APPLICATION==APP_HAMADABALL
	if(AppPlayFlag)
	{
		HLEDMRX_voidDisplay(LEDMRX_Ball0,  	  (uint32)local_u32SpeedDelay);
		HLEDMRX_voidDisplay(LEDMRX_Ball1,	  (uint32)local_u32SpeedDelay);
		HLEDMRX_voidDisplay(LEDMRX_Ball2,	  (uint32)local_u32SpeedDelay);
		HLEDMRX_voidDisplay(LEDMRX_Ball3, 	  (uint32)local_u32SpeedDelay);
		HLEDMRX_voidDisplay(LEDMRX_BallFall1, (uint32)local_u32SpeedDelay);
		HLEDMRX_voidDisplay(LEDMRX_BallFall2, (uint32)local_u32SpeedDelay);
		HLEDMRX_voidDisplay(LEDMRX_BallFall3, (uint32)local_u32SpeedDelay);
	}
#elif APPLICATION==APP_TEXTDISP
	if(AppPlayFlag)
	{
		HLEDMRX_voidDisplayShifting(LEDMRX_I,(uint32)local_u32ShiftSpeedDelay);
		HLEDMRX_voidDisplayShifting(LEDMRX_L,(uint32)local_u32ShiftSpeedDelay);
		HLEDMRX_voidDisplayShifting(LEDMRX_O,(uint32)local_u32ShiftSpeedDelay);
		HLEDMRX_voidDisplayShifting(LEDMRX_V,(uint32)local_u32ShiftSpeedDelay);
		HLEDMRX_voidDisplayShifting(LEDMRX_E,(uint32)local_u32ShiftSpeedDelay);
		HLEDMRX_voidDisplayShifting(LEDMRX_Y,(uint32)local_u32ShiftSpeedDelay);
		HLEDMRX_voidDisplayShifting(LEDMRX_O,(uint32)local_u32ShiftSpeedDelay);
		HLEDMRX_voidDisplayShifting(LEDMRX_U,(uint32)local_u32ShiftSpeedDelay);
	}
#endif
	//TODO: to be added when triggering accurate timings
	//MTIMR2to5_voidSetBusyWait(2,8000);
	return;
}


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
	volatile u16 local_u16msCounter;

	u8 local_u8IsAlarmFired0=FALSE,local_u8IsAlarmFired1=FALSE,local_u8IsAlarmFired2=FALSE,local_u8IsAlarmFired3=FALSE,local_u8IsAlarmFired4=FALSE;

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

	MGPIO_voidSetPinDirection(GPIOA,PIN10, OUTPUT_SPEED_2MHZ_PP);

	HIR_voidEnable(MEXTI_1,GPIOB);

	HLEDMRX_voidInit();

	MNVIC_voidInit();

	MNVIC_voidEnableInterrupt(TIM2_NVICPOS);



//TODO: handle alarms don't initialize time (needs to count relative)

	/*Set Timer each 1ms*/
	MTIMR2to5_voidSetTimerPeriodic(MTIMER_2, 1000, MTIM2_voidCyclic1ms);

	/*Set Alarm from timer2 context 0 for 5000ms)*/
	MTIMR2to5_voidSetAlarm_ms(MTIMER_2 , MTIMx_CONTEXT0, 30000);
	MTIMR2to5_voidSetAlarm_ms(MTIMER_2 , MTIMx_CONTEXT1, 60000);
	MTIMR2to5_voidSetAlarm_ms(MTIMER_2 , MTIMx_CONTEXT2, 90000);
	MTIMR2to5_voidSetAlarm_ms(MTIMER_2 , MTIMx_CONTEXT3, 120000);
	MTIMR2to5_voidSetAlarm_ms(MTIMER_2 , MTIMx_CONTEXT4, 150000);

	while (1)
	{
		/*Check if Alarms Fired*/
		if(E_NOT_OK==TIMx_u8IsAlarmFired(MTIMER_2 , MTIMx_CONTEXT0,&local_u8IsAlarmFired0))
			break;

		if(local_u8IsAlarmFired0==TRUE)
		{
			TIMx_u8ClrAlarmFired(MTIMER_2 , MTIMx_CONTEXT0);
			local_u8IsAlarmFired0=FALSE;

			MGPIO_voidTogglePin(GPIOA, PIN10);
			asm("nop");
		}

		if(E_NOT_OK==TIMx_u8IsAlarmFired(MTIMER_2 , MTIMx_CONTEXT1,&local_u8IsAlarmFired1))
			break;

		if(local_u8IsAlarmFired1==TRUE)
		{
			TIMx_u8ClrAlarmFired(MTIMER_2 , MTIMx_CONTEXT1);
			local_u8IsAlarmFired1=FALSE;

			MGPIO_voidTogglePin(GPIOA, PIN10);
			asm("nop");
		}

		if(E_NOT_OK==TIMx_u8IsAlarmFired(MTIMER_2 , MTIMx_CONTEXT2,&local_u8IsAlarmFired2))
			break;

		if(local_u8IsAlarmFired2==TRUE)
		{
			TIMx_u8ClrAlarmFired(MTIMER_2 , MTIMx_CONTEXT2);
			local_u8IsAlarmFired2=FALSE;

			MGPIO_voidTogglePin(GPIOA, PIN10);
			asm("nop");
		}

		if(E_NOT_OK==TIMx_u8IsAlarmFired(MTIMER_2 , MTIMx_CONTEXT3,&local_u8IsAlarmFired3))
			break;

		if(local_u8IsAlarmFired3==TRUE)
		{
			TIMx_u8ClrAlarmFired(MTIMER_2 , MTIMx_CONTEXT3);
			local_u8IsAlarmFired3=FALSE;

			MGPIO_voidTogglePin(GPIOA, PIN10);
			asm("nop");
		}

		if(E_NOT_OK==TIMx_u8IsAlarmFired(MTIMER_2 , MTIMx_CONTEXT4,&local_u8IsAlarmFired4))
			break;

		if(local_u8IsAlarmFired4==TRUE)
		{
			TIMx_u8ClrAlarmFired(MTIMER_2 , MTIMx_CONTEXT4);
			local_u8IsAlarmFired4=FALSE;

			MGPIO_voidTogglePin(GPIOA, PIN10);
			asm("nop");
		}
	}






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

