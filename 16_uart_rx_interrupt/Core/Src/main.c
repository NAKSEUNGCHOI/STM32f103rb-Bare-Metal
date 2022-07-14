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

static void uart_callback(void);

char key;

int main(void){

	/*1. Enable clock access to GPIOA*/
	RCC->APB2ENR |= GPIOAEN;

	/*2. Set PA5 as output pin*/
	GPIOA->CRL &= ~(1U << 23);
	GPIOA->CRL &= ~(1U << 22);
	GPIOA->CRL |= (1U << 21);
	GPIOA->CRL &= ~(1U << 20);

	uart2_rx_interrupt_init();


	while(1){

	}


}
static void uart_callback(void){
	key = USART2->DR;
	if(key == '1'){
		GPIOA->ODR |= LED_PIN;
	}else{
		GPIOA->ODR &= ~LED_PIN;
	}
}

void USART2_IRQHandler(void){
	/*1. Check if RXNE is set*/
	if(USART2->SR & SR_RXNE){

		/*2. Do something*/
		uart_callback();
	}
}

