#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "AFIO_interface.h"
#include "AFIO_config.h"
#include "AFIO_private.h"


extern void MAFIO_voidSetEXTIConfig(u8 Copy_u8Line,u8 Copy_u8Port)
{
	/*Refer to page 191 in data sheet*/
	/*Their is 	16 EXTI_lines 0-->15 divided on 4 AFIO_EXTICRx[0-3] Registers 4 bits for each line*/
	/*from AFIO_EXTICRx[0-3] for every EXTI_Line you can choose which port control this line:
	 *for PORTA->Write 0b0000
	 *for PORTB->Write 0b0001
	 *for PORTC->Write 0b0010
	 *--not on our MCU--*
	 *for  PORTG->Write 0b0110*/
	 
	 /*For Example if you chose EXTI_5 and wrote 0001 in it the PB5 is the line on which EXTi_5 delivered*/
	 
	 /*The function takes the Line to be activated and the PORT */
	
	/*Make Sure User Selected PORTA,PORTB,PORTC and the EXTI_Line is from 0 to 15*/ 
	if(Copy_u8Port>2 || Copy_u8Line>15)
	{
		/*Error*/
		return;
	}
	/*Make sure PORTC is only selected for LINES 13,14,15 "PORTC have only PINS 13,14,15 on STM32" */
	if(  (Copy_u8Port==GPIOC) && (Copy_u8Line < 13) )
	{
		/*Error*/
		return;
	}
																				
	/*Variable to decide which register we are on to activate the coresponding lines*/
	/*four lines on each of AFIO_EXTICRx[0-3] Register so get the corresponding Register by dividing/4 */
	u8 Local_u8RegIndex=Copy_u8Line/4;

	/*Variable to decide the shift position of the 4 bits that choose the port control the EXTI_Line*/
	/*Get the Position of the LSB bit for the 4 choosing Bits in the AFIO_EXTICRx[0-3] Register*/
	u8 Local_u8LineShiftPos=((Copy_u8Line%4) * 4) ;

	/*Clear the coresponding bits in the coresponding Line in EXTICR Register*/
	AFIO->EXTICR[Local_u8RegIndex] &= ~((0b1111) << Local_u8LineShiftPos);
	/*Write  the Corresponding bits with the required PORT*/
	AFIO->EXTICR[Local_u8RegIndex] |=((Copy_u8Port) << Local_u8LineShiftPos);
	
#if 0
	/*Check if you are in Register 0 or Register 1*/ 
	if(Copy_u8Line <= 3)
	{
		u8 Local_u8RegIndex=0;
	}
	
	else if(Copy_u8Line <= 7)
	{
		Copy_u8Line-= 4;
		u8 Local_u8RegIndex=1;
	}
	
	else if(Copy_u8Line <= 11)
	{
		Copy_u8Line-= 8;
		u8 Local_u8RegIndex=2;
	}
	
	else if(Copy_u8Line <= 15)
	{
		Copy_u8Line-= 12;
		u8 Local_u8RegIndex=3;
	}
	
	/*Clear the coresponding bits in the coresponding Line in EXTICR Register*/
	AFIO->EXTICR[Local_u8RegIndex] &= ~((0b1111) << (Copy_u8Line*4));
	/*Write  the Corresponding bits with the required PORT*/
	AFIO->EXTICR[Local_u8RegIndex] |=((Copy_u8Port) << (Copy_u8Line*4));
#endif
}
