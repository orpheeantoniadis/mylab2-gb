/**
 * @file		display.c
 * @brief		Display task
 *
 * @author		Orphée Antoniadis
 * @date		September 26, 2017
 */

#include "vars.h"

/**
 * @brief	none
 *
 * @param 	none
 * @return 	none
 */
void display(void *arg) {
	while(1) {
		//lcd_print(LCD_CENTER("Hello World!"), 160, SMALLFONT, LCD_WHITE, LCD_BLACK, "Hello World!");
		SLEEP(10);
	}
}


