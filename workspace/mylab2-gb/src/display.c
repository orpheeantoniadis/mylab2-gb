/*
 * display.c
 *
 *  Created on: 22 févr. 2018
 *      Author: Orphee
 */

#include "vars.h"
#include "lcd.h"

uint8_t pixels[0x1680];

// 5:start_state, 4:select_state, 3:start_released, 2:start_pushed, 1:select_released, 0:select_pushed
uint8_t buttons_states = 0b1010;

void init_graphics(void) {
	uint32_t bmp_offset;
	uint16_t width, height;

	clear_screen(0xe73b);

	// border of the gameboy screen
	lcd_empty_rectangle(X_CENTER(GB_LCD_WIDTH+40)-2,
					 	10-2,
						X_CENTER(GB_LCD_WIDTH+40)+GB_LCD_WIDTH+41,
						30+GB_LCD_HEIGHT-1+1,
						2,
						LCD_BLACK);
	lcd_filled_rectangle(X_CENTER(GB_LCD_WIDTH+40),
						 10,
						 X_CENTER(GB_LCD_WIDTH+40)+GB_LCD_WIDTH+40,
						 30+GB_LCD_HEIGHT-1,
						 0x4ace);

	// display battery led
	bmp_offset = battery[0xa] | (battery[0xb]<<0x8) | (battery[0xc]<<0x10) | (battery[0xd]<<0x18);
	width = battery[0x12] | (battery[0x13]<<0x8);
	height = battery[0x16] | (battery[0x17]<<0x8);
	display_bitmap16((uint16_t *)(&(battery[bmp_offset])), X_CENTER(GB_LCD_WIDTH)-18, Y_OFFSET + 30, width, height, ROT_0);

	// display select button
	display_select();

	// display start button
	display_start();

}

void display_select(void) {
	uint32_t bmp_offset;
	uint16_t width, height;

	if (PUSHED_SELECT) {
		buttons_states &= ~1;
		bmp_offset = pushed_select[0xa] | (pushed_select[0xb]<<0x8) | (pushed_select[0xc]<<0x10) | (pushed_select[0xd]<<0x18);
		width = pushed_select[0x12] | (pushed_select[0x13]<<0x8);
		height = pushed_select[0x16] | (pushed_select[0x17]<<0x8);
		display_bitmap16((uint16_t *)(&(pushed_select[bmp_offset])), SELECT_X, BUTTONS_Y, width, height, ROT_0);
	} else if (RELEASED_SELECT) {
		buttons_states &= ~(1<<1);
		bmp_offset = select[0xa] | (select[0xb]<<0x8) | (select[0xc]<<0x10) | (select[0xd]<<0x18);
		width = select[0x12] | (select[0x13]<<0x8);
		height = select[0x16] | (select[0x17]<<0x8);
		display_bitmap16((uint16_t *)(&(select[bmp_offset])), SELECT_X, BUTTONS_Y, width, height, ROT_0);
	}
}

void display_start(void) {
	uint32_t bmp_offset;
	uint16_t width, height;

	if (PUSHED_START) {
		buttons_states &= ~(1<<2);
		bmp_offset = pushed_start[0xa] | (pushed_start[0xb]<<0x8) | (pushed_start[0xc]<<0x10) | (pushed_start[0xd]<<0x18);
		width = pushed_start[0x12] | (pushed_start[0x13]<<0x8);
		height = pushed_start[0x16] | (pushed_start[0x17]<<0x8);
		display_bitmap16((uint16_t *)(&(pushed_start[bmp_offset])), START_X, BUTTONS_Y, width, height, ROT_0);
	} else if (RELEASED_START) {
		buttons_states &= ~(1<<3);
		bmp_offset = start[0xa] | (start[0xb]<<0x8) | (start[0xc]<<0x10) | (start[0xd]<<0x18);
		width = start[0x12] | (start[0x13]<<0x8);
		height = start[0x16] | (start[0x17]<<0x8);
		display_bitmap16((uint16_t *)(&(start[bmp_offset])), START_X, BUTTONS_Y, width, height, ROT_0);
	}
}

// Override
void set_pixel(uint16_t id, uint32_t data) {
	// clear pixel data
	pixels[id/4] &= ~(0b11 << ((id % 4) * 2));
	// set new pixel data
	pixels[id/4] |= data << ((id % 4) * 2);
}

// Override
uint32_t get_pixel(uint16_t id) {
	uint8_t pixel = (pixels[id/4] >> ((id % 4) * 2)) & 0b11;
	switch(pixel) {
		case 0: return WHITE;
		case 1: return LIGHT_GRAY;
		case 2: return DARK_GRAY;
		case 3: return BLACK;
		default : return WHITE;
	}
}

// Override
void draw_screen(void) {
	uint16_t i;
	LCD_CS(0);
	select_frame(X_CENTER(GB_LCD_WIDTH),
				 Y_OFFSET,
				 X_CENTER(GB_LCD_WIDTH)+GB_LCD_WIDTH-1,
				 Y_OFFSET+GB_LCD_HEIGHT-1);
	MEMORY_WRITE();
	for (i = 0; i < GB_LCD_WIDTH*GB_LCD_HEIGHT; i++) {
		lcd_write_data_16(get_pixel(i));
	}
	NOP();
	LCD_CS(1);
	display_select();
	display_start();
}
