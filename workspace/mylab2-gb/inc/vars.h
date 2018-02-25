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

// other
#include "display.h"

_DECL uint8_t pixels[0x1680];
// 0:select_pushed, 1:select_released, 2:start_pushed, 3:start_released
_DECL uint8_t buttons_states _INIT(0b1010);

#endif /* VARS_H_ */
