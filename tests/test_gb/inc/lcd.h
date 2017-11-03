#ifndef _LCD_H
#define _LCD_H

#include <stdint.h>

#define GB_LCDC_WIDTH		160
#define GB_LCDC_HEIGHT		144
#define SCANLINES_NB			153
#define VBLANK_PERIOD		456

#define LCDC	0xff40
#define STAT	0xff41
#define LY		0xff44
#define LYC		0xff45

#define LCD_IS_ENABLE()	((read8(LCDC) >> 7) & 1)

void lcd_cycle(uint8_t cycles);

#endif
