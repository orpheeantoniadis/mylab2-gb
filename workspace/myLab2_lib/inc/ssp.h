/*
===============================================================================
 Name        : ssp.h
 Author      : O. Antoniadis
 Copyright   : HES-SO hepia
 Year        : 2017-2018
===============================================================================
*/

#ifndef _SSP_H_
#define _SSP_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#define SSP0 LPC_SSP0
#define SSP1 LPC_SSP1

void init_ssp(LPC_SSP_TypeDef *ssp);
void write_ssp(LPC_SSP_TypeDef *ssp, uint8_t data);
uint8_t read_ssp(LPC_SSP_TypeDef *ssp);

#endif /* _SSP_H */
