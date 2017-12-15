#ifndef _COMMANDS_H
#define _COMMANDS_H

#include <stdint.h>

typedef struct {
  char *disassembly;
  uint8_t length;
  void *execute;
  uint8_t conditional_duration;
} instruction_t;

extern const instruction_t instruction_set[256];
extern const uint8_t op_times[256];
extern const uint8_t taken_op_times[256];
extern const instruction_t prefix_cb[256];
extern const uint8_t cb_op_times[256];

void print_instructions(uint8_t *opcodes, int size);

#endif