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
#define CR2_SWSTART			(1U << 22)
#define SR_EOC				(1U << 1)

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

	/*Enable clock access to GPIOA */
	RCC->APB2ENR |= GPIOAEN;

	/*Set the mode of PA1 to analog mode (CNF1 analog mode: 00, Mode1 Input mode: 00)*/
	GPIOA->CRL &= ~(1U << 4);
	GPIOA->CRL &= ~(1U << 5);
	GPIOA->CRL &= ~(1U << 6);
	GPIOA->CRL &= ~(1U << 7);

	/*******Configure the ADC module*******/
	/*Enable clock access to ADC*/
	RCC->APB2ENR |= ADC1EN;

	/*Configure ADC parameters*/
	/*Conversion sequence start*/
	ADC1->SQR3 = ADC_CH1;

	/*Conversion sequence length*/
	ADC1->SQR1 = ADC_SEQ_LEN_1;


	/*Enable ADC module*/
	ADC1->CR2 |= CR2_ADON;

}

void start_conversion(void){
	/*Start ADC conversion*/
	ADC1->CR2 |= CR2_SWSTART;
}

uint32_t adc_read(void){
	/*Wait for conversion to be complete*/
	while(!(ADC1->SR & SR_EOC)){}
	/*Read converted results*/
	return (ADC1->DR);
}

