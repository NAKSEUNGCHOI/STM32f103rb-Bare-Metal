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

uint32_t sensor_value = 0;

int main(void){

	uart2_tx_init();
	pa1_adc_interrupt_init();
	start_conversion();

	while(1){

	}


}
static void adc_callback(void){
	sensor_value = ADC1->DR;
	printf("Sensor value: %d \n\r", (int)sensor_value);
}

void ADC1_2_IRQHandler(void){
	/*Check for eoc in SR*/
	// If it is not equal to zero, meaning if it is 1 or if it is enabled as 1.
	if((ADC1->SR & SR_EOC) != 0){

		/* Important: Clear EOC*/
		ADC1->SR &= ~SR_EOC;

		//Do something
		adc_callback();
	}
}
