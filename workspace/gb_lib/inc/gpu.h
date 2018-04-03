#ifndef _GPU_H
#define _GPU_H

#include <stdint.h>
#include "memory.h"

#define GB_LCD_WIDTH    160
#define GB_LCD_HEIGHT   144
#define SCANLINES_NB    153

#ifdef __UNIX
#define WHITE		0x9bbc0f
#define LIGHT_GRAY	0x8bac0f
#define DARK_GRAY	0x306230
#define BLACK		0x0f380f
#else
#define WHITE		0xFFFF
#define LIGHT_GRAY	0x7BCF
#define DARK_GRAY	0x3186
#define BLACK		0x0000
#endif

#define LCD_SCAN_PERIOD 456
#define MODE2_BOUND     80
#define MODE3_BOUND     252

#define MAP_LEN			0x400
#define MAP_ADDR0		0x9800
#define MAP_ADDR1		0x9c00
#define DATA_LEN		0x1000
#define DATA_ADDR0	0x8800
#define DATA_ADDR1	0x8000

#define SPRITE_DISPLAY	LCDC_BIT_ISSET(1)
#define SIZE_FLAG		LCDC_BIT_ISSET(2)
#define WINDOW_DISPLAY	(LCDC_BIT_ISSET(5) && WINDOWY <= LY)

#define BG_LINE (((uint8_t)(LY + SCROLLY) / 8) * 32)
#define BG_TILELINE (((uint8_t)(LY + SCROLLY) % 8) * 2)
#define WINDOW_LINE (((uint8_t)(LY - WINDOWY) / 8) * 32)
#define WINDOW_TILELINE (((uint8_t)(LY - WINDOWY) % 8) * 2)

static inline uint8_t LCD_IS_ENABLE(void) {return (LCDC>>7)&1;}
static inline uint8_t LCDC_BIT_ISSET(uint8_t bit) {return (LCDC>>bit)&1;}
static inline void STAT_SET_BIT(uint8_t bit) {STAT=STAT|(1<<bit);}
static inline void STAT_CLEAR_BIT(uint8_t bit) {STAT=STAT&~(1<<bit);}
static inline void STAT_SET_MODE(uint8_t mode) {STAT=(STAT&~3)|mode;}
static inline uint8_t STAT_GET_MODE(void) {return STAT&3;}

void set_pixel(uint16_t id, uint32_t data);
uint32_t get_pixel(uint16_t id);
void gpu_cycle(uint8_t cycles);

#endif
