/*
===============================================================================
 Name        : vars.h
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
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

// FreeRTOS libraries
//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
//#include "semphr.h"

// myLab2_lib libraries
#include "gpio.h"
#include "utils.h"
#include "lcd.h"
#include "touchscreen.h"

// gb_lib libraries
#include "cpu.h"
#include "interrupts.h"
#include "gpu.h"
#include "memory.h"
#include "timer.h"

#define SLEEP(mseconds)	vTaskDelay(mseconds / portTICK_RATE_MS)
#define TASK_CREATE(function, name, args, priority) xTaskCreate(function, (signed portCHAR*)name, configMINIMAL_STACK_SIZE, args, priority, NULL)

#define USE_BOOTROM	0

void gameboy(void *arg);
void display(void *arg);


#endif /* VARS_H_ */
