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

// gb_lib libraries
#include "cpu.h"
#include "interrupts.h"
#include "gpu.h"
#include "memory.h"
#include "timer.h"
#include "joypad.h"

#define USE_BOOTROM	0

#endif /* VARS_H_ */
