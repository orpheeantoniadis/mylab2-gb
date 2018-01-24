#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "gpu.h"

static uint16_t cpu_cycles_counter = 0;

static uint16_t background_line_num(void) {
	uint8_t draw_line = LY + SCROLLY;
	uint8_t tile_line = draw_line >> 3; // 8 is the number of lines in a tile
	return tile_line << 5; // 32 is the number of tiles in a line
}

static uint16_t window_line_num(void) {
	uint8_t draw_line = LY + WINDOWY;
	uint8_t tile_line = draw_line >> 3; // 8 is the number of lines in a tile
	return tile_line << 5; // 32 is the number of tiles in a line
}

static void draw_tiles(void) {
	uint16_t tilemap_startregion;
	uint16_t data_startregion;

	uint16_t background_line, window_line;
	uint16_t background_col, window_col;
	int16_t background_tile_id, window_tile_id;
	uint16_t background_data_addr, window_data_addr;
	uint8_t background_pixel_offset, window_pixel_offset;
	uint8_t i;
	
	if (LCDC_BIT_ISSET(4)) data_startregion = TILEDATA_STARTREGION1;
	else data_startregion = TILEDATA_STARTREGION0;
	if (WINDOW_DISPLAY) {
		if (LCDC_BIT_ISSET(6)) tilemap_startregion = TILEMAP_STARTREGION1;
		else tilemap_startregion = TILEMAP_STARTREGION0;
	} else {
		if (LCDC_BIT_ISSET(3)) tilemap_startregion = TILEMAP_STARTREGION1;
		else tilemap_startregion = TILEMAP_STARTREGION0;
	}
	
	background_line = background_line_num();
	window_line = window_line_num();
	background_pixel_offset = ((LY + SCROLLY) % 8 + SCROLLX % 8) << 1; // 2 bytes per pixel
	window_pixel_offset = ((LY + WINDOWY) % 8 + WINDOWX % 8) << 1;
	background_col = SCROLLX >> 3;
	window_col = WINDOWX >> 3;
	
	for (i = 0; i < 20; i++) { // loop the tiles
		background_data_addr = data_startregion;
		window_data_addr = data_startregion;
		if (LCDC_BIT_ISSET(4)) {
			background_tile_id = (uint8_t)memory.VRAM[tilemap_startregion+background_line+background_col+i-0x8000];
			background_data_addr += background_tile_id << 4;
			window_tile_id = (uint8_t)memory.VRAM[tilemap_startregion+window_line+window_col+i-0x8000];
			window_data_addr += window_tile_id << 4;
		} else {
			background_tile_id = (int8_t)memory.VRAM[tilemap_startregion+background_line+background_col+i-0x8000];
			background_data_addr += (background_tile_id + 128) << 4;
			window_tile_id = (int8_t)memory.VRAM[tilemap_startregion+window_line+window_col+i-0x8000];
			window_data_addr += (window_tile_id + 128) << 4;
		}
		background_data_addr += background_pixel_offset;
		window_data_addr += window_pixel_offset;
		// if (WINDOW_DISPLAY && i >= window_col) {
    // 
		// } else {
			draw_tileline(read16(background_data_addr), i);
		// }
	}
}

static void draw_sprites(void) {
  
}

static void draw_scanline(void) {
	if (LCDC_BIT_ISSET(0)) {
		draw_tiles();
	}
	if (LCDC_BIT_ISSET(1)) {
		draw_sprites();
	}
}

void gpu_cycle(uint8_t cycles) {
	uint8_t ir_selection = 0;
	uint8_t change_mode = 0;
	if (LCD_IS_ENABLE()) {
		cpu_cycles_counter += cycles;
		if (cpu_cycles_counter < MODE2_BOUND) { // mode 2
			change_mode = STAT_GET_MODE() != 2;
			STAT_SET_MODE(2);
			ir_selection = (STAT >> 5) & 1;
		} else if (cpu_cycles_counter < MODE3_BOUND) { // mode 3
			change_mode = STAT_GET_MODE() != 3;
			STAT_SET_MODE(3);
		} else if (cpu_cycles_counter < LCD_SCAN_PERIOD) { // mode 0
			change_mode = STAT_GET_MODE() != 0;
			STAT_SET_MODE(0);
			ir_selection = (STAT >> 3) & 1;
		} else { // mode 1
			LY++;
			cpu_cycles_counter -= LCD_SCAN_PERIOD;
			if (LY > SCANLINES_NB) LY = 0;
			else if (LY == GB_LCD_HEIGHT) {
				change_mode = STAT_GET_MODE() != 1;
				STAT_SET_MODE(1);
				ir_selection = (STAT >> 4) & 1;
				interrupt_request(IR_VBLANK);
			} else if (LY < GB_LCD_HEIGHT) draw_scanline();
		}
		if (change_mode && ir_selection) interrupt_request(IR_LCD);
		if (LY == LYC) { // check coincidence flag
			STAT_SET_BIT(2);
			if ((STAT >> 6) & 1) interrupt_request(IR_LCD);
		} else STAT_CLEAR_BIT(2);
	} else { // LCD disabled so mode 1 and reset counters
		cpu_cycles_counter = 0;
		LY = 0;
		STAT_SET_MODE(0);
	}
}
