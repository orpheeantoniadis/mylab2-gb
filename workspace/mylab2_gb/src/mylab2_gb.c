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
	//init_touchscreen();
	//init_lcd();
	//clear_screen(LCD_BLACK);
}

int main(void) {
	init_project();

	if (TASK_CREATE(gameboy, "Gameboy task", NULL, tskIDLE_PRIORITY + 1) != pdPASS) {
		return 1;
	}
	if (TASK_CREATE(display, "Display", NULL, tskIDLE_PRIORITY + 1) != pdPASS) {
		return 1;
	}
	vTaskStartScheduler();
}
