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
#include "adc.h"
#include "uart.h"

uint32_t sensor_value = 0;

int main(void){

	pa1_adc_init();
	uart2_tx_init();



	while(1){
		start_conversion();
		sensor_value = adc_read();
		printf("Sensor value: %d \n\r", (int)sensor_value);
	}


}


