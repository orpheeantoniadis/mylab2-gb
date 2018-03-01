/*
 * controls.h
 *
 *  Created on: 1 mars 2018
 *      Author: Orphee
 */

#ifndef CONTROLS_H_
#define CONTROLS_H_

#define CHECK_SELECT(x,y)	(x >= SELECT_X && x <= (SELECT_X + BUTTON_WIDTH) && \
							 y >= BUTTONS_Y && y <= (BUTTONS_Y + BUTTON_HEIGHT))

#define CHECK_START(x,y)	(x >= START_X && x <= (START_X + BUTTON_WIDTH) && \
							 y >= BUTTONS_Y && y <= (BUTTONS_Y + BUTTON_HEIGHT))

extern uint8_t touchscreen_edge;

void check_buttons(void);

#endif /* CONTROLS_H_ */
