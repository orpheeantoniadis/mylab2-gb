#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "interrupts.h"
#include "joypad.h"

static uint8_t joypad_state = 0xff;

uint8_t get_joypad_state(void) {
	// directional request by game
	if (((P1 >> 4) & 1) == 0) {
		return (joypad_state & 0xf) | (P1 & 0x30);
	// button request by game
	} else if (((P1 >> 5) & 1) == 0) {
		return (joypad_state >> 4) | (P1 & 0x30);
	} else {
		return 0x3f;
	}
}

void key_pressed(uint8_t key) {
	// key passed from 1 to 0
	if ((joypad_state>>key)&1) {
		// directional request by game
		if (key < 4 && ((P1>>4)&1) == 0) {
			interrupt_request(IR_JOYPAD);
		// button request by game
		} else if (key >= 4 && ((P1>>5)&1) == 0) {
			interrupt_request(IR_JOYPAD);
		}
	}
	// active low so unset
	joypad_state &= ~(1<<key);
}

void key_released(uint8_t key) {
	// active low so set
	joypad_state |= 1<<key;
}