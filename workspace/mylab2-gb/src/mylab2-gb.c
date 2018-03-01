/*
===============================================================================
 Name        : mylab2-gb.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#define VAR_DECLS
#include "vars.h"

void init_project(void) {
	init_lcd();
	init_graphics();
	initTimer(TIMER0, 25000);
	enableTimerInterrupt(TIMER0, 10, 1, 0);
	startTimer(TIMER0);
	enableGPIOInterrupt(GPIO2, RISING, BUTTON_A);
	enableGPIOInterrupt(GPIO2, FALLING, BUTTON_A);
	enableGPIOInterrupt(GPIO0, RISING, BUTTON_B);
	enableGPIOInterrupt(GPIO0, FALLING, BUTTON_B);
	init_touchscreen(POLLING);
	uart_init(UART0, 115200);
	boot_gameboy(NULL);
}

int main(void) {
	init_project();
	while(1) {
		gb_update();
		check_buttons();
	}
}
