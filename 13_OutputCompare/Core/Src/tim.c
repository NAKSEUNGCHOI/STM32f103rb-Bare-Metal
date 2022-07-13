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

#define GPIOAEN					(1U << 2)
#define PIN0					(1U << 0)
#define LED_PIN					(PIN0)

#define OC_TOGGLE				(1U << 4) | (1U << 5)
#define CCER_CC1E				(1U << 0)

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
void tim2_pa0_output_compare(void){

	/*1. Enable clock acess to GPIOA*/
	RCC->APB2ENR |= GPIOAEN | (1U << 0);
	/*2. Set PA0 mode to alternate function*/
	GPIOA->CRL &= ~(1 << 0);
	GPIOA->CRL |= (1 << 1);
	GPIOA->CRL &= ~(1 << 2);
	GPIOA->CRL |= (1 << 3);
	/*3. Set PA0 alternate function type to TIM2_CH1*/

	/*4. Enable clock access to timer2*/
	RCC->APB1ENR |= TIM2EN;

	/*5. Set a pre-scaler value*/
	TIM2->PSC = 800 - 1; // 8 000 000 / 800 = 10 000

	/*6. Set an auto-reload value*/
	TIM2->ARR = 10000 - 1; // 10 000 / 10 000 = 1

	/*7. Set output compare toggle mode*/
	TIM2->CCMR1 |= OC_TOGGLE;

	/*8. Enable TIM2 CH1 in compare mode*/
	TIM2->CCER |= CCER_CC1E;

	/*8. Clear Counter*/
	TIM2->CNT = 0;

	/*9. Enable timer*/
	TIM2->CR1 = CR1_CEN;


}

