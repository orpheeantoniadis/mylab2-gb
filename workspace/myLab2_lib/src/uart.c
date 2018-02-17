/*
 * uart.c
 *
 *  Created on: 15 févr. 2018
 *      Author: Orphee
 *
 *  Credits : https://github.com/colosimo/lpc-uart-calc/blob/master/main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "uart.h"

static void float_to_fraction(float ft, uint8_t *mul, uint8_t *div, uint8_t maxdiv) {
	uint8_t m, d;
	float err = ft;
	float newerr;

	for (m = 0; m < maxdiv; m++)
		for (d = 1; d <= maxdiv; d++) {
			newerr = fabs(((float)m) / ((float)d) - ft);
			if (newerr < err) {
				*mul = m;
				*div = d;
				err = newerr;
			}
		}
}

static void set_baudrate(LPC_UART0_TypeDef *uart, uint32_t baudrate) {
	float uart_pclk, br, fr_est, br_act;
	uint8_t dl_est = 0;
	uint8_t mul, div;
	uint8_t dlm, dll;

	uart_pclk = PCLK_UART;
	br = baudrate;

	if (uart_pclk / (16 * br) == floor(uart_pclk / (16 * br))) {
		div = 0;
		mul = 1;
		br_act = br;
		dl_est = uart_pclk / (16 * br);
	} else {
		fr_est = 1.5;

		do {
			dl_est = uart_pclk / (16 * br * fr_est);
			fr_est = uart_pclk / (16 * br * dl_est);
		} while (fr_est <= 1.1 && fr_est >= 1.9);

		/* Call float_to_fraction with inverted div and mul */
		float_to_fraction(fr_est - 1, &div, &mul, 15);
	}
	//printf("DIVADDVAL = %u (0x%x)\n", div, div);
	//printf("MULVAL = %u (0x%x)\n", mul, mul);
	uart->FDR &= 0x00;
	uart->FDR |= div | (mul << 4);

	if (dl_est) {
		dlm = (dl_est >> 8) & 0xff;
		dll = dl_est & 0xff;
		//printf("DLM = %u (0x%02x)\n", dlm, dlm);
		//printf("DLL = %u (0x%02x)\n", dll, dll);
		uart->DLM = dlm;
		uart->DLL = dll;
		br_act = uart_pclk / (16 * dl_est * (1 + ((float)div) / ((float)mul)));
	}

	//printf("Desired Baudrate = %.2f\n", br);
	//printf("Actual  Baudrate = %.2f\n", br_act);
	//printf("Error = %.2f%%\n", (br_act - br) / br * 100);
}

void uart_init(LPC_UART0_TypeDef *uart, uint32_t baudrate) {
	if (uart == UART0) {
		LPC_SC->PCONP |= (1<<3);
		LPC_PINCON->PINSEL0 &= ~(0b101 << 4);
		LPC_PINCON->PINSEL0 |= 0b101 << 4;
		uart->LCR = 0b10000011;
		set_baudrate(uart, baudrate);
		uart->FCR = 1;
		uart->LCR = 0b11;
	} else if (uart == UART2) {
		LPC_SC->PCONP |= (1<<24);
		LPC_PINCON->PINSEL0 &= ~(0b101 << 20);
		LPC_PINCON->PINSEL0 |= 0b101 << 20;
	} else if (uart == UART3) {
		LPC_SC->PCONP |= (1<<25);
		LPC_PINCON->PINSEL0 &= ~0b1010;
		LPC_PINCON->PINSEL0 |= 0b1010;
	}
}

void uart_send(LPC_UART0_TypeDef *uart, uint8_t *data, uint32_t length) {
	uint32_t i = 0;
	for (i = 0; i < length; i++) {
		uart->THR = data[i];
		// rajouter while sur flag de fin de transfert
	}
}
