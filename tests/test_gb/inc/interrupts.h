#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include <stdint.h>

#define IR_VBLANK	0
#define IR_LCD		1
#define IR_TIMER	2
#define IR_SERIAL	3
#define IR_JOYPAD	4

#define IF	0xff0f
#define IE	0xffff

extern uint8_t interrupt_master;

void interrupt_request(uint8_t id);
void interrupts_cycle(void);

#endif
