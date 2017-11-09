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
	init_touchscreen();
	init_lcd();
	clear_screen(LCD_BLACK);
	//load_rom(FILENAME);
}

int main(void) {
	init_project();
	lcd_print(LCD_CENTER("Hello World!"), 160, SMALLFONT, LCD_WHITE, LCD_BLACK, "Hello World!");
	//if (TASK_CREATE(gameboy, "Gameboy task", NULL, tskIDLE_PRIORITY + 1) != pdPASS) {
	//	return 1;
	//}
	//if (TASK_CREATE(display, "Display", NULL, tskIDLE_PRIORITY + 1) != pdPASS) {
	//	return 1;
	//}
	//vTaskStartScheduler();
}
