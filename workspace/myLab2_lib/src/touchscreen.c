/*
===============================================================================
 Name        : touchscreen.c
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include "touchscreen.h"

/* ***********************************************************
 * @brief	Touchscreen initialization function.
 *
 * @param 	none
 * @return 	none
 * ***********************************************************/
void init_touchscreen(void) {
	enableGPIOInterrupt(GPIO2, FALLING, TOUCH_SCREEN);
	init_I2C();
	write_touchscreen(G_MODE, 0);
}

/* ***********************************************************
 * @brief	Function used to write in a register of the
 * 			touchscreen
 *
 * @param 	addr	Address of the register
 * @param	data	Byte to write in the register
 * @return 	none
 * ***********************************************************/
void write_touchscreen(uint8_t addr, uint8_t data) {
	I2C_start_transfer(addr, WRITE, 0);
	write_I2C(data);
	I2C_stop_transfer();
}

/* ***********************************************************
 * @brief	Function used to read from a register of the
 * 			touchscreen
 *
 * @param 	addr	Address of the register
 * @param	data	Array of bytes read from the register
 * @param	nb_byte	Number of bytes that will be read
 * @return 	none
 * ***********************************************************/
void read_touchscreen(uint8_t addr, uint8_t *data, uint32_t nb_byte) {
	I2C_start_transfer(addr, WRITE, 1);
	I2C_start_transfer(0, READ, nb_byte == 1);
	read_I2C(data, nb_byte);
}

/* ***********************************************************
 * @brief	Function used to get the x coordinate of the
 * 			first position detected by the screen
 *
 * @param 	none
 * @return 	x1	x coordinate of the first position
 * ***********************************************************/
uint16_t touchscreen_get_x1(void) {
	uint16_t x1;
	uint8_t data[2];
	read_touchscreen(P1_XL, &data[0], 1);
	read_touchscreen(P1_XH, &data[1], 1);
	x1 = data[0] | ((data[1] & 0xF) << 8);
	return 239 - x1;
}

/* ***********************************************************
 * @brief	Function used to get the y coordinate of the
 * 			first position detected by the screen
 *
 * @param 	none
 * @return 	y1	y coordinate of the first position
 * ***********************************************************/
uint16_t touchscreen_get_y1(void) {
	uint16_t y1;
	uint8_t data[2];
	read_touchscreen(P1_YL, &data[0], 1);
	read_touchscreen(P1_YH, &data[1], 1);
	y1 = data[0] | ((data[1] & 0xF) << 8);
	return 319 - y1;
}
