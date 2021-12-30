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

extern volatile u8 global_u8IRFrameReceivedFlag,global_u8OverFlowFlag;
extern volatile u32 global_u32IRArrSignalTime[IR_MAXSIGNALBUFFER];

extern volatile ir_type_index global_irDataCounter,global_irDataCounterRelative;


volatile u8 ToBeRepresented[40]={0};
u8 k=0;

#define NORMALMODE  (1)
#define TESTMODE    (2)
//#undef  TESTMODE

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
	u8 AddresQ,DataQ[50]={0};
	u8 j=0,i=0;


	volatile u32 possibleframescounter=0,symbolcounter=0,overflowcounter=0;

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



#if MODE==TESTMODE
	global_u8IRFrameReceivedFlag=1;
#endif
	while(1)
	{
		if(global_u8IRFrameReceivedFlag)
		{
			while(1)
			{
			#if MODE==TESTMODE
				DataFlagArr[i]=HIR_u8ExtractDataFromBuffer(testbuff, &AddresQ, &DataQ[j]);
			#else
				DataFlagArr[i]=HIR_u8ExtractDataFromBuffer(global_u32IRArrSignalTime, &AddresQ, &DataQ[j]);
			#endif
#if 0
				/*Disabled as no need to know if overflow occurs
				 * because if overflow occurs the buffer overwrite itself from beginning
				 */
				if(DataFlagArr[i]==IR_DATA_OVERFLOWBUF )
				{
					/*Data Overflow occurred after receiving some data -as global_u8IRFrameReceivedFlag is only raised if no overflow occurs or it was raised and overflow occurred after that before clearing global_u8IRFrameReceivedFlag  */
					overflowcounter ++;
					global_irDataCounter=0;
					global_irDataCounterRelative=0;
					vid_clearBuffer((void*)global_u32IRArrSignalTime,sizeof(global_u32IRArrSignalTime)/sizeof(global_u32IRArrSignalTime[0]) ,sizeof(global_u32IRArrSignalTime[0]));
					global_u8OverFlowFlag=0;
					i++;
					/*Clear global_u8IRFrameReceivedFlag*/
					global_u8IRFrameReceivedFlag=0;
					break;
				}
#endif
				if(DataFlagArr[i]==IR_DATA_NO_VALID_DATA)
				{
					/*Don't increment index of DataQ as it hasn't been updated (because of invalid data)*/
					i++;
					break;
				}
				else if(DataFlagArr[i]==IR_DATA_EXTRACTED_EMPTYBUF)
				{
					App_voidReadAndReact(DataQ, j);
					i++;
					j++;
					break;
				}
				else if(DataFlagArr[i]==IR_DATA_REPEATEXTRACTED_EMPTYBUF)
				{
					if(j>0)
					{
						DataQ[j]=DataQ[j-1];
					}
					App_voidReadAndReact(DataQ, j);
					i++;
					j++;
					break;
				}
				else if(DataFlagArr[i]==IR_DATA_REPEATEXTRACTED_PARTIALBUF)
				{
					if(j>0)
					{
						DataQ[j]=DataQ[j-1];
						App_voidReadAndReact(DataQ, j);
						i++;
						j++;
					}
				}
				else if(DataFlagArr[i]==IR_DATA_EXTRACTED_PARTIALBUF)
				{
					App_voidReadAndReact(DataQ, j);
					i++;
					j++;
				}
				else if(DataFlagArr[i]==IR_LOGICERROR || DataFlagArr[i]==IR_IMPOSSIBLETRET || DataFlagArr[i]==IR_DATA_NON_RECEIVED)
					asm("nop");


				if(global_u8IRFrameReceivedFlag==0)
				{
					/*If Flag was Cleared after this means that flag was raised and we checked/extracted all buffer then we cleared flag*/
					/*Impossible to get here as flag is cleaared if we return emptybuf which is followed by break above*/
					/*Make Your Routine HeRE*/
					asm("nop");
					break;
				}

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
					//Reset DataQ index
					j=0;
					/*Reset DataQbuffer*/
					vid_clearBuffer((void*)DataQ, (sizeof(DataQ)/sizeof(DataQ[0])) ,sizeof(DataQ[0]) );
				}
			}
		}


#if MODE==TESTMODE

	if(k==2 && global_u8IRFrameReceivedFlag==0)
	{
		//for(i=0;i<IR_MAXSIGNALBUFFER;i++)
			//testbuff[i]=testTargetBuff[i];

		global_u8IRFrameReceivedFlag=1;
	}
#endif
		if(i>49)
		{
			//we received more than 50 possible frames
			possibleframescounter+=50;
			//Reset buffer index
			i=0;
			/*Reset Buffer*/
			vid_clearBuffer( (void*)DataFlagArr,(sizeof(DataFlagArr)/sizeof(DataFlagArr[0])) ,sizeof(DataFlagArr[0]) );
		}
		if(j>49)
		{
			//we stored more than 50 valid data symbol received
			symbolcounter+=50;
			//Reset DataQ index
			j=0;
			/*Reset DataQbuffer*/
			vid_clearBuffer((void*)DataQ,(sizeof(DataQ)/sizeof(DataQ[0])) ,sizeof(DataQ[0]));
		}
	}

	//can't break the main while 1
	asm("nop");
}

