/*
===============================================================================
 Name        : dev_can.c
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include "utils.h"
#include "marklin.h"

int main(void) {
	init_can();
	uint8_t i = 0;
	uint8_t j = 0;

	station_start();

	signal_cmd(133,READY);
	signal_cmd(133,DOUBLEG);
	signal_cmd(133,STOP);
	signal_cmd(133,GREEN);

	station_stop();


    while(1) {

    }


    return 0;
}
