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

static void dma_callback(void);

int main(void){
	char message[31] = "Hello from Stm32 DMA transfer\n\r";

	/*1. Enable clock access to GPIOA*/
	RCC->APB2ENR |= GPIOAEN;

	/*2. Set PA5 as output pin*/
	GPIOA->CRL &= ~(1U << 23);
	GPIOA->CRL &= ~(1U << 22);
	GPIOA->CRL |= (1U << 21);
	GPIOA->CRL &= ~(1U << 20);

	uart2_tx_init();
	dma1_ch7_init((uint32_t)message, (uint32_t)&USART2->DR, 31);


	while(1){

	}


}
static void dma_callback(void){

		GPIOA->ODR |= LED_PIN;

}

void DMA1_Channel7_IRQHandler(void){
	/*1. Check for transfer complete interrupt*/
	if(DMA1->ISR & DMA_ISR_TCIF7){

		/*2. Clear flag */
		DMA1->IFCR |= DMA_IFCR_CTCIF7;

		/*3. Do something*/
		dma_callback();
	}
}

