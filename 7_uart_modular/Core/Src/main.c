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

int main(void){

	uart2_tx_init();

	while(1){
		printf("Hello from STM32F1....\n\r"); // new line courage return
		for(volatile int i = 0; i < 1000000; i++){}
	}

}


