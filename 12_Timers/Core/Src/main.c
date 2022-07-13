/**
 * 	main.c
 * 	@date July-12-2022
 * 	@author Nakseung Choi
 * 	@Brief Hardware timer
 *
 */

#include <stdio.h>
#include <stdint.h>
#include "stm32f103xb.h"
#include "adc.h"
#include "uart.h"
#include "systick.h"
#include "tim.h"

#define GPIOAEN				(1U << 2)
#define PIN5				(1U << 5)
#define LED_PIN				PIN5

uint32_t count = 0;

int main(void){

	/*1. Enable clock access to GPIOA*/
	RCC->APB2ENR |= GPIOAEN;

	/*2. Set PA5 as output pin*/
	GPIOA->CRL &= ~(1U << 23);
	GPIOA->CRL &= ~(1U << 22);
	GPIOA->CRL |= (1U << 21);
	GPIOA->CRL &= ~(1U << 20);

	uart2_tx_init();
	tim2_1hz_int();

	while(1){

		/*Wait for UIF*/
		while(!(TIM2->SR & SR_UIF)){} // when this is false or not set, we perform "wait"

		/*Clear UIF*/
		TIM2->SR &= ~SR_UIF;
		printf("%d. A second passed !! \n\r", (int)count++);
		GPIOA->ODR ^= LED_PIN;

	}


}


