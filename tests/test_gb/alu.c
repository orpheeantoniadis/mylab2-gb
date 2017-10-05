#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "memory.h"
#include "alu.h"

void inc(uint8_t *reg) {
  if (((*reg) & 0x0f) == 0x0f) FLAG_SET(HALFCARRY_FLAG);
  else FLAG_CLEAR(HALFCARRY_FLAG);

  (*reg)++;

  if ((*reg) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_CLEAR(NEGATIVE_FLAG);
}

void dec(uint8_t *reg) {
  if (((*reg) & 0x0f) == 0) FLAG_SET(HALFCARRY_FLAG);
  else FLAG_CLEAR(HALFCARRY_FLAG);

  (*reg)--;

  if ((*reg) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_SET(NEGATIVE_FLAG);
}

void add8(uint8_t *dst, uint8_t val) {
  if ((((*dst) & 0x0f) + (val & 0x0f)) > 0x0f) FLAG_SET(HALFCARRY_FLAG);
  else FLAG_CLEAR(HALFCARRY_FLAG);

  if ((0xff - (*dst)) < val) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);

  (*dst) += val;

  if ((*dst) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);
}

void add16(uint16_t *dst, uint16_t val) {
  if ((((*dst) & 0x0fff) + (val & 0x0fff)) > 0x0fff) FLAG_SET(HALFCARRY_FLAG);
  else FLAG_CLEAR(HALFCARRY_FLAG);

  if ((0xffff - (*dst)) < val) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);

  (*dst) += val;

  if ((*dst) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);
}
