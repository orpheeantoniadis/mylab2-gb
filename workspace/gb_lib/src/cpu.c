#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "cpu.h"

registers_t registers;

void cycle(void) {
	uint8_t opcode;
	uint8_t n;
	uint16_t nn;

  opcode = memory.MEM[registers.pc];
  switch (instruction_set[opcode].operandLength) {
    case 0 :
			(registers.pc)++;
      ((void (*)(void))instruction_set[opcode].execute)();
      break;
    case 1 :
      n = memory.MEM[registers.pc+1];
			(registers.pc)+=2;
      if (opcode == 0xcb) ((void (*)(void))prefix_cb[n].execute)();
      else ((void (*)(uint8_t))instruction_set[opcode].execute)(n);
      break;
    case 2 :
      nn = memory.MEM[registers.pc+1] | (memory.MEM[registers.pc+2] << 8);
			(registers.pc)+=3;
      ((void (*)(uint16_t))instruction_set[opcode].execute)(nn);
      break;
    default : break;
  }
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

	opcode = memory.MEM[registers.pc];
	switch (instruction_set[opcode].operandLength) {
		case 0 :
			printf("0x%02x : %s (0x%02x)\n", registers.pc, instruction_set[opcode].disassembly, opcode);
			break;
		case 1 :
			n = memory.MEM[registers.pc+1];
			if (opcode == 0xcb) {
				printf("0x%02x : %s (0x%02x 0x%02x)\n", registers.pc, prefix_cb[n].disassembly, opcode, n);
			} else {
				sprintf(buffer, instruction_set[opcode].disassembly, n);
				printf("0x%02x : %s (0x%02x)\n", registers.pc, buffer, opcode);
			}
			break;
		case 2 :
			nn = memory.MEM[registers.pc+1] | (memory.MEM[registers.pc+2] << 8);
			sprintf(buffer, instruction_set[opcode].disassembly, nn);
			printf("0x%02x : %s (0x%02x)\n", registers.pc, buffer, opcode);
			break;
		default : break;
	}
}
