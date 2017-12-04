#ifndef _CPU_H
#define _CPU_H

#include <stdint.h>
#include "commands.h"
#include "alu.h"
#include "interrupts.h"

typedef struct {
  struct {
    union {
      uint16_t af;
      struct {
        uint8_t f;
        uint8_t a;
      };
    };
  };
  struct {
    union {
      uint16_t bc;
      struct {
        uint8_t c;
        uint8_t b;
      };
    };
  };
  struct {
    union {
      uint16_t de;
      struct {
        uint8_t e;
        uint8_t d;
      };
    };
  };
  struct {
    union {
      uint16_t hl;
      struct {
        uint8_t l;
        uint8_t h;
      };
    };
  };
  uint16_t sp;
  uint16_t pc;
} registers_t;
extern registers_t registers;
extern uint8_t halted;

#define ZERO_FLAG (1 << 7)
#define NEGATIVE_FLAG (1 << 6)
#define HALFCARRY_FLAG (1 << 5)
#define CARRY_FLAG (1 << 4)

#define FLAG_ISSET(x) (registers.f & (x))
#define FLAG_SET(x) (registers.f |= (x))
#define FLAG_CLEAR(x) (registers.f &= ~(x))

uint8_t cpu_cycle(void);
void print_registers(void);
void print_instruction(void);

#endif
