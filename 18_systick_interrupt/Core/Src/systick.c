/*
 * systick.c
 *
 *  Created on: Jul 12, 2022
 *      Author: chlsk
 */
#include "stm32f103xb.h"

#define SYSTICK_LOAD_VAL					8000		//8000 cycle per milli-seconds
#define CTRL_ENABLE							(1U << 0)
#define CTRL_CLKSRC							(1U << 2)
#define CTRL_COUNTFLAG						(1U << 16)

#define ONE_SEC_LOAD						8000000
#define CTRL_TICKINT						(1U << 1)

void systickDelayMS(int delay){

	/*1. Configure systick*/
	/*Reload with number of clocks per milli-seconds*/
	SysTick->LOAD = SYSTICK_LOAD_VAL;

	/*2. Clear systick current value register */
	SysTick->VAL = 0;
	/*3. Eanble systick and select internal clk src*/
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;

	for(int i = 0; i < delay; i++){

		/*4. Wait till the COUNTFLAG is set.*/
		while((SysTick->CTRL & CTRL_COUNTFLAG) == 0){}

	}


	SysTick->CTRL = 0;

}

void systick_1hz_interrupt(void){

	/*1. Reload with number of clocks per milli-seconds*/
	SysTick->LOAD = ONE_SEC_LOAD - 1;

	/*2. Clear systick current value register */
	SysTick->VAL = 0;

	/*3. Eanble systick and select internal clk src*/
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;

	/*4. Enable systick interrupt*/
	SysTick->CTRL |= CTRL_TICKINT;


}

