// APB: advanced peripheral bus
// AHB: advanced high-performance bus
// 1. where is the LED connected?
// 2. PA5 (pin21)
// 3. Search Memory Map in datasheet
// 4. register boundary addresses
// 5. 0x4000 0000 + 0x0001 0800 = 0x4001 0800
// 6. It's 32-bit so 0x0000 0000. each 0 is 4 bit. 4*8 is 32-bit
// mode register

#define PERIPH_BASE 		(0x40000000UL)
#define APB2PERIPH_OFFSET 	(0x00010800UL)
#define APB2PERIPH_BASE		(PERIPH_BASE + APB2PERIPH_OFFSET)
#define GPIOA_OFFSET 		(0x0000U) // Step 6 applied.
#define GPIOA_BASE			(APB2PERIPH_BASE + GPIOA_OFFSET)

#define RCC_OFFSET			(0x00021000UL)
#define RCC_BASE			(PERIPH_BASE + RCC_OFFSET)// 0x4002 1000

#define APB2EN_R_OFFSET		(0x18UL)
#define RCC_APB2ENR			(*(volatile unsigned int *)(RCC_BASE + APB2EN_R_OFFSET))

#define GPIOAEN				(1U << 2) // push 1 to bit-field 2 i.g  0b 0000 0000 0000 0000 0000 0000 0100

#define GPIOx_CRL_R_OFFSET	(0x00UL)
#define GPIOA_CRL_R			(*(volatile unsigned int *)(GPIOA_BASE + GPIOx_CRL_R_OFFSET))

#define GPIOx_ODR_OFFSET	(0x0CUL)
#define GPIOx_ODR			(*(volatile unsigned int *)(GPIOA_BASE + GPIOx_ODR_OFFSET))

#define PIN5				(1U << 5)
#define LED_PIN				PIN5


/*
 * rcc reg = 0b 0000 0000 0000 0000 0000 0000 0000
 */

int main(void){
	/* 1. Enable clock access to GPIOA*/
	RCC_APB2ENR |= GPIOAEN;
	/* 2. Set PA5 as output pin */
	/* Set all bit-fields to "0" except for 20th bit-field. */
	//GPIOA_CRL_R = 0x00000020;
	GPIOA_CRL_R &= 0x00000000;
	GPIOA_CRL_R |= 0x00200000;


	while(1){
		/* 3. set PA5 HIGH */
		GPIOx_ODR ^= LED_PIN;
		for(volatile int i = 0; i < 100000; i++){}

	}
}










