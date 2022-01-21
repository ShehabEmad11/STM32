/*Author:Shehab Emad
 *Data: 12-5-2021
 *Ver:	1.1 */


#ifndef IR_CONFIG_H
#define	IR_CONFIG_H

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "GPIO_interface.h"

/*the maximum Microseconds that if no interrupt received again after it then frame is not correct
 * it takes 108ms to repeat frame from data sheet so 115ms is good time*/

#define IR_MAXTICKSTOOUT		(115000ul)

/*The Maximum number of signal times stored*/
#define IR_MAXSIGNALBUFFER				(40u)
#define IR_FRAMEBITLENGTH				(33u)
#define IR_REPEATEDFRAMEBITLENGTH		(2u)


#define IR_STARTBIT_LOWER				(13000u)
#define IR_STARTBIT_HIGHER				(14000u)
#define IR_SIGNAL0_LOWER 				(1000u)
#define IR_SIGNAL0_HIGHER  				(1300u)
#define IR_SIGNAL1_LOWER  				(2000u)
#define IR_SIGNAL1_HIGHER  				(2500u)
#define IR_SIGN_REPEAT_FIRST_LOWER0		(40000u)
#define IR_SIGN_REPEAT_FIRST_HIGHER0	(43000u)
#define IR_SIGN_REPEAT_NONFIRST_LOWER0	(96500ul)
#define IR_SIGN_REPEAT_NONFIRST_HIGHER0	(97500ul)
#define IR_SIGN_REPEAT_LOWER1			(11000u)
#define IR_SIGN_REPEAT_HIGHER1			(11400u)

#endif
