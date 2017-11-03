#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "lcd.h"

static uint16_t scanline_counter = 0;

void lcd_cycle(uint8_t cycles) {
	uint8_t current_line;

	if (LCD_IS_ENABLE()) {
		scanline_counter += cycles;
		if (scanline_counter >= VBLANK_PERIOD) {
			// not using write8 function because the function resets the scanline counter
			(memory.MEM[LY])++;
			current_line = read8(LY);
			scanline_counter = 0;
			if (current_line > SCANLINES_NB) memory.MEM[LY] = 0;
		}
	}
}
