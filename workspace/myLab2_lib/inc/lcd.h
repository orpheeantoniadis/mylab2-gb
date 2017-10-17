/*
===============================================================================
 Name        : lcd.h
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#ifndef _LCD_H
#define _LCD_H

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"
#include "spi.h"
#include "fonts.h"
#include "sys_call.h"

#define USE_SD_CARD		1

#define LCD_BLUE  (0xF<<1)
#define LCD_GREEN (0x3f<<5)
#define LCD_RED   (0x1f<<11)
#define LCD_WHITE 0xFFFF
#define LCD_BLACK 0

#define LCD_MAX_HEIGHT	320
#define LCD_MAX_WIDTH	240

enum {
	ROT_90, ROT_180, ROT_270, ROT_360
};

#define PIN_LCD_CS		16
#define PIN_LCD_DC		30
#define PIN_LCD_BL		18

#define NOP()					lcd_write_instruction(0x00) // No operation
#define COLUMN_ADDRESS_SET()	lcd_write_instruction(0x2A) // Column set
#define PAGE_ADDRESS_SET()		lcd_write_instruction(0x2B) // Line set
#define MEMORY_WRITE()			lcd_write_instruction(0x2C) // Write in LCD memory

static inline void LCD_CS(uint8_t val) {if (val) LPC_GPIO0->FIOSET=1<<PIN_LCD_CS; else LPC_GPIO0->FIOCLR=1<<PIN_LCD_CS;}
static inline void LCD_DC(uint8_t val) {if (val) LPC_GPIO1->FIOSET=1<<PIN_LCD_DC; else LPC_GPIO1->FIOCLR=1<<PIN_LCD_DC;}
static inline void LCD_BL(uint8_t val) {if (val) LPC_GPIO1->FIOSET=1<<PIN_LCD_BL; else LPC_GPIO1->FIOCLR=1<<PIN_LCD_BL;}

void init_lcd(void);
void clear_screen(uint16_t color);
void lcd_print_char(char c, uint16_t x, uint16_t y, uint16_t color, uint16_t back_color, uint8_t font_size);
void lcd_print(uint16_t x, uint16_t y, uint8_t font_size, uint16_t color, uint16_t back_color, char *st, ...);
void read_bmp_file(uint16_t *bitmap, char *filename, uint16_t *width, uint16_t *height);
void display_bitmap16(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t rotation);
void draw_filled_square(uint16_t x, uint16_t y, uint16_t len, uint16_t color);
void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t len, uint16_t color);
void lcd_empty_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t len, uint16_t color);
void lcd_filled_rectangle(int x1, int y1, int x2, int y2, int color);

// macro utilities
#define LCD_CENTER(str) (LCD_MAX_WIDTH - 1) / 2 - ((strlen(str) / 2) * 8)

#endif /* _LCD_H */
