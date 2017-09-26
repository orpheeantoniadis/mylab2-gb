/*
===============================================================================
 Name        : dev_lcd.c
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include "lcd.h"

int main(void) {
	uint16_t line_len = 1;
	init_lcd();
	clear_screen(LCD_WHITE);
	draw_line(0, 0, 239, 319, line_len, LCD_BLACK);
	draw_line(239, 0, 0, 319, line_len, LCD_RED);
	draw_line(0, 159, 239, 159, line_len, LCD_GREEN);
	draw_line(119, 0, 119, 319, line_len, LCD_BLUE);
	draw_filled_square(119, 159, 5, LCD_BLACK);
	draw_filled_square(140, 50, 15, LCD_RED);
	lcd_print_char('B', 50, 50, LCD_GREEN, LCD_RED, BIGFONT);
	char msg[] = "Hello World! My name is %s";
	lcd_print(0, 179, SMALLFONT, LCD_WHITE, LCD_BLACK, msg, "Orphee");
	lcd_empty_rectangle(0, 0, LCD_MAX_WIDTH, LCD_MAX_HEIGHT, 2, LCD_RED);

	uint16_t bitmap[STREAM_BUFFER_SIZE], width, height;
	init_fat();
	openfiles();
	read_bmp_file(bitmap, "rail_s.bmp", &width, &height);
	display_bitmap16(bitmap, 140, 250, width, height, ROT_360);
	return 0;
}
