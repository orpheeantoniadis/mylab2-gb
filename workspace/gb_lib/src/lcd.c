#include "lcd.h"
#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

static uint16_t scanline_counter = 0;

void lcd_cycle(uint8_t cycles) {
  uint8_t ir_selection = 0;
  uint8_t change_mode = 0;
  if (LCD_IS_ENABLE()) {
    scanline_counter += cycles;
    if (scanline_counter < MODE2_BOUND) { // mode 2
      change_mode = STAT_GET_MODE() != 2;
      STAT_SET_MODE(2);
      ir_selection = (read8(STAT) >> 5) & 1;
    } else if (scanline_counter < MODE3_BOUND) { // mode 3
      change_mode = STAT_GET_MODE() != 3;
      STAT_SET_MODE(3);
    } else if (scanline_counter < LCD_SCAN_PERIOD) { // mode 0
      change_mode = STAT_GET_MODE() != 0;
      STAT_SET_MODE(0);
      ir_selection = (read8(STAT) >> 3) & 1;
    } else { // mode 1
      memory.MEM[LY]++;
      scanline_counter = 0;
      if (read8(LY) > SCANLINES_NB) memory.MEM[LY] = 0;
      else if (read8(LY) == GB_LCD_HEIGHT) {
        change_mode = STAT_GET_MODE() != 1;
        STAT_SET_MODE(1);
        ir_selection = (read8(STAT) >> 4) & 1;
        interrupt_request(IR_VBLANK);
      }
    }
    if (change_mode && ir_selection) interrupt_request(IR_LCD);
    if (read8(LY) == read8(LYC)) { // check coincidence flag
      STAT_SET_BIT(2);
      if ((read8(STAT) >> 6) & 1) interrupt_request(IR_LCD);
    } else STAT_CLEAR_BIT(2);
  } else { // LCD disabled so mode 1 and reset counters
    scanline_counter = 0;
    memory.MEM[LY] = 0;
    STAT_SET_BIT(0);
    STAT_CLEAR_BIT(1);
  }
}
