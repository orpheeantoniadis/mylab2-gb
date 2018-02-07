#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "interrupts.h"

uint8_t interrupt_master = 0;
static uint8_t pending_interrupt = 0;

void interrupt_request(uint8_t id) { IF = IF | (1 << id); }

void enable_interrupts(uint8_t wait_cycle) {
	if (wait_cycle) {
		pending_interrupt = 1;
	} else {
		interrupt_master = 1;
	}
}

void interrupts_cycle(void) {
	uint8_t i;
	if ((interrupt_master == 1) && (IF != 0)) {
		for (i = IR_VBLANK; i <= IR_JOYPAD; i++) {
			if (((IF >> i) & 1) && ((IE >> i) & 1)) {
				interrupt_master = 0;
				halted = 0;
				IF = IF & ~(1 << i);
				push(registers.pc);
				switch (i) {
					case IR_VBLANK: registers.pc = 0x40; break;
					case IR_LCD: registers.pc = 0x48; break;
					case IR_TIMER: registers.pc = 0x50; break;
					case IR_SERIAL: registers.pc = 0x58; break;
					case IR_JOYPAD: registers.pc = 0x60; break;
				}
			}
		}
	}
	if (pending_interrupt == 1) {
		pending_interrupt = 0;
		interrupt_master = 1;
	}
	if (halted == 1) {
		for (i = IR_VBLANK; i <= IR_JOYPAD; i++) {
			if (((IF >> i) & 1) && ((IE >> i) & 1)) {
				halted = 0;
			}
		}
	}
}
