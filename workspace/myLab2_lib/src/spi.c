/*
===============================================================================
 Name        : spi.c
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include "spi.h"
#include "timers.h"

#define BUFFER_SIZE	256

struct {
	uint8_t buffer[BUFFER_SIZE];
	uint8_t read;
	uint8_t write;
} circ_buffer = {{},0,0};

void SPI_IRQHandler(void) {
	if ((LPC_SPI->SPSR & (1<<7)) != 0) {
		if (circ_buffer.read != circ_buffer.write) {
			LPC_SPI->SPDR = circ_buffer.buffer[circ_buffer.read];
			circ_buffer.read = (circ_buffer.read + 1) % BUFFER_SIZE;
		}
	}
	LPC_SPI->SPINT = 1;
}

/* ***********************************************************
 * @brief	Init the SPI device.
 *
 * @param 	none
 * @return 	none
 * ***********************************************************/
void init_spi(void) {
	LPC_SC->PCLKSEL0 |= 0b01<<16; 		// SPI clock set to 100MHz
	LPC_PINCON->PINSEL0 |= 0b11<<30; 	// SCK
	LPC_PINCON->PINSEL1 |= 0b111100; 	// !SSEL & MISO & MOSI
	LPC_SPI->SPCCR = 10;				// setup clock to 10MHz (max speed) (100Mhz/10)
	LPC_SPI->SPCR |= 0b001<<5;	 		// master mode & enable interrupt
	//NVIC_EnableIRQ(SPI_IRQn);
	//LPC_SPI->SPDR = 0;
}

/* ***********************************************************
 * @brief	Used to write datas to the SPI line. Wait until the
 * 			data transfer is finish.
 *
 * @param 	none
 * @return 	none
 * ***********************************************************/
void write_spi(uint8_t data) {
//	if ((LPC_SPI->SPSR & (1<<7)) != 0) {
//		LPC_SPI->SPDR = data;
//	} else {
//		circ_buffer.buffer[circ_buffer.write] = data;
//		circ_buffer.write = (circ_buffer.write + 1) % BUFFER_SIZE;
//	}
	LPC_SPI->SPDR = data;
	while ((LPC_SPI->SPSR & (1<<7)) == 0);
}

/* ***********************************************************
 * @brief	Function use to read datas from a SPI device.
 * 			The buffer used by the SPI controler is bi-direct-
 * 			ional. We need to write some data to generate a
 * 			clock and then we can read the SPI buffer, beacause
 * 			the datas from the slave are sent at the same time.
 *
 * @param 	none
 * @return 	none
 * ***********************************************************/
uint8_t read_spi(void){
	write_spi(0xFF);
	return LPC_SPI->SPDR;
}
