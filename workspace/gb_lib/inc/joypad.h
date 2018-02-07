#ifndef _JOYPAD_H
#define _JOYPAD_H

#include <stdint.h>

uint8_t get_joypad_state(void);
void key_pressed(uint8_t key);
void key_released(uint8_t key);

#endif