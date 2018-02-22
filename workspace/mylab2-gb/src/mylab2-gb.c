/*
===============================================================================
 Name        : mylab2-gb.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#define VAR_DECLS
#include "vars.h"

void init_graphics(void) {
	uint32_t bmp_offset;
	uint16_t width, height;

	clear_screen(0xef5c);

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

}

void init_project(void) {
	init_lcd();
	init_graphics();
	initTimer(TIMER0, 25000);
	enableTimerInterrupt(TIMER0, 10, 1, 0);
	startTimer(TIMER0);
	enableGPIOInterrupt(GPIO2, RISING, BUTTON_A);
	enableGPIOInterrupt(GPIO2, FALLING, BUTTON_A);
	enableGPIOInterrupt(GPIO0, RISING, BUTTON_B);
	enableGPIOInterrupt(GPIO0, FALLING, BUTTON_B);
	init_touchscreen(POLLING);
	uart_init(UART0, 115200);
	boot_gameboy(NULL);
}

void set_pixel(uint16_t id, uint32_t data) {
	// clear pixel data
	pixels[id/4] &= ~(0b11 << ((id % 4) * 2));
	// set new pixel data
	pixels[id/4] |= data << ((id % 4) * 2);
}

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
}

void joystick_callback(uint8_t pos, uint8_t edge, void *arg) {
	if (edge == RISING) {
		switch(pos) {
			case JOYSTICK_RIGHT: key_pressed(0); break;
			case JOYSTICK_LEFT: key_pressed(1); break;
			case JOYSTICK_UP: key_pressed(2); break;
			case JOYSTICK_DOWN: key_pressed(3); break;
			case JOYSTICK_CENTER : key_pressed(6); break;
		}
	} else if (edge == FALLING) {
		switch(pos) {
			case JOYSTICK_RIGHT: key_released(0); break;
			case JOYSTICK_LEFT: key_released(1); break;
			case JOYSTICK_UP: key_released(2); break;
			case JOYSTICK_DOWN: key_released(3); break;
			case JOYSTICK_CENTER : key_released(6); break;
		}
	}
}

void TIMER0_IRQHandler(void) {
	joystick_handler(joystick_callback, NULL, POLLING);
	LPC_TIM0->IR = 1;
}

void EINT3_IRQHandler(void) {
	if (getGPIOInterruptStatus(GPIO2, RISING, BUTTON_A)) {
		key_released(4);
		clearGPIOInterrupt(GPIO2, BUTTON_A);
	}
	if (getGPIOInterruptStatus(GPIO2, FALLING, BUTTON_A)) {
		key_pressed(4);
		clearGPIOInterrupt(GPIO2, BUTTON_A);
	}
	if (getGPIOInterruptStatus(GPIO0, RISING, BUTTON_B)) {
		key_released(5);
		clearGPIOInterrupt(GPIO0, BUTTON_B);
	}
	if (getGPIOInterruptStatus(GPIO0, FALLING, BUTTON_B)) {
		key_pressed(5);
		clearGPIOInterrupt(GPIO0, BUTTON_B);
	}
	if (getGPIOInterruptStatus(GPIO2, RISING, TOUCH_SCREEN)) {
		key_released(7);
		clearGPIOInterrupt(GPIO2, TOUCH_SCREEN);
	}
	if (getGPIOInterruptStatus(GPIO2, FALLING, TOUCH_SCREEN)) {
		key_pressed(7);
		clearGPIOInterrupt(GPIO2, TOUCH_SCREEN);
	}
}

int main(void) {
	init_project();
	while(1) {
		gb_update();
	}
}
