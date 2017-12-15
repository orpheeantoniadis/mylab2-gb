#include "gpu.h"
#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

static uint16_t cpu_cycles_counter = 0;

static uint16_t tileline_num(void) {
	uint8_t draw_line = LY + SCROLLY;
	uint8_t tile_line = draw_line >> 3; // 8 is the number of lines in a tile
	return tile_line << 5; // 32 is the number of tiles in a line
}

static void draw_tiles(void) {
	uint16_t BG_tilemap_startregion;
	uint16_t window_tilemap_startregion;
	uint16_t tiledata_startregion;

	uint16_t tile_line;
	int16_t tile_id;
	uint16_t data_addr;
	uint8_t pixel_offset;
	uint8_t i;
	
	if (LCDC_BIT_ISSET(3)) BG_tilemap_startregion = TILEMAP_STARTREGION1;
	else BG_tilemap_startregion = TILEMAP_STARTREGION0;
	if (LCDC_BIT_ISSET(4)) tiledata_startregion = TILEDATA_STARTREGION1;
	else tiledata_startregion = TILEDATA_STARTREGION0;
	if (LCDC_BIT_ISSET(5)) {
		fprintf(stderr, "WINDOW ENABLED");
	}
	if (LCDC_BIT_ISSET(6)) window_tilemap_startregion = TILEMAP_STARTREGION1;
	else window_tilemap_startregion = TILEMAP_STARTREGION0;
	
	tile_line = tileline_num();
	tile_line += SCROLLX >> 3; // 8 is the number of cols in a tile
	pixel_offset = ((LY + SCROLLY) % 8) << 1; // 2 bytes per pixel
	
	for (i = 0; i < 20; i++) { // loop the tiles
		data_addr = tiledata_startregion;
		if (LCDC_BIT_ISSET(4)) {
			tile_id = (uint8_t)read8(BG_tilemap_startregion + tile_line + i);
			data_addr += tile_id << 4;
		} else {
			tile_id = (int8_t)read8(BG_tilemap_startregion + tile_line + i);
			data_addr += (tile_id + 128) << 4;
		}
		data_addr += pixel_offset;
		draw_tileline(read16(data_addr), i);
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
			cpu_cycles_counter = 0;
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
		STAT_SET_BIT(0);
		STAT_CLEAR_BIT(1);
	}
}
