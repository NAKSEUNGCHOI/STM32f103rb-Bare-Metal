/*
 * exti.h
 *
 *  Created on: Jul 13, 2022
 *      Author: chlsk
 */

#ifndef INC_EXTI_H_
#define INC_EXTI_H_

#include "stm32f103xb.h"
void pc13_exti_init(void);

#define LINE13			(1U << 13)

#endif /* INC_EXTI_H_ */
