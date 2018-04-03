#ifndef _GUI_H
#define _GUI_H

#include <stdint.h>
#include "cpu.h"
#include "interrupts.h"
#include "gpu.h"
#include "memory.h"
#include "timer.h"
#include "joypad.h"

enum log_level { ERROR, WARNING, INFO, _DEBUG};

#define USE_BOOTROM 0
#define USE_LCD		1
#define LOG_LEVEL	INFO

#define FREQ		60
#define CYCLES_FREQ	(CLOCKSPEED/FREQ)

extern const char *log_level_str[10];

void draw_screen(void);
int boot_gameboy(char *filename);
void gb_update(void);
void gb_log(uint8_t level, char *format, ...);

#endif
