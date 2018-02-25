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

extern const uint8_t battery[];
extern const uint8_t select[];
extern const uint8_t start[];
extern const uint8_t pushed_select[];
extern const uint8_t pushed_start[];

void init_graphics(void);
void display_select(void);
void display_start(void);

#endif /* DISPLAY_H_ */
