#ifndef _LCD_H
#define _LCD_H

#include "memory.h"
#include <stdint.h>

#define GB_LCD_WIDTH    160
#define GB_LCD_HEIGHT   144
#define SCANLINES_NB    153

#define LCD_SCAN_PERIOD 456
#define MODE2_BOUND     80
#define MODE3_BOUND     252

#define TILEMAP_LEN           0x400
#define TILEMAP_STARTREGION0  0x9800
#define TILEMAP_STARTREGION1  0x9c00
#define TILEDATA_LEN          0x1000
#define TILEDATA_STARTREGION0 0x8800
#define TILEDATA_STARTREGION1 0x8000

#define LCDC    0xff40
#define STAT    0xff41
#define SCROLLY 0xff42
#define SCROLLX 0xff43
#define LY      0xff44
#define LYC     0xff45
#define WINDOWY 0xff4a
#define WINDOWX 0xff4b

static inline uint8_t LCD_IS_ENABLE(void) {return (read8(LCDC)>>7)&1;}
static inline uint8_t LCDC_BIT_ISSET(uint8_t bit) {return (read8(LCDC)>>bit)&1;}
static inline void STAT_SET_BIT(uint8_t bit) {write8(STAT,read8(STAT)|(1<<bit));}
static inline void STAT_CLEAR_BIT(uint8_t bit) {write8(STAT,read8(STAT)&~(1<<bit));}
static inline void STAT_SET_MODE(uint8_t mode) {write8(STAT,(read8(STAT)&~3)|mode);}
static inline uint8_t STAT_GET_MODE(void) {return read8(STAT)&3;}

void lcd_cycle(uint8_t cycles);

#endif