/**
 * @file		mylab2_gb.c
 * @brief		Main project
 *
 * @author		Orphée Antoniadis
 * @date		September 26, 2017
 * @version		0.0.1
 */

#define VAR_DECLS
#include "vars.h"

void init_project(void) {
	init_lcd();
	clear_screen(LCD_BLACK);
	load_rom();
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

void draw_tileline(uint16_t pixels, uint8_t tilenum) {
	uint8_t i;
	uint8_t part1, part2, color;

	part1 = pixels & 0xff;
	part2 = (pixels >> 8) & 0xff;
	LCD_CS(0);
	select_frame(8 * tilenum, LY, 8 * tilenum + 8, LY);
	MEMORY_WRITE();
	for (i = 0; i < 8; i++) {
		color = (part1 >> (7 - i) & 1) | ((part2 >> (7 - i) & 1) << 1);
		switch(color) {
		case 0:
			lcd_write_data_16(LCD_WHITE);
			break;
		case 1:
			lcd_write_data_16(LCD_LIGHT_GRAY);
			break;
		case 2:
			lcd_write_data_16(LCD_GRAY);
			break;
		case 3:
			lcd_write_data_16(LCD_BLACK);
			break;
		}
	}
	NOP();
	LCD_CS(1);
}

void draw_tiledata(void) {
	volatile uint16_t data_addr;
	volatile uint8_t tilenum = 0;
	volatile uint8_t tileline = 0;
	volatile uint16_t pixels;
	volatile uint8_t i;
	volatile uint8_t part1, part2, color;
	volatile uint8_t line;

	for (data_addr = 0x8000; data_addr < 0x9000; data_addr+=2) {
		pixels = read16(data_addr);
		part1 = pixels & 0xff;
		part2 = (pixels >> 8) & 0xff;
		if (data_addr != 0x8000)
			if (((data_addr - 0x8000) % 16) == 0) tilenum++;
		line = ((tilenum / 20) * 8) + tileline;

		LCD_CS(0);
		select_frame((tilenum % 20) * 8, line, (tilenum % 20) * 8 + 8, line);
		MEMORY_WRITE();
		for (i = 0; i < 8; i++) {
			color = (part1 >> (7 - i) & 1) | ((part2 >> (7 - i) & 1) << 1);
			switch(color) {
			case 0:
				lcd_write_data_16(LCD_WHITE);
				break;
			case 1:
				lcd_write_data_16(LCD_LIGHT_GRAY);
				break;
			case 2:
				lcd_write_data_16(LCD_GRAY);
				break;
			case 3:
				lcd_write_data_16(LCD_BLACK);
				break;
			}
		}
		NOP();
		LCD_CS(1);
		tileline = (tileline + 1) % 8;
	}
}

void debug(void) {
	fprintf(stderr, "cpu stopped");
}

int main(void) {
	uint8_t cycles;
	uint8_t do_debug = 0;
	volatile uint32_t nb_cycles = 0;
	volatile uint32_t nb_instructions = 0;
	init_project();
	while(1) {
		cycles = cpu_cycle();
		nb_cycles += cycles;
		interrupts_cycle();
		gpu_cycle(cycles);
		timer_cycle(cycles);
		//if (registers.pc == 0x282a) do_debug = 1;
		//if (registers.pc == 0x27f8) do_debug = 1;
		//if (registers.pc == 0x284d) do_debug = 1;
		//if (do_debug) {
		//	debug();
		//	do_debug = 0;
		//}
		nb_instructions++;
	}
	print_registers();

}
