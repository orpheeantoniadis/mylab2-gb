#ifndef _GPU_H
#define _GPU_H

#include <stdint.h>
#include "memory.h"

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

#define SPRITE_DISPLAY				LCDC_BIT_ISSET(1)
#define SPRITE_SIZE						LCDC_BIT_ISSET(2)
#define WINDOW_DISPLAY				LCDC_BIT_ISSET(5)

static inline uint8_t LCD_IS_ENABLE(void) {return (LCDC>>7)&1;}
static inline uint8_t LCDC_BIT_ISSET(uint8_t bit) {return (LCDC>>bit)&1;}
static inline void STAT_SET_BIT(uint8_t bit) {STAT=STAT|(1<<bit);}
static inline void STAT_CLEAR_BIT(uint8_t bit) {STAT=STAT&~(1<<bit);}
static inline void STAT_SET_MODE(uint8_t mode) {STAT=(STAT&~3)|mode;}
static inline uint8_t STAT_GET_MODE(void) {return STAT&3;}

void draw_tileline(uint16_t pixels, uint8_t tilenum);
void draw_spriteline(uint16_t pixels, uint8_t x, uint8_t flags);
void gpu_cycle(uint8_t cycles);

#endif
