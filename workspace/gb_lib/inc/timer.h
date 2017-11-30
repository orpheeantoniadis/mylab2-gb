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

#define DIV   (memory.MEM[0xff04 - 0x9e00])
#define TIMA  (memory.MEM[0xff05 - 0x9e00])
#define TMA   (memory.MEM[0xff06 - 0x9e00])
#define TAC   (memory.MEM[0xff07 - 0x9e00])

#define TIMER_IS_ENABLE() ((TAC >> 2) & 1)

void timer_cycle(uint8_t cycles);

#endif
