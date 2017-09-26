/*
===============================================================================
 Name        : i2c.c
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include "i2c.h"

static uint8_t DATA;
static uint8_t RW;
static bool STOP;
static bool transmit = true;

/* ***********************************************************
 * @brief	I2C interruption handler. When an interrupt
 * 			occurs, we must know where we are in the
 * 			transmission. We can do it using the I2CSTAT
 * 			register.
 *
 * @param 	none
 * @return 	none
 * ***********************************************************/
void I2C0_IRQHandler(void) {
	I2C_START(0);
	switch(LPC_I2C0->I2STAT) {
		case 0x08 : // START
			LPC_I2C0->I2DAT = 0x38 << 1 | RW;
			break;
		case 0x10 : // Repeated START
			LPC_I2C0->I2DAT = 0x38 << 1 | RW;
			break;
		case 0x18 : // Master Transmitter SLA + W + ACK
			LPC_I2C0->I2DAT = DATA;
			if (STOP == 0) transmit = true;
			break;
		case 0x28 : // Master Transmitter Data + ACK
			if (STOP == 0) LPC_I2C0->I2DAT = DATA;
			else I2C_STOP();
			transmit = true;
			break;
		case 0x40 : // Master Receiver SLA + R + ACK
			if (STOP == 0) I2C_ACK(1);
			else I2C_ACK(0);
			break;
		case 0x50 : // Master Receiver Data + ACK
			DATA = LPC_I2C0->I2DAT;
			transmit = true;
			if (STOP == 0) I2C_ACK(1);
			else I2C_ACK(0);
			break;
		case 0x58 : // Master Receiver Data + NOT ACK
			DATA = LPC_I2C0->I2DAT;
			transmit = true;
			I2C_STOP();
			break;
		default :
			transmit = true;
			I2C_STOP();
			break;
	}
	I2C_SIC();
}

/* ***********************************************************
 * @brief	I2C initialization function.
 * 			output:	pin		-	  name			-	connector
 * 					P0.27	-	  SDA0			-	P1.25
 * 					P0.28	-	  SCL0			-	P1.26
 *
 * @param 	none
 * @return 	none
 * ***********************************************************/
void init_I2C(void) {
	I2C_SDA0();			// set P0.27 to SDA0
	I2C_SCL0();			// set P0.28 to SCL0
	SET_I2CSCLH(32);	// SCLH + SCLL = 64 => I2C Rate = 400kHz
	SET_I2CSCLL(32);
	I2C_ENABLE(1);
	NVIC_EnableIRQ(I2C0_IRQn);
}

/* ***********************************************************
 * @brief	Function used to start an I2C transmission.
 *
 * @param 	data	Byte to be transmitted if in write mode
 * @param	rw		0 to write 1 to read
 * @param	stop	Boolean to set if just one transmission is
 * 					needed
 * @return 	none
 * ***********************************************************/
void I2C_start_transfer(uint8_t data, uint8_t rw, uint8_t stop) {
	while(!transmit);
	transmit = false;
	RW = rw;
	DATA = data;
	STOP = stop;
	I2C_START(1);
}

/* ***********************************************************
 * @brief	Function used to write a byte after a transmission
 * 			that didn't stop.
 *
 * @param 	data	Byte to be transmitted
 * @return 	none
 * ***********************************************************/
void write_I2C(uint8_t data) {
	while(!transmit);
	transmit = false;
	DATA = data;
}

/* ***********************************************************
 * @brief	Function used to read <nb_byte> of bytes received.
 *
 * @param 	data	Array of data received
 * @param	nb_byte	Number of bytes to read
 * @return 	none
 * ***********************************************************/
void read_I2C(uint8_t *data, uint32_t nb_byte) {
	while(!transmit);
	*data = DATA;
	if (STOP == 0) {
		uint32_t i;
		for (i = 1; i < nb_byte; i++) {
			if (i == nb_byte - 1) STOP = true;
			transmit = false;
			while(!transmit);
			data[i] = DATA;
		}
	}
}

/* ***********************************************************
 * @brief	Function used to stop the transmission.
 *
 * @param 	none
 * @return 	none
 * ***********************************************************/
void I2C_stop_transfer(void) {
	while(!transmit);
	STOP = 1;
}
