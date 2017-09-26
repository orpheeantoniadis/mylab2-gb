/*
===============================================================================
 Name        : i2c.h
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#ifndef _I2C_H_
#define _I2C_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <stdbool.h>

#define WRITE	0
#define READ	1

static inline void I2C_SDA0(void) {LPC_PINCON->PINSEL1 &= ~(0b11 << 22); LPC_PINCON->PINSEL1 |= 1 << 22;}
static inline void I2C_SCL0(void) {LPC_PINCON->PINSEL1 &= ~(0b11 << 24); LPC_PINCON->PINSEL1 |= 1 << 24;}

static inline void I2C_ACK(uint8_t val) {if (val) LPC_I2C0->I2CONSET=1<<2; else LPC_I2C0->I2CONCLR=1<<2;}
static inline void I2C_SIC(void) {LPC_I2C0->I2CONCLR=1<<3;}
static inline void I2C_STOP(void) {LPC_I2C0->I2CONSET=1<<4;}
static inline void I2C_START(uint8_t val) {if (val) LPC_I2C0->I2CONSET=1<<5; else LPC_I2C0->I2CONCLR=1<<5;}
static inline void I2C_ENABLE(uint8_t val) {if (val) LPC_I2C0->I2CONSET=1<<6; else LPC_I2C0->I2CONCLR=1<<6;}

static inline void SET_I2CSCLH(uint16_t val) {LPC_I2C0->I2SCLH|=val;}
static inline void SET_I2CSCLL(uint16_t val) {LPC_I2C0->I2SCLL|=val;}

void init_I2C(void);
void I2C_start_transfer(uint8_t data, uint8_t rw, uint8_t stop);
void write_I2C(uint8_t data);
void read_I2C(uint8_t *data, uint32_t nb_byte);
void I2C_stop_transfer(void);

#endif /* _I2C_H */
