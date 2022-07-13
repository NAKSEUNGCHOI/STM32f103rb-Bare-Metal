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
	pa1_adc_init();
	start_conversion();

	while(1){
		sensor_value = adc_read();
		printf("Sensor value: %d \n\r", (int)sensor_value);
	}


}


