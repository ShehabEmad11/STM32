/***********************************/
/*Author: Shehab emad*/
/*Version 1.0*/
/*Data:8/8/2020*/
/**************************************/
#ifndef RCC_CONFIG_H
#define RCC_CONFIG_H

/*OPTIONS:	
			RCC_HSI						(High Speed Internal)
			RCC_HSE_CRYSTAL    4-16Mhz  (High Speed External Crystal)
		 	RCC_HSE_RC		   25Mhz (bybass)(High Speed External RC)   -Need external clk source-
		 	RCC_PLL 					(Phase Locked Loop)
*/
#define RCC_CLOCK_TYPE 			RCC_HSE_CRYSTAL



/*PLL OPTIONS:  
RCC_PLL_IN_HSI_DIV_2
RCC_PLL_IN_HSE_DIV_2
RCC_PLL_IN_HSE
*//*Note: Select Value only if you have PLL as input clock source*/
#if RCC_CLOCK_TYPE==RCC_PLL

	#define RCC_PLL_INPUT   RCC_PLL_IN_HSE
	

#endif


/*OPTIONS For PLL Multiplication value: x2 to x16
PLL_INPUT_CLK_X2
PLL_INPUT_CLK_X3
		:
		:
PLL_INPUT_CLK_X15
PLL_INPUT_CLK_X16
*/ 

#define RCC_PLL_INPUT_CLK_X2	0b0000

/*Note: Select Value only if you have PLL as input clock source*/
#if RCC_CLOCK_TYPE==RCC_PLL
#define RCC_PLL_MUL_VAL 			(RCC_PLL_INPUT_CLK_X2)


#endif	
#endif
