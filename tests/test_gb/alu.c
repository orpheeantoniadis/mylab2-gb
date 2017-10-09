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

  FLAG_CLEAR(NEGATIVE_FLAG);
}

void add16(uint16_t *dst, uint16_t val) {
  if ((((*dst) & 0x0fff) + (val & 0x0fff)) > 0x0fff) FLAG_SET(HALFCARRY_FLAG);
  else FLAG_CLEAR(HALFCARRY_FLAG);

  if ((0xffff - (*dst)) < val) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);

  (*dst) += val;

  FLAG_CLEAR(NEGATIVE_FLAG);
}

void adc(uint8_t *dst, uint8_t val) {
  uint8_t carry = FLAG_ISSET(CARRY_FLAG) ? 1 : 0;

  if ((((*dst) & 0x0f) + (val & 0x0f) + carry) > 0x0f) FLAG_SET(HALFCARRY_FLAG);
  else FLAG_CLEAR(HALFCARRY_FLAG);

  if ((0xff - (*dst)) < (val + carry)) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);

  (*dst) += val + carry;

  if ((*dst) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_CLEAR(NEGATIVE_FLAG);
}

void sub(uint8_t *dst, uint8_t val) {
  if ((val & 0x0f) > ((*dst) & 0x0f)) FLAG_SET(HALFCARRY_FLAG);
  else FLAG_CLEAR(HALFCARRY_FLAG);

  if (val > (*dst)) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG)

  (*dst) -= val;

  if ((*dst) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_SET(NEGATIVE_FLAG);
}

void sbc(uint8_t *dst, uint8_t val) {
  uint8_t carry = FLAG_ISSET(CARRY_FLAG) ? 1 : 0;

  if (((val + carry) & 0x0f) > ((*dst) & 0x0f)) FLAG_SET(HALFCARRY_FLAG);
  else FLAG_CLEAR(HALFCARRY_FLAG);

  if ((val + carry) > (*dst)) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG)

  (*dst) -= (val + carry);

  if ((*dst) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_SET(NEGATIVE_FLAG);
}

void and(uint8_t *dst, uint8_t val) {
  (*dst) &= val;

  if ((*dst) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_SET(HALFCARRY_FLAG);
  FLAG_CLEAR(CARRY_FLAG);
}

void or(uint8_t *dst, uint8_t val) {
  (*dst) |= val;

  if ((*dst) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_CLEAR(HALFCARRY_FLAG);
  FLAG_CLEAR(CARRY_FLAG);
}

void xor(uint8_t *dst, uint8_t val) {
  (*dst) ^= val;

  if ((*dst) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_CLEAR(HALFCARRY_FLAG);
  FLAG_CLEAR(CARRY_FLAG);
}

void cp(uint8_t reg, uint8_t val) {
  if ((reg - val) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_SET(NEGATIVE_FLAG);

  if ((val & 0x0f) > (reg & 0x0f)) FLAG_SET(HALFCARRY_FLAG);
  else FLAG_CLEAR(HALFCARRY_FLAG);

  if (val > reg) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG)
}
