/**
 * 	main.c
 * 	@date July-12-2022
 * 	@author Nakseung Choi
 * 	@Brief Hardware timer toggle mode.
 * 	External LED turns on and off every second.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include "stm32f103xb.h"
#include "adc.h"
#include "uart.h"
#include "systick.h"
#include "tim.h"
#include "exti.h"

#define GPIOAEN				(1U << 2)
#define PIN5				(1U << 5)
#define LED_PIN				PIN5

static void exti_callback(void);

int main(void){

	/*1. Enable clock access to GPIOA*/
	RCC->APB2ENR |= GPIOAEN;

	/*2. Set PA5 as output pin*/
	GPIOA->CRL &= ~(1U << 23);
	GPIOA->CRL &= ~(1U << 22);
	GPIOA->CRL |= (1U << 21);
	GPIOA->CRL &= ~(1U << 20);

	pc13_exti_init();
	uart2_tx_init();


	while(1){

	}


}
static void exti_callback(void){
	printf("BTN Pressed...\n\r");
	GPIOA->ODR ^= LED_PIN;
}

void EXTI15_10_IRQHandler(void){
	if((EXTI->PR & LINE13) != 0){
		/*1. Clear PR flag. */
		EXTI->PR |= LINE13;
		/*2. Do something. */
		exti_callback();
	}
}

