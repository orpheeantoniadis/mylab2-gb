#include "lcd.h"
#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

static uint16_t cpu_cycles_counter = 0;

static void draw_tiles(void) {
  uint16_t BG_tilemap_startregion;
  uint16_t window_tilemap_startregion;
  uint16_t tiledata_startregion;
  if (LCDC_BIT_ISSET(3)) BG_tilemap_startregion = TILEMAP_STARTREGION1;
  else BG_tilemap_startregion = TILEMAP_STARTREGION0;
  if (LCDC_BIT_ISSET(4)) tiledata_startregion = TILEDATA_STARTREGION1;
  else tiledata_startregion = TILEDATA_STARTREGION0;
  if (LCDC_BIT_ISSET(6)) window_tilemap_startregion = TILEMAP_STARTREGION1;
  else window_tilemap_startregion = TILEMAP_STARTREGION0;
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

void lcd_cycle(uint8_t cycles) {
  uint8_t ir_selection = 0;
  uint8_t change_mode = 0;
  if (LCD_IS_ENABLE()) {
    cpu_cycles_counter += cycles;
    if (cpu_cycles_counter < MODE2_BOUND) { // mode 2
      change_mode = STAT_GET_MODE() != 2;
      STAT_SET_MODE(2);
      ir_selection = (read8(STAT) >> 5) & 1;
    } else if (cpu_cycles_counter < MODE3_BOUND) { // mode 3
      change_mode = STAT_GET_MODE() != 3;
      STAT_SET_MODE(3);
    } else if (cpu_cycles_counter < LCD_SCAN_PERIOD) { // mode 0
      change_mode = STAT_GET_MODE() != 0;
      STAT_SET_MODE(0);
      ir_selection = (read8(STAT) >> 3) & 1;
    } else { // mode 1
      memory.MEM[LY]++;
      cpu_cycles_counter = 0;
      if (read8(LY) > SCANLINES_NB) memory.MEM[LY] = 0;
      else if (read8(LY) == GB_LCD_HEIGHT) {
        change_mode = STAT_GET_MODE() != 1;
        STAT_SET_MODE(1);
        ir_selection = (read8(STAT) >> 4) & 1;
        interrupt_request(IR_VBLANK);
      } else draw_scanline();
    }
    if (change_mode && ir_selection) interrupt_request(IR_LCD);
    if (read8(LY) == read8(LYC)) { // check coincidence flag
      STAT_SET_BIT(2);
      if ((read8(STAT) >> 6) & 1) interrupt_request(IR_LCD);
    } else STAT_CLEAR_BIT(2);
  } else { // LCD disabled so mode 1 and reset counters
    cpu_cycles_counter = 0;
    memory.MEM[LY] = 0;
    STAT_SET_BIT(0);
    STAT_CLEAR_BIT(1);
  }
}
