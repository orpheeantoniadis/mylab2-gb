/*
===============================================================================
 Name        : utils.c
 Author      : O. Antoniadis
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include <stdbool.h>
#include "gpio.h"
#include "timers.h"
#include "utils.h"

static bool last_state[5] = {false,false,false,false,false};

void Delay(uint32_t val){
	initTimer(TIMER2, 25000);
	startTimer(TIMER2);
	while(getTime(TIMER2) != val);
	stopTimer(TIMER2);
}

void joystick_handler(void(*oper)(uint8_t arg), uint8_t mode) {
	uint8_t pos;
	if (mode == TRIGGER) {
		for (pos = CENTER; pos <= LEFT; pos++) {
			bool current_state = JoystickGetState(pos);
			if (current_state && !last_state[pos-CENTER]) oper(pos);
			last_state[pos-CENTER] = current_state;
		}
	} else if (mode == POLLING) {
		for (pos = CENTER; pos <= LEFT; pos++) {
			if (JoystickGetState(pos)) oper(pos);
		}
	}
}
