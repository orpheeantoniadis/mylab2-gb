#include <stdio.h>
#include <stdlib.h>
#include "interrupts.h"
#include "timer.h"

static uint16_t timer_cycles_counter = 1024;
static uint8_t divider_cycles_counter = 255;

static void set_frequency(void) {
	switch (read8(TAC) & 0b11) {
		case 0: timer_cycles_counter = 1024; break;	// CLOCKSPEED / 4096
		case 1: timer_cycles_counter = 16; break;		// CLOCKSPEED / 262144
		case 2: timer_cycles_counter = 64; break;		// CLOCKSPEED / 65536
		case 3: timer_cycles_counter = 255; break;	// CLOCKSPEED / 16384
	}
}

void timer_cycle(uint8_t cycles) {
	if (TIMER_IS_ENABLE()) {
		if (timer_cycles_counter < cycles) {
			set_frequency();
			if (read8(TIMA) == TIMER_OVERFLOW) {
				write8(TIMA, read8(TMA));
				interrupt_request(IR_TIMER);
			}
			else {
				write8(TIMA, read8(TIMA) + 1);
			}
		}
		else timer_cycles_counter -= cycles;
	}
	if (divider_cycles_counter < cycles) {
		divider_cycles_counter = 255;
		(memory.MEM[DIV])++;
	}
	else divider_cycles_counter -= cycles;
}
