/*
 * adc.h
 *
 *  Created on: Jul 11, 2022
 *      Author: chlsk
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include <stdint.h>

void pa1_adc_init(void);
uint32_t adc_read(void);
void start_conversion(void);
void pa1_adc_interrupt_init(void);

#define SR_EOC				(1U << 1)




#endif /* INC_ADC_H_ */
