#ifndef _OPCODE_H
#define _OPCODE_H

typedef struct {
	char *disassembly;
	unsigned char operandLength;
	void *execute;
} instruction;

extern const instruction instructions[256];

#endif
