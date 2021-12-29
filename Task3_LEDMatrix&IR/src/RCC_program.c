/***********************************/
/*Author: SHehab emad*/
/*Version 1.0*/
/*Data:8/8/2020*/
/**************************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "RCC_interface.h"
#include "RCC_private.h"
#include "RCC_config.h"


/*Function to Enable Clock of peripherals on buses defined by user*/
void MRCC_voidEnableClock(u8 Copy_u8BusId, u8 Copy_u8PerId)
{
	if(Copy_u8PerId<=31)
	{
		/*Get Bus ID*/
		switch(Copy_u8BusId)
		{
			/*Enable Bit According to the Bus and peripheral given*/
			
			case MRCC_AHB 	: SET_BIT(RCC_AHBENR,Copy_u8PerId); 	break;
			case MRCC_APB1 	: SET_BIT(RCC_APB1ENR,Copy_u8PerId); 	break;
			case MRCC_APB2 	: SET_BIT(RCC_APB2ENR,Copy_u8PerId); 	break;
			//default			:/*return error*/						break;
		}
		
	}
	else
	{
		/*Return Error*/
	}		
}

/*Function to Disable Clock of peripherals on buses defined by user*/
void MRCC_voidDisableClock(u8 Copy_u8BusId, u8 Copy_u8PerId)
{
	if(Copy_u8PerId<=31)
	{
		/*Get Bus ID*/
		switch(Copy_u8BusId)
		{
			/*Disable Bit According to the Bus and peripheral given*/
			
			case MRCC_AHB 	: CLR_BIT(RCC_AHBENR,Copy_u8PerId); 	break;
			case MRCC_APB1 	: CLR_BIT(RCC_APB1ENR,Copy_u8PerId); 	break;
			case MRCC_APB2 	: CLR_BIT(RCC_APB2ENR,Copy_u8PerId); 	break;
			//default			:/*return error*/						break;
		}
		
	}
	else
	{
		/*Return Error*/
	}		
}



void MRCC_voidInitSysClock(void)
{
	/*High Speed Internal_RC (HSI_RC @8Mhz max)*/	
	#if RCC_CLOCK_TYPE == RCC_HSI
		/*Clr bit 0 to select HSI as sys clk ...all prescaler are 00*/
		RCC_CFGR=0x00000000;
		/*Enable HSI by writing 1 in bit_0(HSI) RCC_CR register
		and write 10000 in bits[7:3] for default trimming of HSI clock*/
		RCC_CR=0x00000081;



	/*High Speed External_Crystal (HSE)  @16Mhz max)*/
	#elif(RCC_CLOCK_TYPE == RCC_HSE_CRYSTAL)
		/*Select 01 to select HSE as sys clk ...all prescaler are 00*/
			RCC_CFGR=0x00000001;
		/*Enable HSE by writing 1 in bit_16(HSE) RCC_CR register while bit 18 is 0*/
		RCC_CR=0x00010000;


		
	/*High Speed External_RC (HSE_RC (Bypass)@25Mhz max)*/
	#elif 	(RCC_CLOCK_TYPE == RCC_HSE_RC)
			/*Write 1 to bit 0 to Select HSE as sys clk ...all prescaler are 00*/
			RCC_CFGR=0x00000001;
			/*Enable HSE by writing 1 in bit_16(HSE) And 1 in bit 18(Bypass) in RCC_CR register*/
			RCC_CR=0x00050000;

	

	/*PLL clk source */
	#elif  	 (RCC_CLOCK_TYPE == RCC_PLL)
			/*Caution: The PLL output frequency must not exceed 72 MHz.
			 *Multiplication factor must be selected before enable PLL*/
			 /*Check that PLL_MUL_VAL is in range x2>>x16*/
			#if (RCC_PLL_MUL_VAL>16)
				#error("you chosed wrong PLL_MUL_VAL")

			/*If multiplication factor is not greater than 16 proceed
			 *Select PLL as system clock source by writing 0b10=0x02 in SW-bits[1:0] in RCC_CFGR*/
			/*PLL clk source with HSI/2 as input to PLL*/	
			#elif(RCC_PLL_INPUT==RCC_PLL_IN_HSI_DIV_2)
				/*in RCC_CFGR Write 0 in bit16(PLLSRC) to select HSI/2 as input to PLL
				 *and Write 10 in SW_bits[1:0] to select PLL as system clock source*/
				RCC_CFGR=0x00000002;
				/*Select PLL Mult. Factor BITS[21:18] in RCC_CFGR without edit rest of register*/
				/*Clear PLLMUL bits[21:18]*/
				RCC_CFGR&=~(0b1111<<18);
				/*Write PLL multiplication value on PLLMUL bits[21:18]*/
				RCC_CFGR|=(RCC_PLL_MUL_VAL<<18);
				/*In RCC_CR Write 1 in bit0 to enable HSI Clock
				 * Write 1 in bit-24(PLLON) to Enable PLL */
				RCC_CR=0x01000001;
				
			/*PLL clk source with HSE/2 as input to PLL*/	
			#elif (RCC_PLL_INPUT==RCC_PLL_IN_HSE_DIV_2)
				/*in RCC_CFGR Write 1 in bit16(PLLSRC) to select HSE as input to PLL
				*and Write 1 to Bit-17(PLLXTPRE) to divide HSE/2 clock:
				*and Write 10 in SW_bits[1:0] to select PLL as system clock source*/
				RCC_CFGR=0x00030002;
				/*Select PLL Mult. Factor BITS[21:18] in RCC_CFGR without edit rest of register*/
				/*Clear PLLMUL bits[21:18]*/
				RCC_CFGR&=~(0b1111<<18);
				/*Write PLL multiplication value on PLLMUL bits[21:18]*/
				RCC_CFGR|=(RCC_PLL_MUL_VAL<<18);
				/*In RCC_CR Write Write 1 in Bit-16 to Enable HSE clk
				 *Write 1 in bit-24(PLLON) to Enable PLL*/
				RCC_CR=0x01010000;
		
			/*PLL clk source with HSE as input to PLL*/	
			#elif (RCC_PLL_INPUT==RCC_PLL_IN_HSE)
				/*in RCC_CFGR Write 1 in bit16(PLLSRC) to select HSE/2 as input to PLL
				*and Write 0 to Bit-17(PLLXTPRE) to use HSE clock un-divided:
				*and Write 10 in SW_bits[1:0] to select PLL as system clock source*/
				RCC_CFGR=0x00010002;
				/*Select PLL Mult. Factor BITS[21:18] in RCC_CFGR without edit rest of register*/
				/*Clear PLLMUL bits[21:18]*/
				RCC_CFGR&=~(0b1111<<18);
				/*Write PLL multiplication value on PLLMUL bits[21:18]*/
				RCC_CFGR|=(RCC_PLL_MUL_VAL<<18);
				/*In RCC_CR Write Write 1 in Bit-16 to Enable HSE clk
				 *Write 1 in bit-24(PLLON) to Enable PLL*/
				RCC_CR=0x01010000;
	
			#else
					#error("you chosed wrong PLL_clock type")
			#endif
			
	#else
		#error("you chosed wrong clock type")
	#endif

}
