#ifndef _CPU_H
#define _CPU_H

typedef struct {
	char *disassembly;
	unsigned char operandLength;
	void *execute;
} instruction;

extern const instruction instructions[256];

#endif
