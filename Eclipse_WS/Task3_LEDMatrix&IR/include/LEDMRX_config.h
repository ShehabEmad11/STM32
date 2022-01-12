/***********************************/
/*Author: Shehab emad*/
/*Version 1.3*/
/*Data:19/7/2021*/

/*Changed PinA4 with PINA9 A9 not working*/
/**************************************/

#ifndef LEDMRX_CONFIG_H
#define	LEDMRX_CONFIG_H

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "GPIO_interface.h"



#define HLEDMRX_ANODEDIMENSION			(8)
#define ANODEPORTS				GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOA,GPIOA
#define ANODEPINS				PIN0,PIN1,PIN2,PIN3,PIN9,PIN5,PIN6,PIN7

#define HLEDMRX_CATHODEDIMENSION		(8)
#define CATHODEPORTS			GPIOB,GPIOB,GPIOB,GPIOB,GPIOB,GPIOB,GPIOB,GPIOB
#define CATHODEPINS				PIN0,PIN5,PIN6,PIN7,PIN8,PIN9,PIN10,PIN11

/*Anode and cathode are defined in LEDMRX_private.h*/
#define HLEDMRX_ROWPOLARITY					ANODE
#define HLEDMRX_COLUMNPOLARITY				CATHODE

/*define the max frequency of the 8 LEDs connected to the Kit
	OPTIONs: 1-FREQ_MAX_2MHZ	
			 2-FREQ_MAX_10MHZ	
			 3-FREQ_MAX_50MHZ
			 
FREQ_MAX_xMHZ defined in private.h to be translated into GPIO Frequencies
*/
#define HLEDMRX_MAX_FREQUENCY				FREQ_MAX_2MHZ


#define HLEDMRX_MIN_DISPLAY_TIME			(24000ul) //8*3ms


#if 0
#define HLEDMRX_ANODE0				GPIOA,PIN0
#define HLEDMRX_ANODE1				GPIOA,PIN1
#define HLEDMRX_ANODE2				GPIOA,PIN2
#define HLEDMRX_ANODE3				GPIOA,PIN3
#define HLEDMRX_ANODE4				GPIOA,PIN4
#define HLEDMRX_ANODE5				GPIOA,PIN5
#define HLEDMRX_ANODE6				GPIOA,PIN6
#define HLEDMRX_ANODE7				GPIOA,PIN7
		
		
#define HLEDMRX_CATHODE0			GPIOB,PIN0
#define HLEDMRX_CATHODE1			GPIOB,PIN1
#define HLEDMRX_CATHODE2			GPIOB,PIN2
#define HLEDMRX_CATHODE3			GPIOB,PIN3
#define HLEDMRX_CATHODE4			GPIOB,PIN4
#define HLEDMRX_CATHODE5			GPIOB,PIN5
#define HLEDMRX_CATHODE6			GPIOB,PIN6
#define HLEDMRX_CATHODE7			GPIOB,PIN7




#endif




#endif
