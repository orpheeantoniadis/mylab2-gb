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
	load_rom(FILENAME);
}

void draw_tileline(uint16_t pixels, uint8_t tilenum) {
	uint8_t i;
	uint8_t part1, part2, color;

	part1 = pixels & 0xff;
	part2 = (pixels >> 8) & 0xff;
	select_frame(8 * tilenum, LY, GB_LCD_WIDTH, LY);
	LCD_CS(0);
	MEMORY_WRITE();
	for (i = 0; i < 8; i++) {
		color = (part1 >> (7 - i) & 1) | ((part2 >> (7 - i) & 0b10) << 1);
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

//void draw_tileline(uint16_t pixels, uint8_t tilenum) {
//
//}

int main(void) {
	uint8_t cycles;
	volatile uint32_t nb_cycles = 0;
	volatile uint32_t nb_instructions = 0;
	init_project();
	while(registers.pc != 0x100) {
		cycles = cpu_cycle();
		nb_cycles += cycles;
		gpu_cycle(cycles);
		timer_cycle(cycles);
		interrupts_cycle();
		nb_instructions++;
	}

}
