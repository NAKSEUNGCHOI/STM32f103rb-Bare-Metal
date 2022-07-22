#include "stm32f103xb.h"

void I2C1_init(void){
	/*1. Enable clock access to I2C1. */
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	/*2. Enable clock access to GPIOB*/
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;

	/*2. Set PB6 and PB7 to alternate functions output open-drain. PB6 = I2C_SCL / PB7 = I2C_SDA */
	GPIOB->CRL |= (1 << 27) | (1 << 26) | (1 << 25);
	GPIOB->CRL &= ~(1 << 24);
	GPIOB->CRL |= (1 << 31) | (1 << 30) | (1 << 29);
	GPIOB->CRL &= ~(1 << 28);

	/*3. set PB6 and PB7 output type to open drain. */

	/*4. Enable pull-up for PB6 and PB7*/


	/*5. Enter reset mode*/
	I2C1->CR1 |= I2C_CR1_SWRST;

	/*6. come out of reset mode (required to reset and come out of it to start the I2C)*/
	I2C1->CR1 &= ~I2C_CR1_SWRST;

	/*7. */
	I2C1->CR2 |= I2C_CR2_FREQ_3;
	/*8. */

	/*9. */

}


