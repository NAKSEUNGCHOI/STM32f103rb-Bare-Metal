/*
 * exti.c
 *
 *  Created on: Jul 13, 2022
 *      Author: chlsk
 */

#include "exti.h"

#define GPIOCEN					(1U << 4)
#define AFIOEN					(1U << 0)

void pc13_exti_init(void){

	/*0. Disable global interrupt. (Good practice, wouldn't affect the code.) */
	__disable_irq();

	/*1. Enable clock access to GPIOC. */
	RCC->APB2ENR |= GPIOCEN;

	/*2. Set PC13 as input */
	GPIOC->CRH &= ~(1U << 20);
	GPIOC->CRH &= ~(1U << 21);
	/*2. Enable clock access to SYSCFG. */
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

	/*3. Select PORTC for EXTI13. */
	AFIO->EXTICR[3] |= (1U << 5);

	/*4. Un-mask EXTI13. */
	EXTI->IMR |= EXTI_IMR_MR13;

	/*5. Select a falling edge trigger. */
	EXTI->FTSR |= EXTI_FTSR_TR13;

	/*6. Enable EXTI line in NVIC. */
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	/*7. Enable global interrupts. */
	__enable_irq();

}

