#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "gpu.h"

static uint16_t cpu_cycles_counter = 0;

static uint32_t get_color(uint16_t addr, uint8_t color) {
	uint8_t palette = memory.MEM[addr-OAM_OFFSET];
	uint8_t new_color;

	switch(color) {
		case 0: new_color = palette & 0b11; break;
		case 1: new_color = (palette & 0b1100) >> 2; break;
		case 2: new_color = (palette & 0b110000) >> 4; break;
		case 3: new_color = (palette & 0b11000000) >> 6; break;
	}

#ifndef __UNIX
	return new_color;
#else
	switch(new_color) {
		case 0: return WHITE;
		case 1: return LIGHT_GRAY;
		case 2: return DARK_GRAY;
		case 3: return BLACK;
		default: return WHITE;
	}
#endif
}

static void update_line(void) {
	uint16_t map_addr, bg_map_addr, window_map_addr, data_addr;
	uint16_t line, col, tile_addr;
	int16_t tile_id;
	uint8_t line_offset, col_offset;
	uint8_t data1, data2, color_id, i;
	
	if (LCDC_BIT_ISSET(4)) data_addr = DATA_ADDR1;
	else data_addr = DATA_ADDR0;
	if (LCDC_BIT_ISSET(6)) window_map_addr = MAP_ADDR1;
	else window_map_addr = MAP_ADDR0;
	if (LCDC_BIT_ISSET(3)) bg_map_addr = MAP_ADDR1;
	else bg_map_addr = MAP_ADDR0;
	
	for (i = 0; i < GB_LCD_WIDTH; i++) {
		if (WINDOW_DISPLAY && i >= WINDOWX) {
			map_addr = window_map_addr;
			line = WINDOW_LINE;
			line_offset = WINDOW_TILELINE;
			col = i - WINDOWX;
		} else {
			map_addr = bg_map_addr;
			line = BG_LINE;
			line_offset = BG_TILELINE;
			col = i + SCROLLX;
		}
		col_offset = col % 8;
		col = col / 8;
		
		tile_addr = data_addr;
		if (LCDC_BIT_ISSET(4)) { // unsigned
			tile_id = (uint8_t)memory.VRAM[map_addr+line+col-0x8000];
			tile_addr += tile_id * 16;
		} else { // signed
			tile_id = (int8_t)memory.VRAM[map_addr+line+col-0x8000];
			tile_addr += (tile_id + 128) * 16;
		}
		
		data1 = memory.VRAM[tile_addr+line_offset-0x8000];
		data2 = memory.VRAM[tile_addr+line_offset-0x7fff];
		color_id = (data1 >> (7 - col_offset) & 1) | ((data2 >> (7 - col_offset) & 1) << 1);
		set_pixel(LY * GB_LCD_WIDTH + i, get_color(0xff47, color_id));
	}
	
}

static void draw_spriteline(uint16_t data, uint8_t x, uint8_t flags) {
	uint8_t i, j;
	uint8_t part1, part2, color_id;
	uint32_t color;

	part1 = data & 0xff;
	part2 = (data >> 8) & 0xff;
	j = 7;

	for (i = 0; i < 8; i++) {
		// skip off-screen pixels
		if (x+i >= GB_LCD_WIDTH) continue;

		// when priority bit is set, sprites prevail only over white color
		if (((flags >> 7) & 1) && get_pixel(LY * GB_LCD_WIDTH + x+i) != WHITE) continue;

		// X flip (bit 5 of flag register)
		if ((flags >> 5) & 1)
		  color_id = (part1 >> (7 - j) & 1) | ((part2 >> (7 - j) & 1) << 1);
		else
		  color_id = (part1 >> (7 - i) & 1) | ((part2 >> (7 - i) & 1) << 1);

		// white is transparent for sprites
		if (color_id == 0) continue;

		// address of the color palette (bit 4 of flag register)
		if ((flags >> 4) & 1)
		  color = get_color(0xff49, color_id);
		else
		  color = get_color(0xff48, color_id);

		set_pixel(LY * GB_LCD_WIDTH + x+i, color);
		j--;
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
		update_line();
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
			if (LY > SCANLINES_NB) LY = 0;
			else if (LY == GB_LCD_HEIGHT) {
				change_mode = STAT_GET_MODE() != 1;
				STAT_SET_MODE(1);
				ir_selection = (STAT >> 4) & 1;
				interrupt_request(IR_VBLANK);
			} else if (LY < GB_LCD_HEIGHT) draw_scanline();
			cpu_cycles_counter -= LCD_SCAN_PERIOD;
			LY++;
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
