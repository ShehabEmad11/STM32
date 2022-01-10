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

extern volatile u8 global_u8IRFrameReceivedFlag;
extern volatile u32 global_u32IRArrSignalTime[IR_MAXSIGNALBUFFER];
extern volatile u8 global_u8IRIsStart,global_u8IRIsRepeat;

#define NORMALMODE  (1)
#define TESTMODE    (2)

#define MODE (NORMALMODE)

volatile u8 looptypestartflag=0;
volatile u32 validdatacntr=0;
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

#if MODE==TESTMODE

#define EFRAME 			13754, 1106, 1107, 1131, 1131, 1134, 1159, 1081, 1186, 2244, 2193, 2270, 2246, 2271, 2248, 2192, 2297, 2218, 1157, 2222, 1131, 1104, 1160, 2272, 1159, 1107, 2245, 1135, 2243, 2219, 2272, 1107, 2220
#define MFRAME 			13685, 1134, 1134, 1133, 1133, 1134, 1133, 1134, 1134, 2248, 2248, 2247, 2246, 2248, 2246, 2246, 2246, 1133, 2246, 2246, 1133, 1134, 1133, 2246, 1134, 2246, 1134, 1134, 2247, 2246, 2246, 1133, 2247
#define ZEROFRAME 		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0
#define REPEATFRAME1	40743,11397
#define REPEATFRAME2	97281,11389

	volatile u32 testbuff[IR_MAXSIGNALBUFFER]={EFRAME,MFRAME,0,0,0};
	volatile u32 testTargetBuff[IR_MAXSIGNALBUFFER]={EFRAME,MFRAME,EFRAME,MFRAME,EFRAME};
	u8 testDataQ[]={'E',HIR_MODE,'E',HIR_MODE,0,HIR_MODE};
#endif


#if 0
	volatile u8 ToBeRepresented[40]={0};
	u8 k=0;

static void App_voidReadAndReact(u8* DataQ,u8 indexx)
{
	static u8 LastData;

	if(indexx != 0)
	{
		if(DataQ[indexx] != LastData)
		{
			if(DataQ[indexx]==HIR_POWER)
				HLEDMRX_voidDisplay(LEDMRX_P, 250000);
			else if(DataQ[indexx]==HIR_MODE)
				HLEDMRX_voidDisplay(LEDMRX_M, 250000);

			ToBeRepresented[k++]=DataQ[indexx];
		}
	}
	else if (indexx==0)
	{
		if(DataQ[indexx]==HIR_POWER)
			HLEDMRX_voidDisplay(LEDMRX_P, 250000);
		else if(DataQ[indexx]==HIR_MODE)
			HLEDMRX_voidDisplay(LEDMRX_M, 250000);

		ToBeRepresented[k++]=DataQ[indexx];
	}

	LastData=DataQ[indexx];

	if(k>=40)
	{
		k=0;
	}


}
#endif


static void App_voidReadAndReactUpdated(u8* DataQ)
{
	u8 i;
//	HLEDMRX_voidDisplay(LEDMRX_O, 200000);
//	return;


	for(i=0;i<1;i++)
	{
		if(DataQ[i] != 0)
		{
			validdatacntr++;
			//EXTI_voidMaskLine(MEXTI_1);

			if(DataQ[i]==HIR_POWER)
			{
				HLEDMRX_voidDisplay(LEDMRX_P, 60000);
				HLEDMRX_voidDisplay(LEDMRX_P, 48000);
			}
			else if(DataQ[i]==HIR_MODE)
			{
				HLEDMRX_voidDisplay(LEDMRX_M, 60000);
				HLEDMRX_voidDisplay(LEDMRX_M, 48000);
			}
			else if(DataQ[i]==1)
			{
				HLEDMRX_voidDisplay(LEDMRX_0, 60000);
				HLEDMRX_voidDisplay(LEDMRX_0, 48000);
			}
			else if(DataQ[i]==2)
			{
				HLEDMRX_voidDisplay(LEDMRX_1, 60000);
				HLEDMRX_voidDisplay(LEDMRX_1, 48000);
			}
			else if(DataQ[i]==3)
			{
				HLEDMRX_voidDisplay(LEDMRX_2, 60000);
				HLEDMRX_voidDisplay(LEDMRX_2, 48000);
			}
			else if(DataQ[i]==4)
			{
				HLEDMRX_voidDisplay(LEDMRX_3, 60000);
				HLEDMRX_voidDisplay(LEDMRX_3, 48000);
			}
			else if(DataQ[i]==5)
			{
				HLEDMRX_voidDisplay(LEDMRX_4, 60000);
				HLEDMRX_voidDisplay(LEDMRX_4, 48000);
			}
			else if(DataQ[i]==6)
			{
				HLEDMRX_voidDisplay(LEDMRX_5, 60000);
				HLEDMRX_voidDisplay(LEDMRX_5, 48000);
			}
			else if(DataQ[i]==7)
			{
				HLEDMRX_voidDisplay(LEDMRX_6, 60000);
				HLEDMRX_voidDisplay(LEDMRX_6, 48000);
			}
			else if(DataQ[i]==8)
			{
				HLEDMRX_voidDisplay(LEDMRX_7, 60000);
				HLEDMRX_voidDisplay(LEDMRX_7, 48000);
			}
			else if(DataQ[i]==9)
			{
				HLEDMRX_voidDisplay(LEDMRX_8, 60000);
				HLEDMRX_voidDisplay(LEDMRX_8, 48000);
			}
			else if(DataQ[i]==10)
			{
				HLEDMRX_voidDisplay(LEDMRX_9, 60000);
				HLEDMRX_voidDisplay(LEDMRX_9, 48000);
			}
			else if(DataQ[i]==HIR_PLAY)
			{
				HLEDMRX_voidDisplay(LEDMRX_Dance1, 60000);
				HLEDMRX_voidDisplay(LEDMRX_Dance1, 48000);
			}
			else if(DataQ[i]==HIR_REWIND)
			{
				HLEDMRX_voidDisplay(LEDMRX_Dance2, 60000);
				HLEDMRX_voidDisplay(LEDMRX_Dance2, 48000);
			}
			else if(DataQ[i]==HIR_FORWARD)
			{
				HLEDMRX_voidDisplay(LEDMRX_Dance3, 60000);
				HLEDMRX_voidDisplay(LEDMRX_Dance3, 48000);
			}

			else
			{
				MTIMR2to5_voidSetBusyWait(2,60000);
				MTIMR2to5_voidSetBusyWait(2,48000);
			}
			DataQ[i]=0;

			//EXTI_voidUNMaskLine(MEXTI_1);
		}
		if(validdatacntr>1)
			asm("nop");
	}
	validdatacntr=0;


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

/*TODO:
2-Timeout approach(we are HERE)
 it has the following problem:
 Q)it Checks after stoping getting EXTI for 110 ms so it ignores case of long press REPEAT  (we receive EXTI but no TIMEOUT ISR)
 Ans)From app context if for 108+ms we didn't get (neither received data flag-raised in timeout ISR-
		nor we received globalNotreceivedFlag -raised also in timeout ISR if it hits but no 33 frame where received- )
		and we have ongoing MSTK_interval
	 it means that we keep getting EXTI ISR but no TIMEOUT ISR i.e repeat state

 Plan:
1-from app context Get from timeout ISR following status:
 1.1)fired & DataReceived since last timeout (>=33)
	1.1.1)GetFrame and act on it
 1.2)fired & DataReceived since last timeout(>2 && <33)
 	1.2.1)check if repeated frames and act on them
 1.3)fired & no data received since last timeout (<2)
 	1.3.1)No data received flag
 1.4)Not Fired & ONGOING MSTK interval
 1.5)Not fired and no ongoing MSTK interval
2-Clean timeout ISR status after checking it from app context
*/


//Todo: Check
/*this approach depend on fact that we will poll on start/repeat signal every < 1ms
 * could be extended to 40ms or 67 depending on max least time between frames or depending
 * on LEDMRX delay time
 */

void main()
{
	//This array to be removed after debugging and check on local var no need to array
	u8 volatile DataFlagArr[50]={0};
	u8 AddresQ,DataQ[50]={0},local_u8GetDataFlag=0;
	u8 j=0,i=0;
	u8 volatile local_tempLastDataQ=0;

	volatile u32 possibleframescounter=0,symbolcounter=0,local_u32msCounter=0,dbgloopcounter=0;

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


volatile u32 dbgrepeatcntr=0, RepeatIncSeq=1;

#if MODE==TESTMODE
	global_u8IRFrameReceivedFlag=1;
#endif
	while(1)
	{
#if 1
		if(global_u8IRIsStart)
		{
			global_u8IRIsStart=0;
			do
			{
				MTIMR2to5_voidSetBusyWait(2, 1000);
				local_u32msCounter++;
			}while(local_u32msCounter<55);
			local_u32msCounter=0;
			local_u8GetDataFlag=1;
			looptypestartflag=1;
		}
		else if(global_u8IRIsRepeat)
		{
			global_u8IRIsRepeat=0;
			local_u8GetDataFlag=1;
			dbgrepeatcntr++;
		}

		if(local_u8GetDataFlag)
#endif
#if 0
		if(1)
#endif
		{
			//Todo on current state we need two things
			//1-Handle case if buffer is filled with good data but 110ms delay cause bad view
			//2-Handle case of bad buffer why not getting data right.
			//3-use timer context mehtod or use timer in exti interrupt to flag reception of some kind of frame
			//4-use timer here as debug to check when holding remote what is interval between each flag set and other
			//to make sure that repeat flag is set on time (as it is the most important one, as you cant press on button on/off faster than hold on it)
			while(1)
			{
//				for(k=0;k<5;k++)
//			//		MTIMR2to5_voidSetBusyWait(2, 10000);
//				for(k=0;k<255;k++)
//					testbuff[k]=srcbuff[k];

				#if MODE==TESTMODE
					DataFlagArr[i]=HIR_u8ExtractDataFromBuffer(testbuff, &AddresQ, &DataQ[j]);
				#else //global_u32IRArrSignalTime
					DataFlagArr[i]=HIR_u8ExtractDataFromBuffer(global_u32IRArrSignalTime, &AddresQ, &DataQ[j]);
				#endif

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
						App_voidReadAndReactUpdated(DataQ);
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
						//DataQ[j]=RepeatIncSeq%11;
						//RepeatIncSeq++;
						App_voidReadAndReactUpdated(DataQ);
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
						//DataQ[j]=RepeatIncSeq%11;
						//RepeatIncSeq++;
						App_voidReadAndReactUpdated(DataQ);
						/*Don't Break loop keep getting data*/
						i++;
						j++;
					}
					else if(DataFlagArr[i]==IR_DATA_EXTRACTED_PARTIALBUF)
					{
						/*Save Last Received Data*/
						local_tempLastDataQ=DataQ[j];
						App_voidReadAndReactUpdated(DataQ);
						/*Don't Break loop keep getting data*/
						i++;
						j++;
					}
					else if(DataFlagArr[i]==IR_LOGICERROR || DataFlagArr[i]==IR_IMPOSSIBLETRET || DataFlagArr[i]==IR_DATA_NON_RECEIVED)
						asm("nop");  //error

					if(i>49)
					{
						//we received more than 50 possible frames
						possibleframescounter+=50;
						//Reset buffer index
						i=0;
						/*Reset Buffer*/
						vid_clearBuffer((void*)DataFlagArr,(sizeof(DataFlagArr)/sizeof(DataFlagArr[0])) ,sizeof(DataFlagArr[0]) );
					}
					if(j>0)
					{
						//we stored more than 50 valid data symbol received
						symbolcounter+=50;
						/*Save Last Data Received to handle case if the next check data is repeated
						 * [j-1] because we incremented j above*/
//						local_tempLastDataQ=DataQ[j-1];
						//Reset DataQ index
						j=0;
						//App_voidReadAndReactUpdated(DataQ);
						/*Reset DataQbuffer*/
						vid_clearBuffer((void*)DataQ, (sizeof(DataQ)/sizeof(DataQ[0])) ,sizeof(DataQ[0]) );
					}
					//loop counter for debugging
					dbgloopcounter++;
					if(dbgloopcounter>1)
						asm("nop");
				}//inner while1

			local_u8GetDataFlag=0;
#if 1


			if(i>49)
			{
				//we received more than 50 possible frames
				possibleframescounter+=50;
				//Reset buffer index
				i=0;
				/*Reset Buffer*/
				vid_clearBuffer( (void*)DataFlagArr,(sizeof(DataFlagArr)/sizeof(DataFlagArr[0])) ,sizeof(DataFlagArr[0]) );
			}
			if(j>0)
			{
				//we stored more than 50 valid data symbol received
				symbolcounter+=50;
				/*Save Last Data Received to handle case if the next check data is repeated
				 * [j-1] because we incremented j above*/
//				local_tempLastDataQ=DataQ[j-1];
				//Reset DataQ index
				j=0;
				//App_voidReadAndReactUpdated(DataQ);
				/*Reset DataQbuffer*/
				vid_clearBuffer((void*)DataQ,(sizeof(DataQ)/sizeof(DataQ[0])) ,sizeof(DataQ[0]));
			}
#endif

		}//if that triggers inner while(1)

	}//main while(1)
	//can't break the main while 1
	asm("nop");
}

