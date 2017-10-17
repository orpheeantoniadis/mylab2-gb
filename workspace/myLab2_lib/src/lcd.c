/*
===============================================================================
 Name        : lcd.c
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include "lcd.h"

static uint32_t nb_pixel = LCD_MAX_HEIGHT * LCD_MAX_WIDTH;

/* ***********************************************************
 * @brief	LCD GPIO initialization function.
 *
 * @param 	none
 * @return 	none
 * ***********************************************************/
static void init_LCD_GPIO(void) {
	LPC_GPIO0->FIODIR |= 1<<PIN_LCD_CS;
	LPC_GPIO1->FIODIR |= 1<<PIN_LCD_DC;
	LPC_GPIO1->FIODIR |= 1<<PIN_LCD_BL;
}

/* ***********************************************************
 * @brief	Function used to write an instruction (DC = 0) to
 * 			the LCD screen
 *
 * @param 	cmd		1 byte instruction
 * @return 	none
 * ***********************************************************/
static void lcd_write_instruction(uint8_t cmd) {
	LCD_DC(0); // command mode
	write_spi(cmd);
}

/* ***********************************************************
 * @brief	Function used to write a data (DC = 1) to
 * 			the LCD screen
 *
 * @param 	data		1 byte of data
 * @return 	none
 * ***********************************************************/
static void lcd_write_data_8(uint8_t data) {
	LCD_DC(1); // data mode
	write_spi(data);
}

/* ***********************************************************
 * @brief	Function used to write 2 bytes of data (DC = 1)
 * 			to the LCD screen
 *
 * @param 	data		2 bytes of data
 * @return 	none
 * ***********************************************************/
static void lcd_write_data_16(uint16_t data) {
	uint8_t data1 = data >> 8;
	uint8_t data2 = data & 0xFF;
	lcd_write_data_8(data1);
	lcd_write_data_8(data2);
}

/* ***********************************************************
 * @brief	ILI9341 LCD screen initialization function.
 *
 * @param 	none
 * @return 	none
 * ***********************************************************/
static void ILI9341_Initial(void) {
	LCD_CS(0);

	lcd_write_instruction(0x01); //software reset
	Delay(5);

	lcd_write_instruction(0x11);
	Delay(120);

 	lcd_write_instruction(0xCF);
	lcd_write_data_8(0x00);
	lcd_write_data_8(0x83);
	lcd_write_data_8(0X30);

 	lcd_write_instruction(0xED);
	lcd_write_data_8(0x64);
	lcd_write_data_8(0x03);
	lcd_write_data_8(0X12);
	lcd_write_data_8(0X81);

 	lcd_write_instruction(0xE8);
	lcd_write_data_8(0x85);
	lcd_write_data_8(0x01);
	lcd_write_data_8(0x79);

 	lcd_write_instruction(0xCB);
	lcd_write_data_8(0x39);
	lcd_write_data_8(0x2C);
	lcd_write_data_8(0x00);
	lcd_write_data_8(0x34);
	lcd_write_data_8(0x02);

 	lcd_write_instruction(0xF7);
	lcd_write_data_8(0x20);

 	lcd_write_instruction(0xEA);
	lcd_write_data_8(0x00);
	lcd_write_data_8(0x00);


 	lcd_write_instruction(0xC1);    //Power control
	lcd_write_data_8(0x11);   //SAP[2:0];BT[3:0]

 	lcd_write_instruction(0xC5);    //VCM control 1
	lcd_write_data_8(0x34);
	lcd_write_data_8(0x3D);

 	lcd_write_instruction(0xC7);    //VCM control 2
	lcd_write_data_8(0xC0);

 	lcd_write_instruction(0x36);    // Memory Access Control
	lcd_write_data_8(0x08);

 	lcd_write_instruction(0x3A);   	 // Pixel format
	lcd_write_data_8(0x55);  //16bit

 	lcd_write_instruction(0xB1);   	   // Frame rate
	lcd_write_data_8(0x00);
	lcd_write_data_8(0x1D);  //65Hz

 	lcd_write_instruction(0xB6);    // Display Function Control
	lcd_write_data_8(0x0A);
	lcd_write_data_8(0xA2);
	lcd_write_data_8(0x27);
	lcd_write_data_8(0x00);

	lcd_write_instruction(0xb7); //Entry mode
	lcd_write_data_8(0x07);


 	lcd_write_instruction(0xF2);    // 3Gamma Function Disable
	lcd_write_data_8(0x08);

 	lcd_write_instruction(0x26);    //Gamma curve selected
	lcd_write_data_8(0x01);


	lcd_write_instruction(0xE0); //positive gamma correction
	lcd_write_data_8(0x1f);
	lcd_write_data_8(0x1a);
	lcd_write_data_8(0x18);
	lcd_write_data_8(0x0a);
	lcd_write_data_8(0x0f);
	lcd_write_data_8(0x06);
	lcd_write_data_8(0x45);
	lcd_write_data_8(0x87);
	lcd_write_data_8(0x32);
	lcd_write_data_8(0x0a);
	lcd_write_data_8(0x07);
	lcd_write_data_8(0x02);
	lcd_write_data_8(0x07);
	lcd_write_data_8(0x05);
	lcd_write_data_8(0x00);

	lcd_write_instruction(0xE1); //negamma correction
	lcd_write_data_8(0x00);
	lcd_write_data_8(0x25);
	lcd_write_data_8(0x27);
	lcd_write_data_8(0x05);
	lcd_write_data_8(0x10);
	lcd_write_data_8(0x09);
	lcd_write_data_8(0x3a);
	lcd_write_data_8(0x78);
	lcd_write_data_8(0x4d);
	lcd_write_data_8(0x05);
	lcd_write_data_8(0x18);
	lcd_write_data_8(0x0d);
	lcd_write_data_8(0x38);
	lcd_write_data_8(0x3a);
	lcd_write_data_8(0x1f);

 	lcd_write_instruction(0x11);    //Exit Sleep
 	Delay(120);
 	lcd_write_instruction(0x29);    //Display on

 	LCD_CS(1);
 	Delay(50);
}

/* ***********************************************************
 * @brief	Function used to select a frame of pixels that
 * 			will be writable
 *
 * @param 	x1		x coordinate of the upper left side
 * @param	y1		y coordinate of the upper left side
 * @param	x2		x coordinate of the lower right side
 * @param	y2		y coordinate of the lower right side
 * @return 	none
 * ***********************************************************/
static void select_frame(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	nb_pixel = (x2 - x1 + 1) * (y2 - y1 + 1);
	LCD_CS(0);
	COLUMN_ADDRESS_SET();
	lcd_write_data_16(x1);
	lcd_write_data_16(x2);
	PAGE_ADDRESS_SET();
	lcd_write_data_16(y1);
	lcd_write_data_16(y2);
	LCD_CS(1);
}

/* ***********************************************************
 * @brief	Function used to fill the frame actually selected
 * 			with an RGB color
 *
 * @param	color	16 bits color
 * @return 	none
 * ***********************************************************/
static void fill_frame(uint16_t color) {
	LCD_CS(0);
	MEMORY_WRITE();
	uint32_t i;
	for (i = 0; i < nb_pixel; i++) {
		lcd_write_data_16(color);
	}
	NOP();
	LCD_CS(1);
}

/* ***********************************************************
 * @brief	LCD initialization function.
 * 			output:	pin		-	name	-	connector
 * 					P0.16	-	CS		-	P1.14
 * 					P1.30	-	DC		-	P1.19
 * 					P1.18	-	BL		-	P3.19
 *
 * @param 	none
 * @return 	none
 * ***********************************************************/
void init_lcd(void) {
	init_spi();
	init_LCD_GPIO();
	LCD_BL(1);
	ILI9341_Initial();
}

void clear_screen(uint16_t color) {
	//LCD_CS(0);
	select_frame(0, 0, LCD_MAX_WIDTH - 1, LCD_MAX_HEIGHT - 1);
	fill_frame(color);
	//LCD_CS(1);
}

void set_pixel8(uint8_t val, uint16_t color, uint16_t back_color) {
	uint8_t i, bit_state;
	for (i = 0; i < 8; i++) {
		bit_state = 1 & (val >> (7 - i));
		if (bit_state == 1) lcd_write_data_16(color);
		else lcd_write_data_16(back_color);
	}
}

void lcd_print_char(char c, uint16_t x, uint16_t y, uint16_t color, uint16_t back_color, uint8_t font_size) {
	fontdatatype *font;
	if (font_size == BIGFONT) font = BigFont;
	else font = SmallFont;
	uint8_t width = font[0];
	uint8_t height = font[1];
	uint8_t offset = font[2];
	uint8_t char_size = height * (width / 8);
	uint32_t char_pos = (c - offset) * char_size + 4;
	uint32_t i;
	select_frame(x, y, x + width - 1, y + height - 1);
	LCD_CS(0);
	MEMORY_WRITE();
	for (i = char_pos; i < char_pos + char_size; i++) {
		set_pixel8(font[i], color, back_color);
	}
	NOP();
	LCD_CS(1);
}

void lcd_print(uint16_t x, uint16_t y, uint8_t font_size, uint16_t color, uint16_t back_color, char *st, ...) {
	uint16_t x_char = x;
	uint16_t y_char = y;
	uint16_t i;
	uint8_t width, height;
	uint16_t str_size = strlen(st);
	va_list ap;
	va_start(ap, st);
	char *varg;
	uint16_t arg_len;

	if (font_size == BIGFONT) {width = BigFont[0]; height = BigFont[1];}
	else {width = SmallFont[0]; height = SmallFont[1];}

	for (i = 0; i < str_size; i++) {
		if (st[i] == '\n') {
			x_char = 0;
			y_char += height;
		} else if (st[i] == '%' && i + 1 != str_size) {
			switch(st[i+1]) {
				case 's' :
					varg = va_arg(ap, char*);
					lcd_print(x_char, y_char, font_size, color, back_color, varg);
					arg_len = strlen(varg);
					y_char += ((x_char + arg_len * width) / LCD_MAX_WIDTH) * height;
					x_char = (x_char + arg_len * width) % LCD_MAX_WIDTH;
					i++;
					break;
				default :
					break;
			}
		} else {
			if (x_char + width > LCD_MAX_WIDTH) {
				x_char = 0;
				y_char += height;
			}
			lcd_print_char(st[i], x_char, y_char, color, back_color, font_size);
			x_char += width;
		}
	}
	va_end(ap);
}

void read_bmp_file(uint16_t *bitmap, char *filename, uint16_t *width, uint16_t *height) {
	uint32_t bmp_offset, file_size;

	if (USE_SD_CARD) {
		uint8_t file[STREAM_BUFFER_SIZE];
		int fd = get_fd(filename);
		if (fd != -1) {
			read(fd, file, STREAM_BUFFER_SIZE);
			sd_split_block(&file_size, 0x2, sizeof(uint32_t), file);
			sd_split_block(&bmp_offset, 0xa, sizeof(uint32_t), file);
			sd_split_block(width, 0x12, sizeof(uint16_t), file);
			sd_split_block(height, 0x16, sizeof(uint16_t), file);
			sd_split_block(bitmap, bmp_offset, file_size - bmp_offset, file);
		}
	}
}

void display_bitmap16(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t rotation) {
	int i, j;
	select_frame(x, y, x + width - 1, y + height - 1);
	LCD_CS(0);
	MEMORY_WRITE();
	switch(rotation) {
		case ROT_90 :
			for (j = 0; j < width; j++) for (i = 0; i < height ; i++) lcd_write_data_16(bitmap[i * width + j]);
			break;
		case ROT_180 :
			for (i = 0; i < height ; i++) for (j = width - 1; j >= 0; j--) lcd_write_data_16(bitmap[i * width + j]);
			break;
		case ROT_270 :
			for (j = width - 1; j >= 0; j--) for (i = height - 1; i >= 0 ; i--) lcd_write_data_16(bitmap[i * width + j]);
			break;
		case ROT_360 :
			for (i = height - 1; i >= 0 ; i--) for (j = 0; j < width; j++) lcd_write_data_16(bitmap[i * width + j]);
			break;
	}
	NOP();
	LCD_CS(1);
}

/* ***********************************************************
 * @brief	Function used to draw a square on the screen.
 *
 * @param 	x		x coordinate of the top left corner
 * @param	y		y coordinate of the top left corner
 * @param	len		lenght of the square
 * @param	color	16 bits color
 * @return 	none
 * ***********************************************************/
void draw_filled_square(uint16_t x, uint16_t y, uint16_t len, uint16_t color) {
	uint16_t x1, y1, x2, y2;

	if (x + len > LCD_MAX_WIDTH) {
		x1 = LCD_MAX_WIDTH - len;
		x2 = LCD_MAX_WIDTH - 1;
	} else {
		x1 = x;
		x2 = x + (len - 1);
	}

	if (y + len > LCD_MAX_HEIGHT) {
		y1 = LCD_MAX_HEIGHT - len;
		y2 = LCD_MAX_HEIGHT - 1;
	} else {
		y1 = y;
		y2 = y + (len - 1);
	}

	select_frame(x1, y1, x2, y2);
	fill_frame(color);
}

/* ***********************************************************
 * @brief	Function used to draw a line of pixels on the
 * 			screen. The color of the line is the one
 * 			contained by R, G and B global variables
 *
 * @param 	x1		x coordinate of the 1st point of the line
 * @param	y1		y coordinate of the 1st point of the line
 * @param	x2		x coordinate of the 2nd point of the line
 * @param	y2		y coordinate of the 2nd point of the line
 * @param	color	16 bits color
 * @return 	none
 * ***********************************************************/
void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t len, uint16_t color) {
	if (x1 > x2) {
		draw_line(x2, y2, x1, y1, len, color);
	} else if (x1 == x2) { // vertical line
		uint16_t y;
		for (y = y1; y <= y2; y+=len) {
			draw_filled_square(x1, y, len, color);
		}
	} else {
		float a = (float) (y2 - y1) / (x2 - x1);
		float b = (float) y1 - a * x1;
		uint16_t x, y;
		for (x = x1; x <= x2; x+=len) {
			y = a * x + b;
			draw_filled_square(x, y, len, color);
		}
	}
}

void lcd_empty_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t len, uint16_t color) {
	draw_line(x1, y1, x1, y2, len, color);
	draw_line(x1, y1, x2, y1, len, color);
	draw_line(x2, y2, x1, y2, len, color);
	draw_line(x2, y1, x2, y2, len, color);
}

void lcd_filled_rectangle(int x1, int y1, int x2, int y2, int color) {
	select_frame(x1, y1, x2, y2);
	fill_frame(color);
}
