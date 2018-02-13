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
	enableGPIOInterrupt(GPIO2, RISING, BUTTON_A);
	enableGPIOInterrupt(GPIO2, FALLING, BUTTON_A);
	enableGPIOInterrupt(GPIO0, RISING, BUTTON_B);
	enableGPIOInterrupt(GPIO0, FALLING, BUTTON_B);
	init_touchscreen(POLLING);
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

void set_pixel(uint16_t id, uint8_t data) {
	// clear pixel data
	pixels[id/4] &= ~(0b11 << ((id % 4) * 2));
	// set new pixel data
	pixels[id/4] |= data << ((id % 4) * 2);
}

uint8_t get_pixel(uint16_t id) {
	return (pixels[id/4] >> ((id % 4) * 2)) & 0b11;
}

void draw_screen(void) {
	uint16_t i;
	LCD_CS(0);
	select_frame(0, 0, GB_LCD_WIDTH-1, GB_LCD_HEIGHT-1);
	MEMORY_WRITE();
	for (i = 0; i < GB_LCD_WIDTH*GB_LCD_HEIGHT; i++) {
		switch(get_pixel(i)) {
			case 0: lcd_write_data_16(WHITE); break;
			case 1: lcd_write_data_16(LIGHT_GRAY); break;
			case 2: lcd_write_data_16(DARK_GRAY); break;
			case 3: lcd_write_data_16(BLACK); break;
		}
	}
	NOP();
	LCD_CS(1);
}

void draw_tileline(uint16_t data, uint8_t tilenum) {
	uint8_t i, x;
	uint8_t part1, part2, color;

	part1 = data & 0xff;
	part2 = (data >> 8) & 0xff;
	x = 8 * tilenum;

	for (i = 0; i < 8; i++) {
		color = (part1 >> (7 - i) & 1) | ((part2 >> (7 - i) & 1) << 1);
		set_pixel(LY * GB_LCD_WIDTH + x+i, get_color(0xff47, color));
	}
}

void draw_spriteline(uint16_t data, uint8_t x, uint8_t flags) {
	uint8_t i, j;
	uint8_t part1, part2, color_id;
	int color;

	part1 = data & 0xff;
	part2 = (data >> 8) & 0xff;
	j = 7;

	for (i = 0; i < 8; i++) {
		// skip off-screen pixels
		if (x+i >= GB_LCD_WIDTH) continue;

		// when priority bit is set, sprites prevail only over white color
		if (((flags >> 7) & 1) && get_pixel(LY * GB_LCD_WIDTH + x+i) != 0) continue;

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

		set_pixel(LY * GB_LCD_WIDTH + x+i, color);
		j--;
	}
}

void joystick_callback(uint8_t pos, uint8_t edge, void *arg) {
	if (edge == RISING) {
		switch(pos) {
			case JOYSTICK_RIGHT: key_pressed(0); break;
			case JOYSTICK_LEFT: key_pressed(1); break;
			case JOYSTICK_UP: key_pressed(2); break;
			case JOYSTICK_DOWN: key_pressed(3); break;
			case JOYSTICK_CENTER : key_pressed(6); break;
		}
	} else if (edge == FALLING) {
		switch(pos) {
			case JOYSTICK_RIGHT: key_released(0); break;
			case JOYSTICK_LEFT: key_released(1); break;
			case JOYSTICK_UP: key_released(2); break;
			case JOYSTICK_DOWN: key_released(3); break;
			case JOYSTICK_CENTER : key_released(6); break;
		}
	}
}

void TIMER0_IRQHandler(void) {
	joystick_handler(joystick_callback, NULL, POLLING);
	LPC_TIM0->IR = 1;
}

void EINT3_IRQHandler(void) {
	if (getGPIOInterruptStatus(GPIO2, RISING, BUTTON_A)) {
		key_released(4);
		clearGPIOInterrupt(GPIO2, BUTTON_A);
	}
	if (getGPIOInterruptStatus(GPIO2, FALLING, BUTTON_A)) {
		key_pressed(4);
		clearGPIOInterrupt(GPIO2, BUTTON_A);
	}
	if (getGPIOInterruptStatus(GPIO0, RISING, BUTTON_B)) {
		key_released(5);
		clearGPIOInterrupt(GPIO0, BUTTON_B);
	}
	if (getGPIOInterruptStatus(GPIO0, FALLING, BUTTON_B)) {
		key_pressed(5);
		clearGPIOInterrupt(GPIO0, BUTTON_B);
	}
	if (getGPIOInterruptStatus(GPIO2, RISING, TOUCH_SCREEN)) {
		key_released(7);
		clearGPIOInterrupt(GPIO2, TOUCH_SCREEN);
	}
	if (getGPIOInterruptStatus(GPIO2, FALLING, TOUCH_SCREEN)) {
		key_pressed(7);
		clearGPIOInterrupt(GPIO2, TOUCH_SCREEN);
	}
}

int main(void) {
	uint8_t cycles;
	uint32_t total_cycles;
	init_project();
	while(1) {
		total_cycles = 0;
		while (total_cycles < CYCLES_FREQ) {
			cycles = cpu_cycle();
			total_cycles += cycles;
			interrupts_cycle();
			gpu_cycle(cycles);
			timer_cycle(cycles);
		}
		draw_screen();
	}
	print_registers();
}
