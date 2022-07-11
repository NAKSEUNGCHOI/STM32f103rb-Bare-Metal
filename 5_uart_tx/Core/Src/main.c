/**
 * 	main.c
 * 	@date July-10-2022
 * 	@author Nakseung Choi
 * 	@Brief UART
 * 	Alternate function I/O has to be enabled.
 */

#include <stdint.h>
#include "stm32f103xb.h"

#define GPIOAEN 			(1U << 2)				// TX/PB10, RX/PB11
#define UART2EN				(1U << 17)

#define CR1_TE				(1U << 3)
#define CR1_UE				(1U << 13)
#define SR_TXE				(1U << 7)

#define SYS_FREQ			8000000
#define APB1_CLK			SYS_FREQ

#define UART_BAUDRATE		115200

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PheriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PheriphClk, uint32_t BaudRate);

void uart2_tx_init(void);
void uart2_write(int ch);

int main(void){

	uart2_tx_init();

	while(1){
		uart2_write('Y');
		uart2_write('B');
		uart2_write('C');
		for(volatile int i = 0; i < 1000000; i++){}
	}


}
void uart2_tx_init(void){
	/********** Configure uart gpio pin **********/
	/*Enable clock access to gpioa */
	RCC->APB2ENR |= GPIOAEN;
	/*Set PA2 mode to alternate function mode (TX/PA9, RX/PA10) */
	AFIO->MAPR = 0x00000000;
	AFIO->MAPR &= ~(1U << 2);
	GPIOA->CRL = 0x00004A00;

	/*Set PA9 alternate function type to UART_TX (AF07) */
	//AFIO->MAPR &= ~(1U << 2);

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
