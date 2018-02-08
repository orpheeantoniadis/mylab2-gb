/*
===============================================================================
 Name        : mylab2-gb.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#define VAR_DECLS
#include "vars.h"

void init_project(void) {
	init_lcd();
	clear_screen(LCD_BLACK);
	initTimer(TIMER0, 25000);
	enableTimerInterrupt(TIMER0, 10, 1, 0);
	startTimer(TIMER0);
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
		lcd_write_data_16(get_color(0xff47, color));
	}
	NOP();
	LCD_CS(1);
}

void draw_spriteline(uint16_t data, uint8_t x, uint8_t flags) {
	uint8_t i, j;
	uint8_t part1, part2, color_id;
	int color;

	part1 = data & 0xff;
	part2 = (data >> 8) & 0xff;
	j = 7;

	LCD_CS(0);
	select_frame(x, LY, x + 8, LY);
	MEMORY_WRITE();
	for (i = 0; i < 8; i++) {
		// skip off-screen pixels
		if (x+i >= GB_LCD_WIDTH) continue;

		// when priority bit is set, sprites prevail only over white color
		//if (((flags >> 7) & 1) && pixels[LY * GB_LCD_WIDTH + x+i] != WHITE) continue;

		// X flip (bit 5 of flag register)
		if ((flags >> 5) & 1)
		  color_id = (part1 >> (7 - j) & 1) | ((part2 >> (7 - j) & 1) << 1);
		else
		  color_id = (part1 >> (7 - i) & 1) | ((part2 >> (7 - i) & 1) << 1);

		// white is transparent for sprites
		if (color_id == 0) continue;

		// address of the color palette (bit 4 of flag register)
		if ((flags >> 4) & 1)
		  color = get_color(0xff49, color_id);
		else
		  color = get_color(0xff48, color_id);

		lcd_write_data_16(color);
		j--;
	}
	NOP();
	LCD_CS(1);
}

void joystick_callback(uint8_t pos, uint8_t edge, void *arg) {
	if (edge == RISING) {
		switch(pos) {
			case JOYSTICK_RIGHT: key_pressed(0); break;
			case JOYSTICK_LEFT: key_pressed(1); break;
			case JOYSTICK_UP: key_pressed(2); break;
			case JOYSTICK_DOWN: key_pressed(3); break;
		}
	} else if (edge == FALLING) {
		switch(pos) {
			case JOYSTICK_RIGHT: key_released(0); break;
			case JOYSTICK_LEFT: key_released(1); break;
			case JOYSTICK_UP: key_released(2); break;
			case JOYSTICK_DOWN: key_released(3); break;
		}
	}
}

void TIMER0_IRQHandler() {
	joystick_handler(joystick_callback, NULL, TRIGGER);
	LPC_TIM0->IR = 1;
}

uint32_t nb_cycles = 0;

int main(void) {
	uint8_t cycles;
	init_project();
	while(1) {
		cycles = cpu_cycle();
		nb_cycles += cycles;
		interrupts_cycle();
		gpu_cycle(cycles);
		timer_cycle(cycles);
	}
	print_registers();
}
