/***********************************/
/*Author: Shehab emad*/
/*Version 1.0*/
/*Data:12/1/2022*/
/**************************************/
#ifndef TIMER2TO5_CONFIG_H
#define TIMER2TO5_CONFIG_H

#include "STD_TYPES.h"
#include "BIT_MATH.h"


#define TIMR2to5_MAXCONTEXTS	(5)

/*There is some error margin due to use of TIMx periodic ISR
 1- If we set very short Periodic Timer, this will cause overhead delay which is not accounted for.
  *for example if Timer is 10 Ms Period while we set alarm for 10,000,000Ms(10seconds)
 	---->for this alarm the ISR will executed 1,000,000 time, and if we assumed that ISR execution time
   is only 1Ms this will cause overhead delay of 1,000,000Ms (1 second) which is not accounted for
 2- if we set very long periodic TImer, the following will happen:
     2.1)this will cause resolution of 1sec as minimum resolution
     2.2)first ISR of each alarm will miss a portion of its time
   *for example if Timer is 1000,000 Ms(1sec) Period:
     2.1)we will not be able to set any Alarm with value less than 1 sec
     2.2)lets say we started periodic timer of (1s period) at t=0, while we started to set alarm
         at t=400ms	then when first alarm's ISR comes it will be only 600ms passed, which will
         make alarm miss a value of 400ms
 Conclusion: trade off balance must be made when choosing tick value*/
#define TIM2_BASETICK_Ms		(2500ul)
#define TIM3_BASETICK_Ms		(2500ul)
#define TIM4_BASETICK_Ms		(2500ul)
#define TIM5_BASETICK_Ms		(2500ul)

#endif
