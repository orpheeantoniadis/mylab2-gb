#include "ssp.h"

void init_ssp(LPC_SSP_TypeDef *ssp) {
	LPC_SC->PCLKSEL1 |= 0b01<<10; 			// SSP0 clock set to 100MHz
	LPC_SC->PCLKSEL0 |= 0b01<<20; 			// SSP1 clock set to 100MHz
	LPC_PINCON->PINSEL0 |= 0b10<<30; 		// SCK0
	LPC_PINCON->PINSEL1 |= 0b1010<<2; 		// !SSEL0 & MISO0 & MOSI0
	LPC_PINCON->PINSEL0 |= 0b101010<<14; 	// !SSEL1 & SCK1 & MISO1 & MOSI1
	ssp->CPSR = 10;							// setup clock to 10MHz (max speed) (100Mhz/10)
	ssp->CR0 = 0b111;						// 8 bits data transfer
	ssp->CR1 = 0b10;						// ssp enabled
}

void write_ssp(LPC_SSP_TypeDef *ssp, uint8_t data) {
	ssp->DR = data;
	while ((ssp->SR & (1<<2)) == 1);
}

uint8_t read_ssp(LPC_SSP_TypeDef *ssp) {
	write_ssp(ssp, 0xFF);
	return ssp->DR;
}
