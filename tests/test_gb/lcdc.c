#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "memory.h"
#include "lcdc.h"

static uint16_t scanline_counter = 0;

void lcdc_cycle(void) {
  uint8_t current_line;

  scanline_counter++;
  if (scanline_counter >= VBLANK_PERIOD) {
    // not using write8 function because the function resets the scanline counter
    (memory.MEM[SCANLINE_REG])++;
    current_line = read8(SCANLINE_REG);
    scanline_counter = 0;
    if (current_line > SCANLINES_NB) memory.MEM[SCANLINE_REG] = 0;
  }
}
