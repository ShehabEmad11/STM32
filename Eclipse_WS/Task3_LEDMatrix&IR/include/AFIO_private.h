/***********************************/
/*Author: Shehab emad*/
/*Version 1.0*/
/*Data:22/8/2020*/
/**************************************/

#ifndef AFIO_PRIVATE_H
#define AFIO_PRIVATE_H

#include "STD_TYPES.h"
#include "BIT_MATH.h"

/*Check section 9.4 document 01-STM*/

typedef struct {

	volatile u32 EVCR;
	volatile u32 MAPR;
	volatile u32 EXTICR[4];
	volatile u32 MAPR2;

}AFIO_t;

/*define AFIO as pointer to structure*/
#define AFIO ((volatile AFIO_t *) 0x40010000)


#endif
