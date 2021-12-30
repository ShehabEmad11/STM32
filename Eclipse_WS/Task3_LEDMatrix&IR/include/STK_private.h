/*****************************************/
/* Author  :  Ahmed Assaf & Shehab Emad  */
/* Version :  Ver1.1                     */
/* Date    : 18/5/2021             		 */
/*****************************************/
#ifndef STK_PRIVATE_H
#define STK_PRIVATE_H

typedef struct
{
	volatile u32 CTRL  ;

	/*The LOAD register specifies the start value to load into the VAL register when the counter
	 *is enabled and when it reaches 0.*/
	volatile u32 LOAD  ;

	/*The VAL register contains the current value of the SysTick counter.
     *Reads Val register returns the current value of the SysTick counter.
     *A write of any value clears the field to 0, and also clears the COUNTFLAG bit in the
     *STK_CTRL register to 0.*/
	volatile u32 VAL   ;
}MSTK_Type;

/*MSTK is replaced by (casting of) Pointer to structure*/
#define     MSTK    ((MSTK_Type*)0xE000E010)


#define     MSTK_SRC_AHB           0
#define     MSTK_SRC_AHB_8         1

#define     MSTK_SINGLE_INTERVAL    0
#define     MSTK_PERIOD_INTERVAL    1


#endif
