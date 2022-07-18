/*
 * tim.h
 *
 *  Created on: Jul 12, 2022
 *      Author: chlsk
 */

#ifndef INC_TIM_H_
#define INC_TIM_H_

void tim2_1hz_int(void);

/*Important. Update interrupt flag*/
#define SR_UIF			(1U << 0)
void tim2_1hz_interrupt_init(void);

#endif /* INC_TIM_H_ */
