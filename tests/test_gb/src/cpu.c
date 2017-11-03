#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "cpu.h"

registers_t registers;

uint8_t cpu_cycle(void) {
	uint8_t opcode;
	uint8_t n;
	uint16_t nn;
	uint8_t cycles = 0;

	opcode = read8(registers.pc);
	switch (instruction_set[opcode].length) {
		case 0 :
			(registers.pc)++;
			if (instruction_set[opcode].conditional_duration == 0) {
				((void (*)(void))instruction_set[opcode].execute)();
				cycles = instruction_set[opcode].duration;
			}
			else {
				cycles = ((uint8_t (*)(void))instruction_set[opcode].execute)();
			}
			break;
		case 1 :
			n = read8(registers.pc+1);
			(registers.pc)+=2;
			if (opcode == 0xcb) {
				((void (*)(void))prefix_cb[n].execute)();
				cycles = instruction_set[opcode].duration;
				cycles += prefix_cb[n].duration;
			}
			else {
				if (instruction_set[opcode].conditional_duration == 0) {
					((void (*)(uint8_t))instruction_set[opcode].execute)(n);
					cycles = instruction_set[opcode].duration;
				}
				else {
					cycles = ((uint8_t (*)(uint8_t))instruction_set[opcode].execute)(n);
				}
			}
			break;
		case 2 :
			nn = read16(registers.pc+1);
			(registers.pc)+=3;
			if (instruction_set[opcode].conditional_duration == 0) {
				((void (*)(uint16_t))instruction_set[opcode].execute)(nn);
				cycles = instruction_set[opcode].duration;
			}
			else {
				cycles = ((uint8_t (*)(uint16_t))instruction_set[opcode].execute)(nn);
			}
			break;
		default : break;
	}
	return cycles;
}

void print_registers(void) {
  printf("AF : 0x%04x\n"
         "BC : 0x%04x\n"
         "DE : 0x%04x\n"
         "HL : 0x%04x\n"
         "SP : 0x%04x\n"
         "PC : 0x%04x\n"
         "Z|N|H|C : %d|%d|%d|%d\n\n",
         registers.af, registers.bc,
         registers.de, registers.hl,
         registers.sp, registers.pc,
      	FLAG_ISSET(ZERO_FLAG) ? 1 : 0, FLAG_ISSET(NEGATIVE_FLAG) ? 1 : 0,
         FLAG_ISSET(HALFCARRY_FLAG) ? 1 : 0, FLAG_ISSET(CARRY_FLAG) ? 1 : 0);
}

void print_instruction(void) {
	char buffer[30];
	uint8_t opcode;
	uint8_t n;
	uint16_t nn;

	opcode = read8(registers.pc);
	switch (instruction_set[opcode].length) {
		case 0 :
			printf("0x%02x : %s (0x%02x)\n", registers.pc, instruction_set[opcode].disassembly, opcode);
			break;
		case 1 :
			n = read8(registers.pc+1);
			if (opcode == 0xcb) {
				printf("0x%02x : %s (0x%02x 0x%02x)\n", registers.pc, prefix_cb[n].disassembly, opcode, n);
			} else {
				sprintf(buffer, instruction_set[opcode].disassembly, n);
				printf("0x%02x : %s (0x%02x)\n", registers.pc, buffer, opcode);
			}
			break;
		case 2 :
			nn = read16(registers.pc+1);
			sprintf(buffer, instruction_set[opcode].disassembly, nn);
			printf("0x%02x : %s (0x%02x)\n", registers.pc, buffer, opcode);
			break;
		default : break;
	}
}
