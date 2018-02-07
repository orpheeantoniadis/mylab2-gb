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
	uint8_t draw_line = LY - WINDOWY;
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
	// 2 bytes per pixel so x2
	background_pixel_offset = ((LY + SCROLLY) % 8 + SCROLLX % 8) << 1; 
	window_pixel_offset = ((LY - WINDOWY) % 8 - WINDOWX % 8) << 1;
	// 8 pixels per tile so /8
	background_col = SCROLLX >> 3;
	window_col = WINDOWX >> 3;
	
	for (i = 0; i < 20; i++) { // loop the tiles
		background_data_addr = data_startregion;
		window_data_addr = data_startregion;
		if (LCDC_BIT_ISSET(4)) { // unsigned
			background_tile_id = (uint8_t)memory.VRAM[tilemap_startregion+background_line+background_col+i-0x8000];
			window_tile_id = (uint8_t)memory.VRAM[tilemap_startregion+window_line-window_col+i-0x8000];
			// 8 pixel per line and 2 bytes per pixel so x16
			background_data_addr += background_tile_id << 4;
			window_data_addr += window_tile_id << 4; // 8 pixel per line and 2 bytes per pixel so x16
		} else { // signed
			background_tile_id = (int8_t)memory.VRAM[tilemap_startregion+background_line+background_col+i-0x8000];
			window_tile_id = (int8_t)memory.VRAM[tilemap_startregion+window_line-window_col+i-0x8000];
			// 8 pixel per line and 2 bytes per pixel so x16
			background_data_addr += (background_tile_id + 128) << 4;
			window_data_addr += (window_tile_id + 128) << 4;
		}
		background_data_addr += background_pixel_offset;
		window_data_addr += window_pixel_offset;
		if (WINDOW_DISPLAY && i >= window_col) {
    	if (i == window_col && (WINDOWX % 8) != 0) {
				// we need to combine background tile and window tile
				uint16_t background_data = read16(background_data_addr);
				background_data = (background_data << (8 - (WINDOWX % 8)) << 1) >> (8 - (WINDOWX % 8)) >> 1;
				uint16_t window_data = read16(window_data_addr);
				window_data = (window_data >> (WINDOWX % 8) >> 1) << (WINDOWX % 8) << 1;
				draw_tileline(background_data | window_data, i);
			} else if (i >= window_col) {
				// window tile only
				draw_tileline(read16(window_data_addr), i);
			}
		} else {
			// background tile only
			draw_tileline(read16(background_data_addr), i);
		}
	}
}

static void draw_sprites(void) {
	uint8_t id;
	uint8_t size;
	uint8_t x, y, pattern_num, flags;
	uint16_t data_addr;
	uint8_t sprite_line;
	uint8_t i;
	
	if (SPRITE_DISPLAY) {
	  for (i = 0; i < 40; i++) {
			// 4 bytes per sprite so x4
			id = i * 4;
			y = memory.OAM[id] - 16;
			x = memory.OAM[id+1] - 8;
			if (SIZE_FLAG == 0) {
				size = 8;
				pattern_num = memory.OAM[id+2];
			} else {
				size = 16;
				// least significant bit to 0 when size = 16
				pattern_num = memory.OAM[id+2] & ~1;
			}
			flags = memory.OAM[id+3];
			
			if (x == 0 || x >= GB_LCD_WIDTH || y == 0 || y >= GB_LCD_HEIGHT) continue;
			
			if (LY >= y && LY < (y + size)) {
				sprite_line = LY - y;
				// Y flip (bit 6 of flag register)
				if ((flags >> 6) & 1) {
					// 2 bytes per pixel so x2
					sprite_line = (sprite_line - size) * 2;
					// 8 pixel per line and 2 bytes per pixel so x16
					data_addr = 0x8000 + (pattern_num * 16) - sprite_line;
				} else {
					// 2 bytes per pixel so x2
					sprite_line = sprite_line * 2;
					// 8 pixel per line and 2 bytes per pixel so x16
					data_addr = 0x8000 + (pattern_num * 16) + sprite_line;
				}
				draw_spriteline(read16(data_addr), x, flags);
			}
		}
	}
}

static void draw_scanline(void) {
	if (LCDC_BIT_ISSET(0)) {
		draw_tiles();
	}
	if (LCDC_BIT_ISSET(1)) {
		draw_sprites();
	}
}

int get_color(uint16_t addr, uint8_t color) {
	uint8_t palette = memory.MEM[addr-OAM_OFFSET];
	uint8_t new_color;
	
	switch(color) {
		case 0: new_color = palette & 0b11; break;
		case 1: new_color = (palette & 0b1100) >> 2; break;
		case 2: new_color = (palette & 0b110000) >> 4; break;
		case 3: new_color = (palette & 0b11000000) >> 6; break;
	}
	
	switch(new_color) {
		case 0: return WHITE;
		case 1: return LIGHT_GRAY;
		case 2: return DARK_GRAY;
		case 3: return BLACK;
		default: return WHITE;
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
