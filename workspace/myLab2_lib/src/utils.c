/*
===============================================================================
 Name        : utils.c
 Author      : O. Antoniadis
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include <lpc_timer.h>
#include <stdbool.h>
#include "gpio.h"
#include "utils.h"

static bool last_state[5] = {false,false,false,false,false};

void Delay(uint32_t val){
	initTimer(TIMER2, 25000);
	startTimer(TIMER2);
	while(getTime(TIMER2) < val);
	stopTimer(TIMER2);
}

void joystick_handler(void(*oper)(uint8_t, uint8_t, void*), void *arg, uint8_t mode) {
	uint8_t pos;
	if (mode == POLLING) {
		for (pos = JOYSTICK_CENTER; pos <= JOYSTICK_LEFT; pos++) {
			bool current_state = JoystickGetState(pos);
			if (current_state != last_state[pos-JOYSTICK_CENTER]) {
				if (last_state[pos-JOYSTICK_CENTER] == false) oper(pos, RISING, arg);
				if (last_state[pos-JOYSTICK_CENTER] == true) oper(pos, FALLING, arg);
			}
			last_state[pos-JOYSTICK_CENTER] = current_state;
		}
		return;
	} else if (mode == TRIGGER) {
		for (pos = JOYSTICK_CENTER; pos <= JOYSTICK_LEFT; pos++) {
			if (JoystickGetState(pos)) {oper(pos, RISING, arg); return;}
		}
	}
	oper(0, 0, arg);
}
