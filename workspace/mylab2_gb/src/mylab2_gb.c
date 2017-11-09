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

void draw_tileline(uint16_t pixels) {
	
}

int main(void) {
	init_project();
	lcd_print(LCD_CENTER("Hello World!"), 160, SMALLFONT, LCD_WHITE, LCD_BLACK, "Hello World!");
}
