/*
 * display.h
 *
 *  Created on: 22 févr. 2018
 *      Author: Orphee
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#define X_CENTER(width)	((LCD_MAX_WIDTH - 1) / 2 - (width) / 2)
#define Y_OFFSET		20

#define START_X			(X_CENTER(GB_LCD_WIDTH)+104)
#define SELECT_X		(X_CENTER(GB_LCD_WIDTH)+24)
#define BUTTONS_Y 		250
#define BUTTON_WIDTH	32
#define BUTTON_HEIGHT	32

#define PUSHED_SELECT 	(buttons_states & 1)
#define RELEASED_SELECT ((buttons_states >> 1) & 1)
#define PUSHED_START 	((buttons_states >> 2) & 1)
#define RELEASED_START 	((buttons_states >> 3) & 1)

extern const uint8_t battery[];
extern const uint8_t select[];
extern const uint8_t start[];
extern const uint8_t pushed_select[];
extern const uint8_t pushed_start[];

extern uint8_t pixels[0x1680];
extern uint8_t buttons_states;

void init_graphics(void);
void display_select(void);
void display_start(void);

#endif /* DISPLAY_H_ */
