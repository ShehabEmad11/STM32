/***********************************/
/*Author: SHehab emad*/
/*Version 1.0*/
/*Data:8/8/2020*/
/**************************************/
#ifndef RCC_PRIVATE_H
#define RCC_PRIVATE_H


/*Register Definations*/

/*CR register enable/disable and calibrate all clock systems*/
#define RCC_CR				*((volatile u32 *)0x40021000)   //clk
/*CFGR Register Choose which one of the enabled clocks is used by the CPU*/
#define RCC_CFGR			*((volatile u32 *)0x40021004)   //clk
#define RCC_CIR				*((volatile u32 *)0x40021008)
#define RCC_APB2RSTR		*((volatile u32 *)0x4002100C)
#define RCC_APB1RSTR		*((volatile u32 *)0x40021010)
#define RCC_AHBENR			*((volatile u32 *)0x40021014)   //clk
#define RCC_APB2ENR			*((volatile u32 *)0x40021018)	//clk
#define RCC_APB1ENR			*((volatile u32 *)0x4002101C)	//clk
#define RCC_BDCR			*((volatile u32 *)0x40021020)
#define RCC_CSR				*((volatile u32 *)0x40021024)


/* Clock TYPES*/
#define RCC_HSI				0
#define RCC_HSE_CRYSTAL     1
#define RCC_HSE_RC			2
#define RCC_PLL				3


/*PLL Input OPTIONS*/
#define RCC_PLL_IN_HSI_DIV_2	4
#define RCC_PLL_IN_HSE_DIV_2	5
#define RCC_PLL_IN_HSE			6

/*PLL MUlTIPLICATION FACTOR*/
#define RCC_PLL_INPUT_CLK_X2	0b0000
#define RCC_PLL_INPUT_CLK_X3	0b0001
#define RCC_PLL_INPUT_CLK_X4	0b0010
#define RCC_PLL_INPUT_CLK_X5	0b0011
#define RCC_PLL_INPUT_CLK_X6	0b0100
#define RCC_PLL_INPUT_CLK_X7	0b0101
#define RCC_PLL_INPUT_CLK_X8	0b0110
#define RCC_PLL_INPUT_CLK_X9	0b0111
#define RCC_PLL_INPUT_CLK_X10	0b1000
#define RCC_PLL_INPUT_CLK_X11	0b1001
#define RCC_PLL_INPUT_CLK_X12	0b1010
#define RCC_PLL_INPUT_CLK_X13	0b1011
#define RCC_PLL_INPUT_CLK_X14	0b1100
#define RCC_PLL_INPUT_CLK_X15	0b1101
#define RCC_PLL_INPUT_CLK_X16	0b1110


#endif
