/*
===============================================================================
 Name        : touchscreen.h
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#ifndef _TOUCHSCREEN_H
#define _TOUCHSCREEN_H

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdbool.h>
#include "gpio.h"
#include "i2c.h"

#define P1_XH	0x03
#define P1_XL	0x04
#define P1_YH	0x05
#define P1_YL	0x06
#define G_MODE	0xA4

void init_touchscreen(void);
void write_touchscreen(uint8_t addr, uint8_t data);
void read_touchscreen(uint8_t addr, uint8_t *data, uint32_t nb_byte);
uint16_t touchscreen_get_x1(void);
uint16_t touchscreen_get_y1(void);

#endif /* _TOUCHSCREEN_H */
