#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>
#include "memory.h"

#define CLOCKSPEED		4194304
#define FREQ0				4096
#define FREQ1				262144
#define FREQ2				65536
#define FREQ3				16384
#define TIMER_OVERFLOW	255

#define DIV		0xff04
#define TIMA	0xff05
#define TMA		0xff06
#define TAC		0xff07

#define TIMER_IS_ENABLE()	((read8(TAC) >> 2) & 1)

void timer_cycle(uint8_t cycles);

#endif
