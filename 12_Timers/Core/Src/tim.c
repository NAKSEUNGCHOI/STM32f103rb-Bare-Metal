/*
 * tim.c
 *
 *  Created on: Jul 12, 2022
 *      Author: chlsk
 */
#include "stm32f103xb.h"
#include "tim.h"

#define TIM2EN 					(1U << 0)
#define CR1_CEN					(1U << 0)

void tim2_1hz_int(void){

	/*1. Enable clock access to timer2*/
	RCC->APB1ENR |= TIM2EN;

	/*2. Set a pre-scaler value*/
	TIM2->PSC = 800 - 1; // 8 000 000 / 800 = 10 000

	/*3. Set an auto-reload value*/
	TIM2->ARR = 10000 - 1; // 10 000 / 10 000 = 1

	/*4. Clear Counter*/
	TIM2->CNT = 0;

	/*5. Enable timer*/
	TIM2->CR1 = CR1_CEN;


}

