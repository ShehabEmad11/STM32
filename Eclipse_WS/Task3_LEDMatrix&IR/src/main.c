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


#define NORMALMODE  (1)
#define TESTMODE    (2)

#define MODE (NORMALMODE)


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


static void App_voidReadAndReactUpdated(u8* DataQ)
{
	u8 i;
//	HLEDMRX_voidDisplay(LEDMRX_O, 20000);
//	return;


	for(i=0;i<50;i++)
	{
		if(DataQ[i] != 0)
		{
			//EXTI_voidMaskLine(MEXTI_1);

			if(DataQ[i]==HIR_POWER)
				HLEDMRX_voidDisplay(LEDMRX_P, 20000);
			else if(DataQ[i]==HIR_MODE)
				HLEDMRX_voidDisplay(LEDMRX_M, 20000);

			DataQ[i]=0;

			//EXTI_voidUNMaskLine(MEXTI_1);
		}
	}


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


void main()
{
	//This array to be removed after debugging and check on local var no need to array
	u8 volatile DataFlagArr[50]={0};
	u8 AddresQ,DataQ[50]={0},local_tempLastDataQ=0;
	u8 j=0,i=0;

	volatile u32 local_u32RelativeSignalsCount=0;
	u32 localu32TicksCount=0;
	volatile u32 possibleframescounter=0,symbolcounter=0;

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




	while(1)
	{
		MTIMR2to5_voidSetBusyWait(2,10000);
				localu32TicksCount+=10000;
		if(localu32TicksCount>=70000)
		{
			localu32TicksCount=0;
			HIR_voidCalculateUnReadSignalsCount((u32*)&local_u32RelativeSignalsCount);
			if(local_u32RelativeSignalsCount>=33)
			{
				HIR_voidSyncUnreadSignalsCount();

#if MODE==TESTMODE
		DataFlagArr[i]=HIR_u8ExtractDataFromBuffer(testbuff, &AddresQ, &DataQ[j]);
#else
		DataFlagArr[i]=HIR_u8ExtractDataFromBuffer(global_u32IRArrSignalTime, &AddresQ, &DataQ[j]);
#endif
		if(DataFlagArr[i]==IR_DATA_NO_VALID_DATA)
		{
			/*Don't increment index of DataQ as it hasn't been updated (because of invalid data)*/
			i++;
//			break;
		}
		else if(DataFlagArr[i]==IR_DATA_EXTRACTED_EMPTYBUF)
		{
			/*Save Last Received Data*/
			local_tempLastDataQ=DataQ[j];
			/*Break loop no data left*/
			i++;
			j++;
	//		break;
		}
		else if(DataFlagArr[i]==IR_DATA_REPEATEXTRACTED_EMPTYBUF)
		{
			/*Handle following cases:
			 * 1-Case where last data received is still in buffer
			 * 2-Case where we reseted DataQ and we received repeated signal
			   (i.e First new data equals last data received before clearing buffer)*/
			DataQ[j]=local_tempLastDataQ;
			/*Break loop no data left*/
			i++;
			j++;
	//		break;
		}
		else if(DataFlagArr[i]==IR_DATA_REPEATEXTRACTED_PARTIALBUF)
		{
			do
			{
				/*Handle following cases:
				 * 1-Case where last data received is still in buffer
				 * 2-Case where we reseted DataQ and we received repeated signal
				   (i.e First new data equals last data received before clearing buffer)*/
				DataQ[j]=local_tempLastDataQ;
				/*Don't Break loop keep getting data*/
				i++;
				j++;
				//TODO: Should check i,j boundaries here
				App_voidReadAndReactUpdated(DataQ);
				DataFlagArr[i]=HIR_u8ExtractDataFromBuffer(global_u32IRArrSignalTime, &AddresQ, &DataQ[j]);
			}while(DataFlagArr[i]==IR_DATA_REPEATEXTRACTED_PARTIALBUF);
		}
		else if(DataFlagArr[i]==IR_DATA_EXTRACTED_PARTIALBUF)
		{
			do
			{
				/*Save Last Received Data*/
				local_tempLastDataQ=DataQ[j];
				/*Don't Break loop keep getting data*/
				i++;
				j++;
				//TODO: Should check i,j boundaries here
				App_voidReadAndReactUpdated(DataQ);
				DataFlagArr[i]=HIR_u8ExtractDataFromBuffer(global_u32IRArrSignalTime, &AddresQ, &DataQ[j]);
			}while(DataFlagArr[i]==IR_DATA_EXTRACTED_PARTIALBUF);
		}
		else if(DataFlagArr[i]==IR_LOGICERROR || DataFlagArr[i]==IR_IMPOSSIBLETRET) //|| DataFlagArr[i]==IR_DATA_NON_RECEIVED)
			asm("nop");  //error


		App_voidReadAndReactUpdated(DataQ);

		if(i>49)
		{
			//we received more than 50 possible frames
			possibleframescounter+=50;
			//Reset buffer index
			i=0;
			/*Reset Buffer*/
			vid_clearBuffer((void*)DataFlagArr,(sizeof(DataFlagArr)/sizeof(DataFlagArr[0])) ,sizeof(DataFlagArr[0]) );
		}
		if(j>49)
		{
			//we stored more than 50 valid data symbol received
			symbolcounter+=50;
			/*Save Last Data Received to handle case if the next check data is repeated
			 * [j-1] because we incremented j above*/
			local_tempLastDataQ=DataQ[j-1];
			//Reset DataQ index
			j=0;
			/*Reset DataQbuffer*/
			vid_clearBuffer((void*)DataQ, (sizeof(DataQ)/sizeof(DataQ[0])) ,sizeof(DataQ[0]) );
		}
	}//if relative signals
		}//if periodiccheck
	}//while

}

