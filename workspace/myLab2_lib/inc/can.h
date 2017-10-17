/*
===============================================================================
 Name        : can.h
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#ifndef CAN_H_
#define CAN_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <stdbool.h>

typedef struct{
	int ID;
	uint8_t DLC;
	bool id_size;
	uint8_t data[8];
} can_data_send;


// Bus Timing Register
#define CAN_BRP 	0
#define CAN_SJW 	14
#define CAN_TSEG1 	16
#define CAN_TSEG2 	20

// Mode register
#define CAN_OM		~1 		// operating mode
#define CAN_RM		1 		// reset mode
#define CAN_STM		(1<<2) 	// self test mode

// Command Register
#define CAN_TR		1		// transmission request
#define CAN_AT		(1<<1)	// abort transmission
#define CAN_RRB		(1<<2)	// release receive buffer
#define CAN_CDO		(1<<3)	// clear data overrun
#define CAN_SRR		(1<<4)	// self reception request
#define CAN_STB1	(1<<5)	// select tx buffer 1
#define CAN_STB2	(1<<6)	// select tx buffer 2
#define CAN_STB3	(1<<7)	// select tx buffer 3

// Transmit buffers
#define BUFFER1		0
#define BUFFER2		1
#define BUFFER3		2
#define BUF1_STAT	2	// the bit for the buffer 1 state
#define BUF2_STAT	10	// the bit for the buffer 2 state
#define BUF3_STAT	18	// the bit for the buffer 3 state

// Transmit Frame Information register
#define CAN_FF1(x)((x == 1) ? (LPC_CAN1->TFI1 |= 1<<31) : (LPC_CAN1->TFI1 &= ~(1<<31)))	// set the identifire in 11 or 29 bits mode
#define CAN_FF2(x)((x == 1) ? (LPC_CAN1->TFI2 |= 1<<31) : (LPC_CAN1->TFI2 &= ~(1<<31)))	// set the identifire in 11 or 29 bits mode
#define CAN_FF3(x)((x == 1) ? (LPC_CAN1->TFI3 |= 1<<31) : (LPC_CAN1->TFI3 &= ~(1<<31)))	// set the identifire in 11 or 29 bits mode

void init_can(void);
void write_can(can_data_send* data);
void read_can(void);

#endif /* CAN_H_ */
