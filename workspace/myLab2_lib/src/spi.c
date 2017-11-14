/*
===============================================================================
 Name        : spi.c
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include "spi.h"

/* ***********************************************************
 * @brief	Init the SPI device.
 *
 * @param 	none
 * @return 	none
 * ***********************************************************/
void init_spi(void) {
	LPC_SC->PCLKSEL0 |= 0b01<<16;
	LPC_PINCON->PINSEL0 |= 0b11<<30; // SCK
	LPC_PINCON->PINSEL1 |= 0b111100; // !SSEL & MISO & MOSI
	LPC_SPI->SPCCR = 10;				 // setup clock to 3.125MHz (max speed) (25Mhz/8)
	LPC_SPI->SPCR |= 1<<5;	 // master mode & enable interrupt
}

/* ***********************************************************
 * @brief	Used to write datas to the SPI line. Wait until the
 * 			data transfer is finish.
 *
 * @param 	none
 * @return 	none
 * ***********************************************************/
void write_spi(uint8_t data) {
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
