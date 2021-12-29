/***********************************/
/*Author: Shehab emad*/
/*Version 1.0*/
/*Data:19/8/2020*/
/**************************************/

/*The Register defination and addresses of the NVIC are in ARM drivers (programming manual document)
 while the peripheral order is in the ST document (Register Describtion)*/
 
#ifndef NVIC_PRIVATE_H
#define NVIC_PRIVATE_H_H

/*Define NVIC Groups*/
/*HIGH 16 bits=0x05FA    bits 10,9,8= 0b011 and rest Low side is zeros*/
#define GROUP4			0x05FA0300
/*HIGH 16 bits=0x05FA    bits 10,9,8= 0b100 and rest Low side is zeros*/
#define GROUP3			0x05FA0400
/*HIGH 16 bits=0x05FA    bits 10,9,8= 0b101 and rest Low side is zeros*/
#define GROUP2 			0x05FA0500
/*HIGH 16 bits=0x05FA    bits 10,9,8= 0b110 and rest Low side is zeros*/
#define GROUP1			0x05FA0600
/*HIGH 16 bits=0x05FA    bits 10,9,8= 0b111 and rest Low side is zeros*/
#define GROUP0			0x05FA0700




/*0xE000E100   Base Address of NVIC*/

/*The Following Registers are 32Bit Array type accessed
 *Note that NVIC_Regx 's pointer are not dereferenced when defined
 *Each array element is 32 bit corresponding to 32 interrupt
 *Regx[0]--------->from Interrupt0 to int31
 *Regx[1]--------->from Interrupt32 to int 63
 *Regx[2]--------->from Interrupt64 to int 67 and rest are reserved
 *Our MCU only have 59 external interrupt so Regx[2] is not used */

/*Interrupt Set Enable Registers ISERx: NVIC_ISER[0 to 3]*/
#define NVIC_ISER		((volatile u32*) 0xE000E100)
/*Disable External Interrupts Registers ICERx NVIC_ICER[0 to 3]*/
#define NVIC_ICER		((volatile u32*) 0xE000E180)
/* Interrupts Set Pending Registers ISPRx NVIC_ISPR[0 to 3]*/
#define NVIC_ISPR		((volatile u32*) 0xE000E200)
/*Interrupt Clear Pending Register ICPRx NVIC_ISPR[0 to 3]*/
#define NVIC_ICPR		((volatile u32*) 0xE000E280)
/*Interrupt Active Bit Register (read only) IABRx: NVIC_IABR[0 to 3]*/
#define NVIC_IABR		((volatile u32*) 0xE000E300)


/*This register is accessed in 8Bits Array mode
 * note that NVIC_IPR is pointer not dereferenced when defined */
#define NVIC_IPR		((volatile u8*) 0xE000E400)





/*The Following Are Code created in IMT's Session*/
#if 0
/*Enable External Interrupts from 0 to 31 -Interrupt set-enable registers-*/
#define NVIC_ISER0		*((volatile u32*) 0xE000E100)
/*Enable External Interrupts from 32 to 63 -Interrupt set-enable registers-*/
#define NVIC_ISER1		*((volatile u32*) 0xE000E104)

/*Disable External Interrupts from 0 to 31*/
#define NVIC_ICER0		*((volatile u32*) 0xE000E180)
/*Disable External Interrupts from 32 to 63*/
#define NVIC_ICER1		*((volatile u32*) 0xE000E184)

/*Set External Interrupts Pending Flag from 0 to 31*/
#define NVIC_ISPR0		*((volatile u32*) 0xE000E200)
/*Set External Interrupts Pending Flag from 32 to 63*/
#define NVIC_ISPR1		*((volatile u32*) 0xE000E204)

/*Clear External Interrupts Pending Flag from 0 to 31*/
#define NVIC_ICPR0		*((volatile u32*) 0xE000E280)
/*Clear External Interrupts Pending Flag from 32 to 63*/
#define NVIC_ICPR1		*((volatile u32*) 0xE000E284)


/*Interrupt Active Bit Register (read only) from 0 to 31*/
#define NVIC_IABR0		*((volatile u32*) 0xE000E300)
/*Interrupt Active Bit Register (read only) from 32 to 63*/
#define NVIC_IABR1		*((volatile u32*) 0xE000E304)

/*This register is accessed in byte
 * NVIC_IPR is pointer not dereferenced */
#define NVIC_IPR		((volatile u8*) 0xE000E400)





#endif

#endif