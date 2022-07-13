/**
 * 	main.c
 * 	@date July-10-2022
 * 	@author Nakseung Choi
 * 	@Brief UART
 * 	Alternate function I/O has to be enabled.
 * 	Fixed Issue: CR2_ADON has to be enabled twice to start. (Once: wakes it up, twice: start of conversion)
 * 	For continuous conversion, both ADON and CONST have to be enabled twice.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include "stm32f103xb.h"
#include "adc.h"
#include "uart.h"
#include "systick.h"

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

	while(1){
		printf("%d. A second passed !! \n\r", (int)count++);
		GPIOA->ODR ^= LED_PIN;
		systickDelayMS(1000);
	}


}


