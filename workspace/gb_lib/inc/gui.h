#ifndef _GUI_H
#define _GUI_H

#include "cpu.h"
#include "interrupts.h"
#include "gpu.h"
#include "memory.h"
#include "timer.h"
#include "joypad.h"
#include "logger.h"

#define USE_BOOTROM 0
#define USE_LCD		1
#define FREQ		60
#define CYCLES_FREQ	(CLOCKSPEED/FREQ)

void draw_screen(void);
int boot_gameboy(char *filename);
void gb_update(void);

#endif