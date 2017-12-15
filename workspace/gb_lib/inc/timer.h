#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>
#include "memory.h"

#define CLOCKSPEED      4194304
#define FREQ0           4096
#define FREQ1           262144
#define FREQ2           65536
#define FREQ3           16384
#define TIMER_OVERFLOW  255

#define TIMER_IS_ENABLE() ((TAC >> 2) & 1)

void timer_cycle(uint8_t cycles);

#endif
