/*
 * uart.h
 *
 *  Created on: 15 févr. 2018
 *      Author: Orphee
 */

#ifndef _UART_H_
#define _UART_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#define PCLK_UART 25000000

#define UART0 LPC_UART0
#define UART2 LPC_UART2
#define UART3 LPC_UART3

void uart_init(LPC_UART0_TypeDef *uart, uint32_t baudrate);
void uart_send(LPC_UART0_TypeDef *uart, uint8_t *data, uint32_t length);

#endif /* UART_H_ */
