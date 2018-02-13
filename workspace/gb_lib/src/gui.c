#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "gui.h"

void boot_gameboy(char *filename) {
	load_rom(filename);
	if (USE_BOOTROM == 0) {
		registers.af = 0x01b0;
		registers.bc = 0x0013;
		registers.de = 0x00d8;
		registers.hl = 0x014d;
		registers.sp = 0xfffe;
		registers.pc = 0x100;
		interrupt_master = 1;
		write8(0xFF10, 0x80);
		write8(0xFF11, 0xBF);
		write8(0xFF12, 0xF3);
		write8(0xFF14, 0xBF);
		write8(0xFF16, 0x3F);
		write8(0xFF19, 0xBF);
		write8(0xFF1A, 0x7A);
		write8(0xFF1B, 0xFF);
		write8(0xFF1C, 0x9F);
		write8(0xFF1E, 0xBF);
		write8(0xFF20, 0xFF);
		write8(0xFF23, 0xBF);
		write8(0xFF24, 0x77);
		write8(0xFF25, 0xF3);
		write8(0xFF26, 0xF1);
		write8(0xFF40, 0x91);
		write8(0xFF47, 0xFC);
		write8(0xFF48, 0xFF);
		write8(0xFF49, 0xFF);
		write8(0xFF50, 0x01);
	}
}

void gb_update(void) {
	uint8_t cycles;
	uint32_t total_cycles = 0;
	while (total_cycles < CYCLES_FREQ) {
		cycles = cpu_cycle();
		interrupts_cycle();
		gpu_cycle(cycles);
		timer_cycle(cycles);
		total_cycles += cycles;
		// Blargg's tests
		if (SC == 0x81) {
			printf("%c", SB);
			SC = 0;
		}
	}
	draw_screen();
}