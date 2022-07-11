/**
 * 	main.c
 * 	@date July-10-2022
 * 	@author Nakseung Choi
 * 	@Brief turning on/off the on-board LED with BSRR.
 * 	STM32F has a on-board button (B1) that is active LOW (It stays HIGH).
 */

#include "stm32f103xb.h"

#define GPIOAEN 			(1U << 2)
#define GPIOCEN				(1U << 4)
#define PIN13				(1U << 13)
#define PIN5				(1U << 5)
#define LED_PIN				PIN5
#define BTN_PIN				PIN13


int main(void){

	/*Enable clock access to GPIOA and GPIOC*/
	RCC->APB2ENR |= GPIOAEN | GPIOCEN;

	/*Set PA5 as an output pin */
	GPIOA->CRL &= 0x00000000;
	GPIOA->CRL |= 0x00200000;

	/*Set PC13 as an input pin */
	GPIOC->CRH &= ~(1U << 21);
	GPIOC->CRH &= ~(1U << 20);

	while(1){
		if(GPIOC->IDR & BTN_PIN){
			GPIOA->BSRR = LED_PIN;
		}else{
			GPIOA->BSRR = (1U << 21);
		}
	}
}
