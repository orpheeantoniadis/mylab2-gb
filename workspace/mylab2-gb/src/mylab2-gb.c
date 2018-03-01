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

void joystick_callback(uint8_t pos, uint8_t edge, void *arg) {
	if (edge == RISING) {
		switch(pos) {
			case JOYSTICK_RIGHT: key_pressed(0); break;
			case JOYSTICK_LEFT: key_pressed(1); break;
			case JOYSTICK_UP: key_pressed(2); break;
			case JOYSTICK_DOWN: key_pressed(3); break;
			case JOYSTICK_CENTER : key_pressed(6); break;
		}
	} else if (edge == FALLING) {
		switch(pos) {
			case JOYSTICK_RIGHT: key_released(0); break;
			case JOYSTICK_LEFT: key_released(1); break;
			case JOYSTICK_UP: key_released(2); break;
			case JOYSTICK_DOWN: key_released(3); break;
			case JOYSTICK_CENTER : key_released(6); break;
		}
	}
}

void TIMER0_IRQHandler(void) {
	joystick_handler(joystick_callback, NULL, POLLING);
	LPC_TIM0->IR = 1;
}

void EINT3_IRQHandler(void) {
	if (getGPIOInterruptStatus(GPIO2, RISING, BUTTON_A)) {
		key_released(4);
		clearGPIOInterrupt(GPIO2, BUTTON_A);
	}
	if (getGPIOInterruptStatus(GPIO2, FALLING, BUTTON_A)) {
		key_pressed(4);
		clearGPIOInterrupt(GPIO2, BUTTON_A);
	}
	if (getGPIOInterruptStatus(GPIO0, RISING, BUTTON_B)) {
		key_released(5);
		clearGPIOInterrupt(GPIO0, BUTTON_B);
	}
	if (getGPIOInterruptStatus(GPIO0, FALLING, BUTTON_B)) {
		key_pressed(5);
		clearGPIOInterrupt(GPIO0, BUTTON_B);
	}
	if (getGPIOInterruptStatus(GPIO2, RISING, TOUCH_SCREEN)) {
		key_released(7);
		clearGPIOInterrupt(GPIO2, TOUCH_SCREEN);
		buttons_states |= 1<<3;
	}
	if (getGPIOInterruptStatus(GPIO2, FALLING, TOUCH_SCREEN)) {
		//if (touchscreen_get_x1() >= START_X && touchscreen_get_x1() <= (START_X + BUTTON_WIDTH) &&
		//	touchscreen_get_y1() >= BUTTONS_Y && touchscreen_get_y1() <= (BUTTONS_Y + BUTTON_HEIGHT)) {
			key_pressed(7);
			clearGPIOInterrupt(GPIO2, TOUCH_SCREEN);
			buttons_states |= 1<<2;
		//}
	}
}

int main(void) {
	init_project();
	while(1) {
		gb_update();
	}
}
