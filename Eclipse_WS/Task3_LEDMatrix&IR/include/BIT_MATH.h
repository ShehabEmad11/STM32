/*************************************************************/
/*Author	:Shehab											*/
/*Date		:20/5/2021										*/
/*Version	:V1.1											*/
/************************************************************/

#ifndef BIT_MATH_H
#define BIT_MATH_H

#define SET_BIT(VAR,BIT)    			(VAR |=(1<<(BIT)))
#define CLR_BIT(VAR,BIT)				(VAR &= ~(1<<(BIT)))
#define GET_BIT(VAR,BIT)			    ((VAR>>BIT) & 1)
#define TOG_BIT(VAR,BIT)				(VAR ^=(1<<(BIT)))
#define ASSIGN_BIT(VAR,BIT,VALUE)		(VAR=((VAR&~(1<<BIT))|(VALUE<<BIT)))

#endif