/*
===============================================================================
 Name        : vars.h
 Author      : O. Antoniadis
 Copyright   : HES-SO hepia
 Year        : 2017-2018
===============================================================================
*/

#ifndef VARS_H_
#define VARS_H_

#ifndef VAR_DECLS
#define _DECL extern
#define _INIT(x)
#else
#define _DECL
#define _INIT(x)  = x
#endif

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

// myLab2_lib libraries
#include "gpio.h"
#include "lpc_timer.h"
#include "utils.h"
#include "lcd.h"
#include "touchscreen.h"
#include "uart.h"

// gb_lib libraries
#include "gui.h"

#define X_CENTER(width)	((LCD_MAX_WIDTH - 1) / 2 - (width) / 2)
#define Y_OFFSET		20

_DECL volatile uint8_t pixels[0x1680];
extern const uint8_t battery[];
extern const uint8_t select[];
extern const uint8_t start[];

#endif /* VARS_H_ */
