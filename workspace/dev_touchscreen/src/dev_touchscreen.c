/*
===============================================================================
 Name        : dev_touchscreen.c
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include <stdbool.h>
#include "utils.h"
#include "menu.h"

int main(void) {
	init_menu();

	print_window("Let's play");
	print_window("Salut");
	print_window("petit Orpheas !");
	print_window("hello !");
	print_window("hello1 !");
	print_window("hello2 !");
	print_window("hello3 !");
	print_window("hello4 !");
	print_window("hello5 !");
	print_window("HELLOOOOO !");
	print_window("hello7 !");
	print_window("hello8 !");
	print_window("hello9 !");
	print_window("hello10 !");
	print_window("hello11 !");
	print_window("hello12 !");
	print_window("hello13 !");
	print_window("hello14 !");
	print_window("hello15 !");
	print_window("hello16 !");

    while(1) {
    	if (click) {
    		/*uint16_t x = touchscreen_get_x1();
    		uint16_t y = touchscreen_get_y1();	*/
    		check_clicked(boutons,touchscreen_get_x1(),touchscreen_get_y1());
    		//draw_filled_square(x, y, 5, LCD_BLACK);
    		//clear_window();
    		click = false;
    	}
    }
    return 0 ;
}
