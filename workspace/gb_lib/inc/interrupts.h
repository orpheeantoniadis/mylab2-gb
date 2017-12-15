#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include <stdint.h>
#include "memory.h"

#define IR_VBLANK 0
#define IR_LCD    1
#define IR_TIMER  2
#define IR_SERIAL 3
#define IR_JOYPAD 4

#define IF  (memory.MEM[0xff0f - 0x9e00])
#define IE  (memory.MEM[0xffff - 0x9e00])

extern uint8_t interrupt_master;

void interrupt_request(uint8_t id);
void enable_interrupts(uint8_t wait_cycle);
void interrupts_cycle(void);

#endif
