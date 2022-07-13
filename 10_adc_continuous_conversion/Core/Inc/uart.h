/*
 * uart.h
 *
 *  Created on: Jul 11, 2022
 *      Author: chlsk
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include <stdint.h>
#include "stm32f103xb.h"

void uart2_tx_init(void);
void uart2_rxtx_init(void);
char uart2_read(void);

#endif /* INC_UART_H_ */
