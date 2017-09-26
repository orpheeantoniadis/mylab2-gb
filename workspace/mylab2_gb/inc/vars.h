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

// FreeRTOS libraries
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

// MyLab_lib libraries
#include "gpio.h"
#include "utils.h"
#include "lcd.h"
#include "fat.h"
#include "sys_call.h"
#include "marklin.h"
#include "touchscreen.h"

#define SLEEP(mseconds)	vTaskDelay(mseconds / portTICK_RATE_MS)
#define TASK_CREATE(function, name, args, priority) xTaskCreate(function, (signed portCHAR*)name, configMINIMAL_STACK_SIZE, args, priority, NULL)

#define BUFFER_SIZE	255

void gameboy(void *arg);
void display(void *arg);


#endif /* VARS_H_ */
