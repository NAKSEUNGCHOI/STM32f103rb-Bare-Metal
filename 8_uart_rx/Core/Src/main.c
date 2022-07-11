/**
 * 	main.c
 * 	@date July-10-2022
 * 	@author Nakseung Choi
 * 	@Brief UART
 * 	Alternate function I/O has to be enabled.
 */

#include <stdio.h>
#include <stdint.h>
#include "stm32f103xb.h"
#include "uart.h"


#define GPIOAEN					(1U << 2)
#define GPIOA_5					(1U << 5)

#define LED_PIN					GPIOA_5

char key;

int main(void){

	/*1. Enable clock access to GPIOA*/
	RCC->APB2ENR |= GPIOAEN;

	/*2. Set PA5 as output pin*/
	/*Note: Be aware that modifying this could generate some issue with CRL defined in uart.c file */
	GPIOA->CRL &= 0x00000000;
	GPIOA->CRL |= 0x00200000;


	uart2_rxtx_init();

	while(1){
		key = uart2_read();
		if(key == '1'){
			GPIOA->ODR |= LED_PIN;
		}else{
			GPIOA->ODR &= ~LED_PIN;
		}
	}


}


