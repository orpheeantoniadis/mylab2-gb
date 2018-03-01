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

	clear_screen(BACK_COLOR);

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
						 BORDER_COLOR);

	// display matrix text
	bmp_offset = matrix[0xa] | (matrix[0xb]<<0x8) | (matrix[0xc]<<0x10) | (matrix[0xd]<<0x18);
	width = matrix[0x12] | (matrix[0x13]<<0x8);
	height = matrix[0x16] | (matrix[0x17]<<0x8);
	display_bitmap16((uint16_t *)(&(matrix[bmp_offset])), X_CENTER(GB_LCD_WIDTH)-16, Y_OFFSET - 9, width, height, ROT_0);

	// display battery led
	bmp_offset = battery[0xa] | (battery[0xb]<<0x8) | (battery[0xc]<<0x10) | (battery[0xd]<<0x18);
	width = battery[0x12] | (battery[0x13]<<0x8);
	height = battery[0x16] | (battery[0x17]<<0x8);
	display_bitmap16((uint16_t *)(&(battery[bmp_offset])), X_CENTER(GB_LCD_WIDTH)-18, Y_OFFSET + 30, width, height, ROT_0);

	// display nintendo logo
	bmp_offset = logo[0xa] | (logo[0xb]<<0x8) | (logo[0xc]<<0x10) | (logo[0xd]<<0x18);
	width = logo[0x12] | (logo[0x13]<<0x8);
	height = logo[0x16] | (logo[0x17]<<0x8);
	display_bitmap16((uint16_t *)(&(logo[bmp_offset])), X_CENTER(GB_LCD_WIDTH+40)-2, 35+GB_LCD_HEIGHT, width, height, ROT_0);

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
		bmp_offset = pselect[0xa] | (pselect[0xb]<<0x8) | (pselect[0xc]<<0x10) | (pselect[0xd]<<0x18);
		width = pselect[0x12] | (pselect[0x13]<<0x8);
		height = pselect[0x16] | (pselect[0x17]<<0x8);
		display_bitmap16((uint16_t *)(&(pselect[bmp_offset])), SELECT_X, BUTTONS_Y, width, height, ROT_0);
	} else if (RELEASED_SELECT) {
		buttons_states &= ~(1<<1);
		bmp_offset = rselect[0xa] | (rselect[0xb]<<0x8) | (rselect[0xc]<<0x10) | (rselect[0xd]<<0x18);
		width = rselect[0x12] | (rselect[0x13]<<0x8);
		height = rselect[0x16] | (rselect[0x17]<<0x8);
		display_bitmap16((uint16_t *)(&(rselect[bmp_offset])), SELECT_X, BUTTONS_Y, width, height, ROT_0);
	}
}

void display_start(void) {
	uint32_t bmp_offset;
	uint16_t width, height;

	if (PUSHED_START) {
		buttons_states &= ~(1<<2);
		bmp_offset = pstart[0xa] | (pstart[0xb]<<0x8) | (pstart[0xc]<<0x10) | (pstart[0xd]<<0x18);
		width = pstart[0x12] | (pstart[0x13]<<0x8);
		height = pstart[0x16] | (pstart[0x17]<<0x8);
		display_bitmap16((uint16_t *)(&(pstart[bmp_offset])), START_X, BUTTONS_Y, width, height, ROT_0);
	} else if (RELEASED_START) {
		buttons_states &= ~(1<<3);
		bmp_offset = rstart[0xa] | (rstart[0xb]<<0x8) | (rstart[0xc]<<0x10) | (rstart[0xd]<<0x18);
		width = rstart[0x12] | (rstart[0x13]<<0x8);
		height = rstart[0x16] | (rstart[0x17]<<0x8);
		display_bitmap16((uint16_t *)(&(rstart[bmp_offset])), START_X, BUTTONS_Y, width, height, ROT_0);
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
