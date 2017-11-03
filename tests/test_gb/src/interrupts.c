#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "memory.h"
#include "interrupts.h"

uint8_t interrupt_master = 0;

void interrupt_request(uint8_t id) {
	write8(IF, read8(IF) | (1 << id));
}

void interrupts_cycle(void) {
	uint8_t i;
	if ( (interrupt_master == 1) && (read8(IF) != 0) ) {
		for (i = IR_VBLANK; i <= IR_JOYPAD; i++) {
			if ( ((read8(IF) >> i) & 1) && ((read8(IE) >> i) & 1) ) {
				interrupt_master = 0;
				write8(IF, read8(IF) & ~(1 << i));
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
}
