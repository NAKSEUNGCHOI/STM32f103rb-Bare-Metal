/*
 * adc.c
 *
 *  Created on: Jul 11, 2022
 *      Author: chlsk
 */
#include "adc.h"
#include "stm32f103xb.h"

#define ADC1EN				(1U << 9)
#define GPIOAEN				(1U << 2)
#define ADC_CH1				(1U << 0)
#define ADC_SEQ_LEN_1		0x00;
#define CR2_ADON			(1U << 0)
#define CR2_CONT			(1U << 1)
#define CR2_SWSTART			(1U << 22)


void pa1_adc_interrupt_init(void){

	/*1. Enable clock access to GPIOA */
	RCC->APB2ENR |= GPIOAEN;

	/*2. Enable clock access to ADC*/
	RCC->APB2ENR |= ADC1EN;

	/*3. Enable ADC end of conversion interrupt*/
	ADC1->CR1 |= ADC_CR1_EOCIE;

	/*4. Enable ADC interrupt in NVIC*/
	NVIC_EnableIRQ(ADC1_2_IRQn);

	/*5. Conversion sequence start*/
	ADC1->SQR3 |= ADC_SQR3_SQ1_0;

	/*6. Conversion sequence length*/
	ADC1->SQR1 = ADC_SEQ_LEN_1;

	/*7. Enable ADC module*/
	ADC1->CR2 |= CR2_ADON | CR2_CONT;

}

/**
 * ADC configured with 3 channels
 * Ch2, Ch3, Ch5
 * first = ch5
 * second = ch2
 * third ch3
 * This is the order where we want our ADCs to sample the channels.
 * Therefore, you have to put 5 in "binary number" into SQ1, 2 into SQ2, 3into SQ3, and so fourth.
 * In this case, you are going to need to use only one adc, so you put 1 in binary number into SQ1.
 * Then, set the sequence length @ ADC_SQR1.
 * Length of sequence means with how many channels you are sampling.
 */
void pa1_adc_init(void){

	/*1. Enable clock access to GPIOA */
	RCC->APB2ENR |= GPIOAEN;

	/*2. Enable clock access to ADC*/
	RCC->APB2ENR |= ADC1EN;

	/*3. Conversion sequence start*/
	ADC1->SQR3 |= ADC_SQR3_SQ1_0;

	/*4. Conversion sequence length*/
	ADC1->SQR1 = ADC_SEQ_LEN_1;

	/*5. Enable ADC continuous conversion module*/

	ADC1->CR2 |= CR2_ADON | CR2_CONT;

}

void start_conversion(void){
	/*For STM32f103rb, ADON has to be enabled twice to start conversion.*/
	ADC1->CR2 |= CR2_ADON | CR2_CONT;
	/*Start ADC conversion*/
	ADC1->CR2 |= CR2_SWSTART;
}

uint32_t adc_read(void){

	/*Wait for conversion to be complete*/
	while(!(ADC1->SR & SR_EOC)){}
	/*Read converted results*/
	return (ADC1->DR);
}

