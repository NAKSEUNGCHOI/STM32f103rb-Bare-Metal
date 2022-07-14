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



int timestamp = 0;

int main(void){


	//uart2_tx_init();
	tim2_pa0_output_compare();
	tim3_input_capture();

	while(1){

		while(!(TIM3->SR & SR_CC1IF)){}
		timestamp = TIM3->CCR1;
	}


}


