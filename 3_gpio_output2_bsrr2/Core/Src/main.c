/**
 * 	main.c
 * 	@date July-10-2022
 * 	@author Nakseung Choi
 * 	@Brief turning on/off the on-board LED with BSRR.
 */

#include "stm32f103xb.h"

#define GPIOAEN 			(1U << 2)
#define PIN5				(1U << 5)
#define LED_PIN				PIN5


int main(void){
	RCC->APB2ENR |= GPIOAEN;
	GPIOA->CRL &= 0x00000000;
	GPIOA->CRL |= 0x00200000;

	while(1){
		GPIOA->BSRR = LED_PIN;
		for(volatile int i = 0; i < 100000; i++){}
		GPIOA->BSRR = (1U << 21);
		for(volatile int i = 0; i < 100000; i++){}
	}
}
