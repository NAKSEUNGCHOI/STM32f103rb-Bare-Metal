#include "stm32f103xb.h"
#include "i2c.h"

#define I2C_100KHZ						40 // 0b 0101 0000 = decimal 80
#define SD_MODE_MAX_RISE_TIME			9
#define GPIOAEN 			(1U << 2)
#define PIN5				(1U << 5)
#define LED_PIN				PIN5

void I2C1_init(void){
	RCC->APB1ENR &= ~(RCC_APB1ENR_I2C1EN);
	I2C1->CR1 = 0x00;
	I2C1->CR2 = 0x00;

	/*Testing*/
	RCC->APB2ENR |= GPIOAEN;
	GPIOA->CRL &= 0x00000000;
	GPIOA->CRL |= 0x00200000;
	/////////////////////////

	/*1. Enable clock access to I2C1. */
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	/*2. Enable clock access to GPIOB*/
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;

	/*2. Set PB6 and PB7 to alternate functions output open-drain. PB6 = I2C_SCL / PB7 = I2C_SDA */
	GPIOB->CRL |= (1U << 27) | (1U << 26) | (1U << 25);
	GPIOB->CRL &= ~(1U << 24);
	GPIOB->CRL |= (1U << 31) | (1U << 30) | (1U << 29);
	GPIOB->CRL &= ~(1U << 28);

	/*3. set PB6 and PB7 output type to open drain. */
	// not needed for stm32f1
	/*4. Enable pull-up for PB6 and PB7*/
	// not needed for stm32f1

	/*5. Enter reset mode*/
	I2C1->CR1 |= I2C_CR1_SWRST;

	/*6. come out of reset mode (required to reset and come out of it to start the I2C)*/
	I2C1->CR1 &= ~(I2C_CR1_SWRST);

	/*7. Set peripheral clock frequency */
	I2C1->CR2 = (1U << 3); // I2C_CR2_FREQ_3; // 8 Mhz

	/*8. set i2C to standard mode , 100kHz clock*/
	I2C1->CCR = I2C_100KHZ;

	/*9. Set rise time*/
	I2C1->TRISE = SD_MODE_MAX_RISE_TIME;

	/* Enable peripheral clock */
	I2C1->CR1 |= I2C_CR1_PE;

}

void I2C1_byteRead(char saddr, char maddr, char* data){
	/*slave address, memory address, data where we want to read from */

	volatile int temp;
	/*1. Wait until bus is not busy*/
	while(I2C1->SR2 & I2C_SR2_BUSY){}

	/*2. Generate start condition*/
	I2C1->CR1 |= I2C_CR1_START;

	/*3. wait until start flag is set*/
	while(!(I2C1->SR1 & I2C_SR1_SB)){}

	/*4. transmit slave address + write */
	I2C1->DR = (uint8_t)saddr << 1;

	/*5. Wait until address flag is set*/
	while(!(I2C1->SR1 & (I2C_SR1_ADDR))){}
	GPIOA->ODR = LED_PIN;
	/*6. Clear addr flag*/
	temp = I2C1->SR2 | I2C1->SR1;

	// Done with sending slave address//

	/*7. send memory address*/
	I2C1->DR = maddr;

	/*8. wait until the data register is empty. */
	while(!(I2C1->SR1 & I2C_SR1_TXE)){}

	/*9. Generate restart condition	 */
	I2C1->CR1 |= I2C_CR1_START;

	/*10. Wait until start flag is set*/
	while(!(I2C1->SR1 & I2C_SR1_SB)){}

	/*11. Transmit slave address + read*/
	I2C1->DR = saddr << 1 | 1;

	/*12. wait until addr flag is set*/
	while(!(I2C1->SR1 & I2C_SR1_ADDR)){}

	/*13. Disable acknowledge bit*/
	I2C1->SR1 &= ~(I2C_CR1_ACK);

	/*14. Clear addr flag by reading SR2 again*/
	temp = I2C1->SR2;

	/*15. Enable stop condition */
	I2C1->CR1 |= I2C_CR1_STOP;

	/*16. Wait until RXNE flag is set*/
	while(!(I2C1->SR1 & I2C_SR1_RXNE)){}

	/*17. Read data from DR*/
	*data++ = I2C1->DR;


}
void I2C1_burstRead(char saddr, char maddr, int n, char* data){
	volatile int temp;

	/*1. Wait until bus is not busy*/
	while(I2C1->SR2 & I2C_SR2_BUSY){}

	/*2. Generate start condition*/
	I2C1->CR1 |= I2C_CR1_START;

	/*3. wait until start flag is set*/
	while(!(I2C1->SR1 & I2C_SR1_SB)){}

	/*4. transmit the data into I2C data register*/
	I2C1->DR = saddr << 1;

	/*5. Wait until address flag is set*/
	while(!(I2C1->SR1 & I2C_SR1_ADDR)){}

	/*6. Clear addr flag by reading SR2*/
	temp = I2C1->SR2;

	/*8. wait until the data register is empty. */
	// It is advisable to wait until the data register is empty before sending memory address.
	while(!(I2C1->SR1 & I2C_SR1_TXE)){}

	/*7. send memory address*/
	I2C1->DR = maddr;

	/*8. wait until the data register is empty. */
	while(!(I2C1->SR1 & I2C_SR1_TXE)){}

	/*9. Generate restart condition	 */
	I2C1->CR1 |= I2C_CR1_START;

	/*10. Wait until start flag is set*/
	while(!(I2C1->SR1 & I2C_SR1_SB)){}

	/*11. Transmit slave address + read*/
	I2C1->DR = saddr << 1 | 1;

	/*12. wait until addr flag is set*/
	while(!(I2C1->SR1 & I2C_SR1_ADDR)){}

	/*14. Clear addr flag by reading SR2 again*/
	temp = I2C1->SR2;

	/*13. Enable acknowledge bit*/
	I2C1->SR1 |= I2C_CR1_ACK;

	while(n > 0U){
		/*If n == 1 byte b/c we are decrementing n*/
		if(n == 1U){
			/*13. Disable acknowledge bit*/
			I2C1->SR1 &= ~I2C_CR1_ACK;

			/*15. Generate stop */
			I2C1->CR1 |= I2C_CR1_STOP;

			/*16. Wait until RXNE flag is set*/
			while(!(I2C1->SR1 & I2C_SR1_RXNE)){}

			/*17. Read data from DR*/
			*data++ = I2C1->DR;

			break;
		}else{
			/*16. Wait until RXNE flag is set*/
			while(!(I2C1->SR1 & I2C_SR1_RXNE)){}

			/*17. Read data from DR*/
			*data++ = I2C1->DR;

			n--;
		}
	}

}
void I2C1_burstWrite(char saddr, char maddr, int n, char* data){
	volatile int temp;

	/*1. Wait until bus is not busy*/
	while(I2C1->SR2 & I2C_SR2_BUSY){}

	/*2. Generate start condition*/
	I2C1->CR1 |= I2C_CR1_START;

	/*3. wait until start flag is set*/
	while(!(I2C1->SR1 & I2C_SR1_SB)){}

	/*4. transmit the data into I2C data register*/
	I2C1->DR = saddr << 1;

	/*5. wait until address flag is set*/
	while(!(I2C1->SR1 & I2C_SR1_ADDR)){}

	/*5. Clear addr flag*/
	temp = I2C1->SR2;

	/*6. wait until the data register is empty. */
	while(!(I2C1->SR1 & I2C_SR1_TXE)){}

	/*7. send memory address*/
	I2C1->DR = maddr;
	for(int i = 0; i < n; i++){
		/*8. wait until the data register is empty. */
		while(!(I2C1->SR1 & I2C_SR1_TXE)){}

		/*9. Transmit Memory address */
		I2C1->DR = *data++;
	}
	/*10. wait until transfer finished*/
	while(!(I2C1->SR1 & I2C_SR1_BTF)){}

	/*generate stop condition*/

	I2C1->CR1 |= I2C_CR1_STOP;



}

