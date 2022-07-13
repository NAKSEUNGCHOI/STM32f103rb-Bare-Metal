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




#endif /* INC_ADC_H_ */
