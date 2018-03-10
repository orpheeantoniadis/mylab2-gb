/*
 * controls.c
 *
 *  Created on: 1 mars 2018
 *      Author: Orphee
 */

#include "vars.h"
#include "controls.h"

// 0: null, 1: RISING, 2: FALLING
uint8_t touchscreen_edge = 0;

void joystick_callback(uint8_t pos, uint8_t edge, void *arg)
{
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
		touchscreen_edge |= 1;
		clearGPIOInterrupt(GPIO2, TOUCH_SCREEN);
	}
	if (getGPIOInterruptStatus(GPIO2, FALLING, TOUCH_SCREEN)) {
		touchscreen_edge |= 2;
		clearGPIOInterrupt(GPIO2, TOUCH_SCREEN);
	}
}

void check_buttons(void) {
	uint16_t x, y;

	if (touchscreen_edge != 0) {
		x = touchscreen_get_x1();
		y = touchscreen_get_y1();

		if (CHECK_SELECT(x,y)) {
			if ((touchscreen_edge & 2) == 2) {
				key_pressed(6);
				buttons_states |= 0b10001;
				touchscreen_edge &= ~2;
			} else if ((touchscreen_edge & 1) == 1) {
				key_released(6);
				buttons_states &= ~(1<<4);
				buttons_states |= 1<<1;
				touchscreen_edge &= ~1;
			}
		} else if (CHECK_START(x,y)) {
			if ((touchscreen_edge & 2) == 2) {
				key_pressed(7);
				buttons_states |= 0b100100;
				touchscreen_edge &= ~2;
			} else if ((touchscreen_edge & 1) == 1) {
				key_released(7);
				buttons_states &= ~(1<<5);
				buttons_states |= 1<<3;
				touchscreen_edge &= ~1;
			}
		} else {
			if ((touchscreen_edge & 2) == 2) {
				touchscreen_edge &= ~2;
			} else if ((touchscreen_edge & 1) == 1) {
				if ((buttons_states & (1<<4)) == (1<<4)) {
					key_released(6);
					buttons_states &= ~(1<<4);
					buttons_states |= 1<<1;
				} else if ((buttons_states & (1<<5)) == (1<<5)) {
					key_released(7);
					buttons_states &= ~(1<<5);
					buttons_states |= 1<<3;
					touchscreen_edge &= ~1;
				}
				touchscreen_edge &= ~1;
			}
		}
	}
}
