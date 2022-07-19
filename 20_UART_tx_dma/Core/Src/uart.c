/*
 * uart.c
 *
 *  Created on: Jul 11, 2022
 *     Author: chlsk
 */
#include "uart.h"

#define GPIOAEN 			(1U << 2)				// TX/PB10, RX/PB11
#define UART2EN				(1U << 17)

#define CR1_TE				(1U << 3)
#define CR1_RE				(1U << 2)

#define CR1_UE				(1U << 13)
#define SR_TXE				(1U << 7)
#define CR1_RXNEIE			(1U << 5)

#define SYS_FREQ			8000000
#define APB1_CLK			SYS_FREQ

#define UART_BAUDRATE		115200

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PheriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PheriphClk, uint32_t BaudRate);


void uart2_write(int ch);

int __io_putchar(int ch){
	uart2_write(ch);
	return ch;
}

void dma1_ch7_init(uint32_t src, uint32_t dst, uint32_t len){

	/*1. Enable clock access to DMA*/
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;

	/*2. Disable DMA1 channel 7*/
	DMA1_Channel7->CCR &= ~DMA_CCR_EN;

	/*Wait until dMA1 channel 7 is disabled*/
	//while(DMA1_Channel7->CCR & DMA_CCR_EN){}

	/*3. Clear all interrupt flags of channel 7*/
	DMA1->IFCR |= DMA_IFCR_CTCIF7 | DMA_IFCR_CGIF7 | DMA_IFCR_CHTIF7 | DMA_IFCR_CTEIF7;

	/*4. Set the destination buffer. */
	DMA1_Channel7->CPAR = dst;

	/*5. Set the source buffer. */
	DMA1_Channel7->CMAR = src;

	/*6. set the length*/
	DMA1_Channel7->CNDTR = len;

	/*7. Select CH7. */

	/*8. Enable the memory increment*/
	DMA1_Channel7->CCR |= DMA_CCR_MINC;

	/*9. Configure transfer direction (memory -> peripheral)*/
	DMA1_Channel7->CCR |= DMA_CCR_DIR;

	/*10. Enable DMA transfer complete interrupt */
	DMA1_Channel7->CCR |= DMA_CCR_TCIE;

	/*11. Enable direct mode and disable FIFO mode. */
	DMA1_Channel7->CCR |= DMA_CCR_TCIE;

	/*11.1. Enable Circular mode */
	//DMA1_Channel7->CCR |= DMA_CCR_CIRC;

	/*12. Enable DMA1 CH 7*/
	DMA1_Channel7->CCR |= DMA_CCR_EN;

	/*13. Enable UART2 transmitter MDA */
	USART2->CR3 |= USART_CR3_DMAT;

	/*14. Enable DMA interrupt in NVIC */
	NVIC_EnableIRQ(DMA1_Channel7_IRQn);
}

void uart2_rxtx_init(void){
	/********** Configure uart gpio pin **********/
	/*Enable clock access to gpioa */
	RCC->APB2ENR |= GPIOAEN;
	/*Set PA2 mode to alternate function mode (TX/PA9, RX/PA10) */
//	AFIO->MAPR = 0x00000000;
//	AFIO->MAPR &= ~(1U << 2);
	GPIOA->CRL |= (1U << 15);
	GPIOA->CRL &= ~(1U << 14);
	GPIOA->CRL |= (1U << 13);
	GPIOA->CRL &= ~(1U << 12);
	GPIOA->CRL |= (1U << 11);
	GPIOA->CRL &= ~(1U << 10);
	GPIOA->CRL |= (1U << 9);
	GPIOA->CRL &= ~(1U << 8);
	//GPIOA->CRL |= 0x0000AA00;

	/*Set PA3 mode to alternate function mode (TX/PA9, RX/PA10) */
	//GPIOA->CRL |= 0x000A000;

	/********** Configure uart module **********/
	/*Enable clock access to uart2 */
	RCC->APB1ENR |= UART2EN;

	/*Configure baudrate */
	//USART2->BRR = 0x0EA6;
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/*Configure the transfer direction */
	USART2->CR1 |= CR1_TE | CR1_RE;

	/*Enable uart module */
	USART2->CR1 |= CR1_UE;
}
void uart2_rx_interrupt_init(void){
	/********** Configure uart gpio pin **********/
	/*1. Enable clock access to gpioa */
	RCC->APB2ENR |= GPIOAEN;
	/*2. Set PA2 mode to alternate function mode (TX/PA9, RX/PA10) */
//	AFIO->MAPR = 0x00000000;
//	AFIO->MAPR &= ~(1U << 2);
	GPIOA->CRL |= (1U << 15);
	GPIOA->CRL &= ~(1U << 14);
	GPIOA->CRL |= (1U << 13);
	GPIOA->CRL &= ~(1U << 12);
	GPIOA->CRL |= (1U << 11);
	GPIOA->CRL &= ~(1U << 10);
	GPIOA->CRL |= (1U << 9);
	GPIOA->CRL &= ~(1U << 8);
	//GPIOA->CRL |= 0x0000AA00;

	/*3. Set PA3 mode to alternate function mode (TX/PA9, RX/PA10) */
	//GPIOA->CRL |= 0x000A000;

	/********** Configure uart module **********/
	/*4. Enable clock access to uart2 */
	RCC->APB1ENR |= UART2EN;

	/*5. Configure baudrate */
	//USART2->BRR = 0x0EA6;
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/*6. Configure the transfer direction */
	USART2->CR1 |= CR1_TE | CR1_RE;

	/*7. Enable RXNE interrupt */
	USART2->CR1 |= USART_CR1_RXNEIE;

	/*8. Enable UART2 interrupt in NVIC*/
	NVIC_EnableIRQ(USART2_IRQn);

	/*9. Enable uart module */
	USART2->CR1 |= CR1_UE;
}
void uart2_tx_init(void){
	/********** Configure uart gpio pin **********/
	/*Enable clock access to gpioa */
	RCC->APB2ENR |= GPIOAEN;
	/*Set PA2 mode to alternate function mode (TX/PA9, RX/PA10) */
//	AFIO->MAPR = 0x00000000;
//	AFIO->MAPR &= ~(1U << 2);
	GPIOA->CRL &= ~(1U << 15);
	GPIOA->CRL |= (1U << 14);
	GPIOA->CRL &= ~(1U << 13);
	GPIOA->CRL &= ~(1U << 12);
	GPIOA->CRL |= (1U << 11);
	GPIOA->CRL &= ~(1U << 10);
	GPIOA->CRL |= (1U << 9);
	GPIOA->CRL &= ~(1U << 8);

	/********** Configure uart module **********/
	/*Enable clock access to uart2 */
	RCC->APB1ENR |= UART2EN;
	/*Configure baudrate */
	//USART2->BRR = 0x0EA6;
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);
	/*Configure the transfer direction */
	USART2->CR1 |= CR1_TE;
	/*Enable uart module */
	USART2->CR1 |= CR1_UE;
}

char uart2_read(void){
	/*Make sure the receive data register is not empty*/
	while(!(USART2->SR & SR_RXNE)){}
	/*Read data*/
	return USART2->DR;
}

void uart2_write(int ch){
	/*Make sure the transmit data register is empty before sending again*/
	while(!(USART2->SR & SR_TXE)){}
	/*Write to transmit data register*/
	USART2->DR = (ch & 0xFF);
}
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PheriphClk, uint32_t BaudRate){
	USARTx->BRR = compute_uart_bd(PheriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PheriphClk, uint32_t BaudRate){
	return ( (PheriphClk + (BaudRate/2U)) / BaudRate);
}

