/**
 * 	main.c
 * 	@date July-10-2022
 * 	@author Nakseung Choi
 * 	@brief bare metal programming using structure
 * 	@Note
 * 	APB: Advanced Peripheral Bus
 * 		- minimum of 2 clock cycles access to peripherals
 *
 * 	AHB: Advanced High-performance Bus
 * 		- 1 clock cycle access to peripherals
 *
 * 	Direction Register
 * 		- Set pin as input or output
 *
 * 	Data Register
 * 		- Write to pin or read from pin.
 *
 * 	Three clock sources:
 *
 * 	On-Chip RC Oscillator
 * 		- Least precise
 *
 * 	Externally Connected Crystal
 * 		- Most precise
 *
 * 	Phase Locked Loop (PLL)
 * 		- Programmable
 *
 * 	1. where is the LED connected?
 * 	2. PA5 (pin21)
 * 	3. Search Memory Map in datasheet
 * 	4. register boundary addresses
 * 	5. 0x4000 0000 + 0x0001 0800 = 0x4001 0800
 * 	6. It's 32-bit so 0x0000 0000. each 0 is 4 bit. 4*8 is 32-bit
 * 	mode register
 */

#include <stdint.h>
#define PERIPH_BASE 		(0x40000000UL)
#define APB2PERIPH_OFFSET 	(0x00010800UL)
#define APB2PERIPH_BASE		(PERIPH_BASE + APB2PERIPH_OFFSET)
#define GPIOA_OFFSET 		(0x0000U) // Step 6 applied.
#define GPIOA_BASE			(APB2PERIPH_BASE + GPIOA_OFFSET)

#define RCC_OFFSET			(0x00021000UL)
#define RCC_BASE			(PERIPH_BASE + RCC_OFFSET)// 0x4002 1000

#define GPIOAEN				(1U << 2) // push 1 to bit-field 2 i.g  0b 0000 0000 0000 0000 0000 0000 0100


#define PIN5				(1U << 5)
#define LED_PIN				PIN5

#define __IO				volatile

typedef struct{
	volatile uint32_t CR[2];	/*!< GPIO port configuration register LOW & HIGH,		address offset:	0x00 (LOW) 0x04 (HIGH) */
	volatile uint32_t dummy;	/*!< dummy is placed here to replace the other register in between CR and ODR*/
	volatile uint32_t ODR;		/*!< GPIO port output data register, 					address offset: 0x0C */
}GPIO_TypeDef;

typedef struct{
	volatile uint32_t dummy[6]; /*!< Dummy with an array of 6 replaces the 6 registers prior to APB2ENR */
	volatile uint32_t APB2ENR; /*!< RCC APB2 peripheral clock enable register address offset: 0x18 */
}RCC_TypeDef;

#define RCC ((RCC_TypeDef*)RCC_BASE)
#define GPIOA ((GPIO_TypeDef*)GPIOA_BASE)

int main(void){
	/* 1. Enable clock access to GPIOA*/
	RCC->APB2ENR |= GPIOAEN;
	/* 2. Set PA5 as output pin */
	/* Set all bit-fields to "0" except for 20th bit-field. */
	GPIOA->CR[0] &= 0x00000000;
	GPIOA->CR[0] |= 0x00200000;

	while(1){
		/* 3. set PA5 HIGH */
		GPIOA->ODR ^= LED_PIN;
		for(volatile int i = 0; i < 100000; i++){}

	}
}










