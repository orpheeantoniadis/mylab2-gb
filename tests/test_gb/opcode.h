#ifndef _OPCODE_H
#define _OPCODE_H

typedef struct {
	char *disassembly;
	unsigned char operandLength;
	void *execute;
} instruction_t;

extern const instruction_t instructions[256];

#endif
