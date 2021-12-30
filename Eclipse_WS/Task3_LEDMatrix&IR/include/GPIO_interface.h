/***********************************/
/*Author: Shehab emad*/
/*Version 1.2*/
/*Data:6/5/2021*/

/*Added PULL UP vs Pull DOwn to PIN defination modes and to PIN Direction function and to private definations*/


/**************************************/

#ifndef GPIO_INTERFACE_H
#define GPIO_INTERFACE_H

#include "STD_TYPES.h"
#include "BIT_MATH.h"


#define GPIO_HIGH 1
#define GPIO_LOW 0




enum {GPIOA,GPIOB,GPIOC};

#define PIN0	0
#define PIN1	1
#define PIN2	2
#define PIN3	3
#define PIN4	4
#define PIN5	5
#define PIN6	6
#define PIN7	7
#define PIN8	8
#define PIN9	9
#define PIN10	10
#define PIN11	11
#define PIN12	12
#define PIN13	13
#define PIN14	14
#define PIN15	15
#define PIN16	16
#define PIN17	17
#define PIN18	18
#define PIN19	19
#define PIN20	20
#define PIN21	21
#define PIN22	22
#define PIN23	23
#define PIN24	24	
#define PIN25	25	
#define PIN26	26
#define PIN27	27
#define PIN28	28
#define PIN29	29
#define PIN30	30
#define PIN31	31

/*GPIO PINs' MODES   See page 161 datasheet STM32*/

#define		OUTPUT_SPEED_10MHZ_PP		0b0001
#define		OUTPUT_SPEED_10MHZ_OD		0b0101
#define		OUTPUT_SPEED_10MHZ_AFPP		0b1001
#define		OUTPUT_SPEED_10MHZ_AFOD		0b1101


#define		OUTPUT_SPEED_2MHZ_PP		0b0010
#define		OUTPUT_SPEED_2MHZ_OD		0b0110
#define		OUTPUT_SPEED_2MHZ_AFPP		0b1010
#define		OUTPUT_SPEED_2MHZ_AFOD		0b1110


#define		OUTPUT_SPEED_50MHZ_PP		0b0011
#define		OUTPUT_SPEED_50MHZ_OD		0b0111
#define		OUTPUT_SPEED_50MHZ_AFPP		0b1011
#define		OUTPUT_SPEED_50MHZ_AFOD		0b1111

#define 	INPUT_ANALOG				0b0100
#define 	INPUT_FLOATING				0b0100
/*This will be defined in private as the user will state specific PULLuP or DOWN not general*/
//#define 	INPUT_PULL_UP_DOWN			0b1000


/*User Defined to distinguish between them*/
#define 	INPUT_PULL_UP				0b101000
#define 	INPUT_PULL_DOWN				0b111000


extern void MGPIO_voidSetPinValue(volatile u8 copy_u8PORT , volatile u8 copy_u8PIN , volatile u8 copy_u8Value);
extern u8 MGPIO_u8GetPinValue(volatile u8 copy_u8PORT , volatile u8 copy_u8PIN);
extern void MGPIO_voidSetPinDirection(volatile u8 copy_u8PORT , volatile u8 copy_u8PIN , u8 copy_u8Mode);
extern void MGPIO_voidTogglePin(volatile u8 copy_u8PORT , volatile u8 copy_u8PIN);

extern void MGPIO_voidWritePortValue(volatile u8 copy_u8PORT ,volatile u16 copy_u16Value);
extern void MGPIO_voidLockPinConfig(volatile u8 copy_u8PORT ,u16 copy_u16SelectedPINsCode);
extern u8 MGPIO_u8GetPinConfigLockStatus(volatile u8 copy_u8PORT ,volatile u8 copy_u8PIN);
extern u8 MGPIO_u8GetPortKeyLockStatus(volatile u8 copy_u8PORT);



#endif
