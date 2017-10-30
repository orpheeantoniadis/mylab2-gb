#ifndef _COMMANDS_H
#define _COMMANDS_H

#include <stdint.h>

typedef struct {
	char *disassembly;
	uint8_t length;
	uint8_t duration;
	void *execute;
} instruction_t;

extern const instruction_t instruction_set[256];
extern const instruction_t prefix_cb[256];

void print_instructions(uint8_t *opcodes, int size);

#endif
