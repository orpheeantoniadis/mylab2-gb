/*
===============================================================================
 Name        : spi.h
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#ifndef _SPI_H_
#define _SPI_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

void init_spi(void);
void write_spi(uint8_t data);
uint8_t read_spi(void);

#endif /* _SPI_H */
