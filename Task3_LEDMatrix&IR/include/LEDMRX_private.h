/***********************************/
/*Author: Shehab emad*/
/*Version 1.3*/
/*Data:19/7/2021*/
/**************************************/
 
#ifndef LEDMRX_PRIVATE_H
#define LEDMRX_PRIVATE_H


#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "GPIO_interface.h"
#include "LEDMRX_config.h"



/*These 3 private lines translate the frequnecy to OUTPUT mode and frequency*/
#define FREQ_MAX_2MHZ		OUTPUT_SPEED_2MHZ_PP
#define FREQ_MAX_10MHZ		OUTPUT_SPEED_10MHZ_PP
#define FREQ_MAX_50MHZ		OUTPUT_SPEED_50MHZ_PP

#define ANODE				124
#define CATHODE				125

#if HLEDMRX_ROWPOLARITY==ANODE
#define HLEDMRX_ROWDIMENSION			 HLEDMRX_ANODEDIMENSION
#define ROWPORTS						 ANODEPORTS
#define ROWPINS							 ANODEPINS
#elif HLEDMRX_ROWPOLARITY==CATHODE
#define HLEDMRX_ROWDIMENSION			 HLEDMRX_CATHODEDIMENSION
#define ROWPORTS						 CATHODEPORTS
#define ROWPINS							 CATHODEPINS
#endif

#if HLEDMRX_COLUMNPOLARITY==ANODE
#define HLEDMRX_COLUMNDIMENSION			 HLEDMRX_ANODEDIMENSION
#define COLUMNPORTS						 ANODEPORTS
#define COLUMNPINS						 ANODEPINS
#elif HLEDMRX_COLUMNPOLARITY==CATHODE
#define HLEDMRX_COLUMNDIMENSION			 HLEDMRX_CATHODEDIMENSION
#define COLUMNPORTS						 CATHODEPORTS
#define COLUMNPINS						 CATHODEPINS
#endif


#endif
