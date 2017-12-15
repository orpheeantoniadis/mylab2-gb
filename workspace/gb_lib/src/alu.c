#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "memory.h"
#include "alu.h"

static void inc(uint8_t *reg) {
  if (((*reg) & 0x0f) == 0x0f) FLAG_SET(HALFCARRY_FLAG);
  else FLAG_CLEAR(HALFCARRY_FLAG);

  (*reg)++;

  if ((*reg) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_CLEAR(NEGATIVE_FLAG);
}

static void dec(uint8_t *reg) {
  if (((*reg) & 0x0f) == 0) FLAG_SET(HALFCARRY_FLAG);
  else FLAG_CLEAR(HALFCARRY_FLAG);

  (*reg)--;

  if ((*reg) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_SET(NEGATIVE_FLAG);
}

static void add8(uint8_t *dst, uint8_t val) {
  if ((((*dst) & 0x0f) + (val & 0x0f)) > 0x0f) FLAG_SET(HALFCARRY_FLAG);
  else FLAG_CLEAR(HALFCARRY_FLAG);

  if ((0xff - (*dst)) < val) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);

  (*dst) += val;

  if ((*dst) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_CLEAR(NEGATIVE_FLAG);
}

static void add16(uint16_t *dst, uint16_t val) {
  if ((((*dst) & 0x0fff) + (val & 0x0fff)) > 0x0fff) FLAG_SET(HALFCARRY_FLAG);
  else FLAG_CLEAR(HALFCARRY_FLAG);

  if ((0xffff - (*dst)) < val) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);

  (*dst) += val;

  FLAG_CLEAR(NEGATIVE_FLAG);
}

static void add_sp(uint16_t *dst, int8_t val) {
  FLAG_CLEAR(ZERO_FLAG);
  FLAG_CLEAR(NEGATIVE_FLAG);

  if (((((*dst) & 0x0f) + (val & 0x0f)) & 0x10) != 0) FLAG_SET(HALFCARRY_FLAG);
  else FLAG_CLEAR(HALFCARRY_FLAG);

  if (((((*dst) & 0xff) + (val & 0xff)) & 0x100) != 0) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);

  (*dst) += val;
}

static void adc(uint8_t *dst, uint8_t val) {
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

static void sub(uint8_t *dst, uint8_t val) {
  if ((val & 0x0f) > ((*dst) & 0x0f)) FLAG_SET(HALFCARRY_FLAG);
  else FLAG_CLEAR(HALFCARRY_FLAG);

  if (val > (*dst)) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);

  (*dst) -= val;

  if ((*dst) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_SET(NEGATIVE_FLAG);
}

static void sbc(uint8_t *dst, uint8_t val) {
  uint8_t carry = FLAG_ISSET(CARRY_FLAG) ? 1 : 0;

  if ((((*dst)^val^(((*dst)-val-carry)&0xff))&(1<<4)) != 0) FLAG_SET(HALFCARRY_FLAG);
  else FLAG_CLEAR(HALFCARRY_FLAG);	

  if ((val + carry) > (*dst)) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);

  (*dst) -= (val + carry);

  if ((*dst) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_SET(NEGATIVE_FLAG);
}

static void and(uint8_t * dst, uint8_t val) {
  (*dst) &= val;

  if ((*dst) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_SET(HALFCARRY_FLAG);
  FLAG_CLEAR(CARRY_FLAG);
}

static void or(uint8_t * dst, uint8_t val) {
  (*dst) |= val;

  if ((*dst) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_CLEAR(HALFCARRY_FLAG);
  FLAG_CLEAR(CARRY_FLAG);
}

static void xor(uint8_t * dst, uint8_t val) {
  (*dst) ^= val;

  if ((*dst) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_CLEAR(HALFCARRY_FLAG);
  FLAG_CLEAR(CARRY_FLAG);
}

static void cp(uint8_t reg, uint8_t val) {
  if ((reg - val) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_SET(NEGATIVE_FLAG);

  if ((val & 0x0f) > (reg & 0x0f)) FLAG_SET(HALFCARRY_FLAG);
  else FLAG_CLEAR(HALFCARRY_FLAG);

  if (val > reg) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);
}

void push(uint16_t val) {
  registers.sp -= 2;
  write16(registers.sp, val);
}

void pop(uint16_t *dst) {
  (*dst) = read16(registers.sp);
  registers.sp += 2;
}

static void rlc(uint8_t *reg) {
  uint8_t carry = ((*reg) >> 7) & 1;
  (*reg) = ((*reg) << 1) | carry;
  if ((*reg) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);
  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_CLEAR(HALFCARRY_FLAG);
  if (carry) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);
}

static void rrc(uint8_t *reg) {
  uint8_t carry = (*reg) & 1;
  (*reg) = ((*reg) >> 1) | (carry << 7);
  if ((*reg) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);
  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_CLEAR(HALFCARRY_FLAG);
  if (carry) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);
}

static void rl(uint8_t *reg) {
  uint8_t bit7 = ((*reg) >> 7) & 1;
  (*reg) = ((*reg) << 1) | (FLAG_ISSET(CARRY_FLAG) ? 1 : 0);
  if ((*reg) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);
  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_CLEAR(HALFCARRY_FLAG);
  if (bit7) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);
}

static void rr(uint8_t *reg) {
  uint8_t bit0 = (*reg) & 1;
  (*reg) = ((*reg) >> 1) | ((FLAG_ISSET(CARRY_FLAG) ? 1 : 0) << 7);
  if ((*reg) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);
  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_CLEAR(HALFCARRY_FLAG);
  if (bit0) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);
}

static void sla(uint8_t *reg) {
  uint8_t carry = (*reg) >> 7;
  (*reg) = ((*reg) << 1);
  if ((*reg) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);
  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_CLEAR(HALFCARRY_FLAG);
  if (carry) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);
}

static void sra(uint8_t *reg) {
  uint8_t carry = (*reg) & 1;
  (*reg) = ((*reg) & (1 << 7)) | (*reg) >> 1;
  if ((*reg) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);
  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_CLEAR(HALFCARRY_FLAG);
  if (carry) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);
}

static void swap(uint8_t *reg) {
  (*reg) = (((*reg) & 0xf) << 4) | (((*reg) & 0xf0) >> 4);
  if ((*reg) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);
  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_CLEAR(HALFCARRY_FLAG);
  FLAG_CLEAR(CARRY_FLAG);
}

static void srl(uint8_t *reg) {
  uint8_t carry = (*reg) & 1;
  (*reg) = (*reg) >> 1;
  if ((*reg) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);
  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_CLEAR(HALFCARRY_FLAG);
  if (carry) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);
}

static void bit(uint8_t reg, uint8_t bit) {
  if ((reg & (1 << bit)) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);
  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_SET(HALFCARRY_FLAG);
}

static void set(uint8_t *reg, uint8_t bit) { (*reg) |= (1 << bit); }

static void res(uint8_t *reg, uint8_t bit) { (*reg) &= ~(1 << bit); }

/**** instruction set ****/

// 0x00
void nop(void) {}

// 0x01
void ld_bc_nn(uint16_t nn) { registers.bc = nn; }

// 0x02
void ld_bcp_a(void) { write8(registers.bc, registers.a); }

// 0x03
void inc_bc(void) { registers.bc++; }

// 0x04
void inc_b(void) { inc(&(registers.b)); }

// 0x05
void dec_b(void) { dec(&(registers.b)); }

// 0x06
void ld_b_n(uint8_t n) { registers.b = n; }

// 0x07
void rlca(void) {
  uint8_t carry = registers.a >> 7;
  registers.a = (registers.a << 1) | carry;
	FLAG_CLEAR(ZERO_FLAG);
  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_CLEAR(HALFCARRY_FLAG);
  if (carry) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);
}

// 0x08
void ld_nnp_sp(uint16_t nn) { write16(nn, registers.sp); }

// 0x09
void add_hl_bc(void) { add16(&(registers.hl), registers.bc); }

// 0x0a
void ld_a_bcp(void) { registers.a = read8(registers.bc); }

// 0x0b
void dec_bc(void) { registers.bc--; }

// 0x0c
void inc_c(void) { inc(&(registers.c)); }

// 0x0d
void dec_c(void) { dec(&(registers.c)); }

// 0x0e
void ld_c_n(uint8_t n) { registers.c = n; }

// 0x0f
void rrca(void) {
  uint8_t carry = registers.a & 1;
  registers.a = (registers.a >> 1) | (carry << 7);
	FLAG_CLEAR(ZERO_FLAG);
  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_CLEAR(HALFCARRY_FLAG);
  if (carry) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);
}

// 0x10
void stop(void) { fprintf(stderr, "stop\n"); }

// 0x11
void ld_de_nn(uint16_t nn) { registers.de = nn; }

// 0x12
void ld_dep_a(void) { write8(registers.de, registers.a); }

// 0x13
void inc_de(void) { registers.de++; }

// 0x14
void inc_d(void) { inc(&(registers.d)); }

// 0x15
void dec_d(void) { dec(&(registers.d)); }

// 0x16
void ld_d_n(uint8_t n) { registers.d = n; }

// 0x17
void rla(void) {
  uint8_t old_carry = registers.a >> 7;
  registers.a = (registers.a << 1) | (FLAG_ISSET(CARRY_FLAG) ? 1 : 0);
	FLAG_CLEAR(ZERO_FLAG);
  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_CLEAR(HALFCARRY_FLAG);
  if (old_carry) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);
}

// 0x18
void jr_n(uint8_t n) { registers.pc += (int8_t)n; }

// 0x19
void add_hl_de(void) { add16(&(registers.hl), registers.de); }

// 0x1a
void ld_a_dep(void) { registers.a = read8(registers.de); }

// 0x1b
void dec_de(void) { registers.de--; }

// 0x1c
void inc_e(void) { inc(&(registers.e)); }

// 0x1d
void dec_e(void) { dec(&(registers.e)); }

// 0x1e
void ld_e_n(uint8_t n) { registers.e = n; }

// 0x1f
void rra(void) {
  uint8_t old_carry = registers.a & 1;
  registers.a = (registers.a >> 1) | ((FLAG_ISSET(CARRY_FLAG) ? 1 : 0) << 7);
	FLAG_CLEAR(ZERO_FLAG);
  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_CLEAR(HALFCARRY_FLAG);
  if (old_carry) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);
}

// 0x20
uint8_t jr_nz(uint8_t n) {
  if (!FLAG_ISSET(ZERO_FLAG)) {
    registers.pc += (int8_t)n;
    return taken_op_times[0x20];
  }
  return op_times[0x20];
}

// 0x21
void ld_hl_nn(uint16_t nn) { registers.hl = nn; }

// 0x22
void ldi_hlp_a(void) {
  write8(registers.hl, registers.a);
  registers.hl++;
}

// 0x23
void inc_hl(void) { registers.hl++; }

// 0x24
void inc_h(void) { inc(&(registers.h)); }

// 0x25
void dec_h(void) { dec(&(registers.h)); }

// 0x26
void ld_h_n(uint8_t n) { registers.h = n; }

// 0x27
void daa(void) {
  int result = registers.a;

  if (FLAG_ISSET(NEGATIVE_FLAG)) {
    if (FLAG_ISSET(HALFCARRY_FLAG)) result = (result - 6) & 0xff;
    if (FLAG_ISSET(CARRY_FLAG)) result = (result - 0x60) & 0xff;
  } else {
    if (FLAG_ISSET(HALFCARRY_FLAG) || (result & 0xf) > 9) result += 6;
    if (FLAG_ISSET(CARRY_FLAG) || result > 0x9f) result += 0x60;
  }
  FLAG_CLEAR(HALFCARRY_FLAG);
	if (result > 0xff) FLAG_SET(CARRY_FLAG);
	result &= 0xff;
  if (result == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  registers.a = result;
}

// 0x28
uint8_t jr_z(uint8_t n) {
  if (FLAG_ISSET(ZERO_FLAG)) {
    registers.pc += (int8_t)n;
		return taken_op_times[0x28];
  }
  return op_times[0x28];
}

// 0x29
void add_hl_hl(void) { add16(&(registers.hl), registers.hl); }

// 0x2a
void ldi_a_hlp(void) {
  registers.a = read8(registers.hl);
  registers.hl++;
}

// 0x2b
void dec_hl(void) { registers.hl--; }

// 0x2c
void inc_l(void) { inc(&(registers.l)); }

// 0x2d
void dec_l(void) { dec(&(registers.l)); }

// 0x2e
void ld_l_n(uint8_t n) { registers.l = n; }

// 0x2f
void cpl(void) {
  registers.a = ~(registers.a);
  FLAG_SET(NEGATIVE_FLAG);
  FLAG_SET(HALFCARRY_FLAG);
}

// 0x30
uint8_t jr_nc(uint8_t n) {
  if (!FLAG_ISSET(CARRY_FLAG)) {
    registers.pc += (int8_t)n;
		return taken_op_times[0x30];
  }
  return op_times[0x30];
}

// 0x31
void ld_sp_nn(uint16_t nn) { registers.sp = nn; }

// 0x32
void ldd_hlp_a(void) {
  write8(registers.hl, registers.a);
  registers.hl--;
}

// 0x33
void inc_sp(void) { registers.sp++; }

// 0x34
void inc_hlp(void) {
  uint8_t temp = read8(registers.hl);
  inc(&temp);
  write8(registers.hl, temp);
}

// 0x35
void dec_hlp(void) {
  uint8_t temp = read8(registers.hl);
  dec(&temp);
  write8(registers.hl, temp);
}

// 0x36
void ld_hlp_n(uint8_t n) { write8(registers.hl, n); }

// 0x37
void scf(void) {
  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_CLEAR(HALFCARRY_FLAG);
  FLAG_SET(CARRY_FLAG);
}

// 0x38
uint8_t jr_c(uint8_t n) {
  if (FLAG_ISSET(CARRY_FLAG)) {
    registers.pc += (int8_t)n;
		return taken_op_times[0x38];
  }
  return op_times[0x38];
}

// 0x39
void add_hl_sp(void) { add16(&(registers.hl), registers.sp); }

// 0x3a
void ldd_a_hlp(void) {
  registers.a = read8(registers.hl);
  registers.hl--;
}

// 0x3b
void dec_sp(void) { registers.sp--; }

// 0x3c
void inc_a(void) { inc(&(registers.a)); }

// 0x3d
void dec_a(void) { dec(&(registers.a)); }

// 0x3e
void ld_a_n(uint8_t n) { registers.a = n; }

// 0x3f
void cff(void) {
  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_CLEAR(HALFCARRY_FLAG);
  if (FLAG_ISSET(CARRY_FLAG)) FLAG_CLEAR(CARRY_FLAG);
  else FLAG_SET(CARRY_FLAG);
}

// 0x40
void ld_b_b(void) { registers.b = registers.b; };

// 0x41
void ld_b_c(void) { registers.b = registers.c; }

// 0x42
void ld_b_d(void) { registers.b = registers.d; }

// 0x43
void ld_b_e(void) { registers.b = registers.e; }

// 0x44
void ld_b_h(void) { registers.b = registers.h; }

// 0x45
void ld_b_l(void) { registers.b = registers.l; }

// 0x46
void ld_b_hlp(void) { registers.b = read8(registers.hl); }

// 0x47
void ld_b_a(void) { registers.b = registers.a; }

// 0x48
void ld_c_b(void) { registers.c = registers.b; }

// 0x49
void ld_c_c(void) { registers.c = registers.c; };

// 0x4a
void ld_c_d(void) { registers.c = registers.d; }

// 0x4b
void ld_c_e(void) { registers.c = registers.e; }

// 0x4c
void ld_c_h(void) { registers.c = registers.h; }

// 0x4d
void ld_c_l(void) { registers.c = registers.l; }

// 0x4e
void ld_c_hlp(void) { registers.c = read8(registers.hl); }

// 0x4f
void ld_c_a(void) { registers.c = registers.a; }

// 0x50
void ld_d_b(void) { registers.d = registers.b; }

// 0x51
void ld_d_c(void) { registers.d = registers.c; }

// 0x52
void ld_d_d(void) { registers.d = registers.d; };

// 0x53
void ld_d_e(void) { registers.d = registers.e; }

// 0x54
void ld_d_h(void) { registers.d = registers.h; }

// 0x55
void ld_d_l(void) { registers.d = registers.l; }

// 0x56
void ld_d_hlp(void) { registers.d = read8(registers.hl); }

// 0x57
void ld_d_a(void) { registers.d = registers.a; }

// 0x58
void ld_e_b(void) { registers.e = registers.b; }

// 0x59
void ld_e_c(void) { registers.e = registers.c; }

// 0x5a
void ld_e_d(void) { registers.e = registers.d; }

// 0x5b
void ld_e_e(void) { registers.e = registers.e; };

// 0x5c
void ld_e_h(void) { registers.e = registers.h; }

// 0x5d
void ld_e_l(void) { registers.e = registers.l; }

// 0x5e
void ld_e_hlp(void) { registers.e = read8(registers.hl); }

// 0x5f
void ld_e_a(void) { registers.e = registers.a; }

// 0x60
void ld_h_b(void) { registers.h = registers.b; }

// 0x61
void ld_h_c(void) { registers.h = registers.c; }

// 0x62
void ld_h_d(void) { registers.h = registers.d; }

// 0x63
void ld_h_e(void) { registers.h = registers.e; }

// 0x64
void ld_h_h(void) { registers.h = registers.h; };

// 0x65
void ld_h_l(void) { registers.h = registers.l; }

// 0x66
void ld_h_hlp(void) { registers.h = read8(registers.hl); }

// 0x67
void ld_h_a(void) { registers.h = registers.a; }

// 0x68
void ld_l_b(void) { registers.l = registers.b; }

// 0x69
void ld_l_c(void) { registers.l = registers.c; }

// 0x6a
void ld_l_d(void) { registers.l = registers.d; }

// 0x6b
void ld_l_e(void) { registers.l = registers.e; }

// 0x6c
void ld_l_h(void) { registers.l = registers.h; }

// 0x6d
void ld_l_l(void) { registers.b = registers.b; };

// 0x6e
void ld_l_hlp(void) { registers.l = read8(registers.hl); }

// 0x6f
void ld_l_a(void) { registers.l = registers.a; }

// 0x70
void ld_hlp_b(void) { write8(registers.hl, registers.b); }

// 0x71
void ld_hlp_c(void) { write8(registers.hl, registers.c); }

// 0x72
void ld_hlp_d(void) { write8(registers.hl, registers.d); }

// 0x73
void ld_hlp_e(void) { write8(registers.hl, registers.e); }

// 0x74
void ld_hlp_h(void) { write8(registers.hl, registers.h); }

// 0x75
void ld_hlp_l(void) { write8(registers.hl, registers.l); }

// 0x76
void halt(void) {
	if (interrupt_master == 0 && ((IE & IF & 0x1f) != 0)) halted = 2;
	else halted = 1;
}

// 0x77
void ld_hlp_a(void) { write8(registers.hl, registers.a); }

// 0x78
void ld_a_b(void) { registers.a = registers.b; }

// 0x79
void ld_a_c(void) { registers.a = registers.c; }

// 0x7a
void ld_a_d(void) { registers.a = registers.d; }

// 0x7b
void ld_a_e(void) { registers.a = registers.e; }

// 0x7c
void ld_a_h(void) { registers.a = registers.h; }

// 0x7d
void ld_a_l(void) { registers.a = registers.l; }

// 0x7e
void ld_a_hlp(void) { registers.a = read8(registers.hl); }

// 0x7f
void ld_a_a(void) { registers.a = registers.a; }

// 0x80
void add_a_b(void) { add8(&(registers.a), registers.b); }

// 0x81
void add_a_c(void) { add8(&(registers.a), registers.c); }

// 0x82
void add_a_d(void) { add8(&(registers.a), registers.d); }

// 0x83
void add_a_e(void) { add8(&(registers.a), registers.e); }

// 0x84
void add_a_h(void) { add8(&(registers.a), registers.h); }

// 0x85
void add_a_l(void) { add8(&(registers.a), registers.l); }

// 0x86
void add_a_hlp(void) { add8(&(registers.a), read8(registers.hl)); }

// 0x87
void add_a_a(void) { add8(&(registers.a), registers.a); }

// 0x88
void adc_b(void) { adc(&(registers.a), registers.b); }

// 0x89
void adc_c(void) { adc(&(registers.a), registers.c); }

// 0x8a
void adc_d(void) { adc(&(registers.a), registers.d); }

// 0x8b
void adc_e(void) { adc(&(registers.a), registers.e); }

// 0x8c
void adc_h(void) { adc(&(registers.a), registers.h); }

// 0x8d
void adc_l(void) { adc(&(registers.a), registers.l); }

// 0x8e
void adc_hlp(void) { adc(&(registers.a), read8(registers.hl)); }

// 0x8f
void adc_a(void) { adc(&(registers.a), registers.a); }

// 0x90
void sub_b(void) { sub(&(registers.a), registers.b); }

// 0x91
void sub_c(void) { sub(&(registers.a), registers.c); }

// 0x92
void sub_d(void) { sub(&(registers.a), registers.d); }

// 0x93
void sub_e(void) { sub(&(registers.a), registers.e); }

// 0x94
void sub_h(void) { sub(&(registers.a), registers.h); }

// 0x95
void sub_l(void) { sub(&(registers.a), registers.l); }

// 0x96
void sub_hlp(void) { sub(&(registers.a), read8(registers.hl)); }

// 0x97
void sub_a(void) { sub(&(registers.a), registers.a); }

// 0x98
void sbc_b(void) { sbc(&(registers.a), registers.b); }

// 0x99
void sbc_c(void) { sbc(&(registers.a), registers.c); }

// 0x9a
void sbc_d(void) { sbc(&(registers.a), registers.d); }

// 0x9b
void sbc_e(void) { sbc(&(registers.a), registers.e); }

// 0x9c
void sbc_h(void) { sbc(&(registers.a), registers.h); }

// 0x9d
void sbc_l(void) { sbc(&(registers.a), registers.l); }

// 0x9e
void sbc_hlp(void) { sbc(&(registers.a), read8(registers.hl)); }

// 0x9f
void sbc_a(void) { sbc(&(registers.a), registers.a); }

// 0xa0
void and_b(void) { and(&(registers.a), registers.b); }

// 0xa1
void and_c(void) { and(&(registers.a), registers.c); }

// 0xa2
void and_d(void) { and(&(registers.a), registers.d); }

// 0xa3
void and_e(void) { and(&(registers.a), registers.e); }

// 0xa4
void and_h(void) { and(&(registers.a), registers.h); }

// 0xa5
void and_l(void) { and(&(registers.a), registers.l); }

// 0xa6
void and_hlp(void) { and(&(registers.a), read8(registers.hl)); }

// 0xa7
void and_a(void) { and(&(registers.a), registers.a); }

// 0xa8
void xor_b(void) { xor(&(registers.a), registers.b); }

// 0xa9
void xor_c(void) { xor(&(registers.a), registers.c); }

// 0xaa
void xor_d(void) { xor(&(registers.a), registers.d); }

// 0xab
void xor_e(void) { xor(&(registers.a), registers.e); }

// 0xac
void xor_h(void) { xor(&(registers.a), registers.h); }

// 0xad
void xor_l(void) { xor(&(registers.a), registers.l); }

// 0xae
void xor_hlp(void) { xor(&(registers.a), read8(registers.hl)); }

// 0xaf
void xor_a(void) { xor(&(registers.a), registers.a); }

// 0xb0
void or_b(void) { or (&(registers.a), registers.b); }

// 0xb1
void or_c(void) { or (&(registers.a), registers.c); }

// 0xb2
void or_d(void) { or (&(registers.a), registers.d); }

// 0xb3
void or_e(void) { or (&(registers.a), registers.e); }

// 0xb4
void or_h(void) { or (&(registers.a), registers.h); }

// 0xb5
void or_l(void) { or (&(registers.a), registers.l); }

// 0xb6
void or_hlp(void) { or (&(registers.a), read8(registers.hl)); }

// 0xb7
void or_a(void) { or (&(registers.a), registers.a); }

// 0xb8
void cp_b(void) { cp(registers.a, registers.b); }

// 0xb9
void cp_c(void) { cp(registers.a, registers.c); }

// 0xba
void cp_d(void) { cp(registers.a, registers.d); }

// 0xbb
void cp_e(void) { cp(registers.a, registers.e); }

// 0xbc
void cp_h(void) { cp(registers.a, registers.h); }

// 0xbd
void cp_l(void) { cp(registers.a, registers.l); }

// 0xbe
void cp_hlp(void) { cp(registers.a, read8(registers.hl)); }

// 0xbf
void cp_a(void) { cp(registers.a, registers.a); }

// 0xc0
uint8_t ret_nz(void) {
  if (!FLAG_ISSET(ZERO_FLAG)) {
    pop(&(registers.pc));
		return taken_op_times[0xc0];
  }
  return op_times[0xc0];
}

// 0xc1
void pop_bc(void) { pop(&(registers.bc)); }

// 0xc2
uint8_t jp_nz(uint16_t nn) {
  if (!FLAG_ISSET(ZERO_FLAG)) {
    registers.pc = nn;
		return taken_op_times[0xc2];
  }
  return op_times[0xc2];
}

// 0xc3
void jp(uint16_t nn) { registers.pc = nn; }

// 0xc4
uint8_t call_nz(uint16_t nn) {
  if (!FLAG_ISSET(ZERO_FLAG)) {
    push(registers.pc);
    registers.pc = nn;
		return taken_op_times[0xc4];
  }
  return op_times[0xc4];
}

// 0xc5
void push_bc(void) { push(registers.bc); }

// 0xc6
void add_a_n(uint8_t n) { add8(&(registers.a), n); }

// 0xc7
void rst_00h(void) {
  push(registers.pc);
  registers.pc = 0x00;
}

// 0xc8
uint8_t ret_z(void) {
  if (FLAG_ISSET(ZERO_FLAG)) {
    pop(&(registers.pc));
		return taken_op_times[0xc8];
  }
  return op_times[0xc8];
}

// 0xc9
void ret(void) { pop(&(registers.pc)); }

// 0xca
uint8_t jp_z(uint16_t nn) {
  if (FLAG_ISSET(ZERO_FLAG)) {
    registers.pc = nn;
		return taken_op_times[0xca];
  }
  return op_times[0xca];
}

// 0xcc
uint8_t call_z(uint16_t nn) {
  if (FLAG_ISSET(ZERO_FLAG)) {
    push(registers.pc);
    registers.pc = nn;
		return taken_op_times[0xcc];
  }
  return op_times[0xcc];
}

// 0xcd
void call(uint16_t nn) {
  push(registers.pc);
  registers.pc = nn;
}

// 0xce
void adc_n(uint8_t n) { adc(&(registers.a), n); }

// 0xcf
void rst_08h(void) {
  push(registers.pc);
  registers.pc = 0x08;
}

// 0xd0
uint8_t ret_nc(void) {
  if (!FLAG_ISSET(CARRY_FLAG)) {
    pop(&(registers.pc));
		return taken_op_times[0xd0];
  }
  return op_times[0xd0];
}

// 0xd1
void pop_de(void) { pop(&(registers.de)); }

// 0xd2
uint8_t jp_nc(uint16_t nn) {
  if (!FLAG_ISSET(CARRY_FLAG)) {
    registers.pc = nn;
		return taken_op_times[0xd2];
  }
  return op_times[0xd2];
}

// 0xd4
uint8_t call_nc(uint16_t nn) {
  if (!FLAG_ISSET(CARRY_FLAG)) {
    push(registers.pc);
    registers.pc = nn;
		return taken_op_times[0xd4];
  }
  return op_times[0xd4];
}

// 0xd5
void push_de(void) { push(registers.de); }

// 0xd6
void sub_n(uint8_t n) { sub(&(registers.a), n); }

// 0xd7
void rst_10h(void) {
  push(registers.pc);
  registers.pc = 0x10;
}

// 0xd8
uint8_t ret_c(void) {
  if (FLAG_ISSET(CARRY_FLAG)) {
    pop(&(registers.pc));
		return taken_op_times[0xd8];
  }
  return op_times[0xd8];
}

// 0xd9
void reti(void) {
	enable_interrupts(0);
  pop(&(registers.pc));
}

// 0xda
uint8_t jp_c(uint16_t nn) {
  if (FLAG_ISSET(CARRY_FLAG)) {
    registers.pc = nn;
		return taken_op_times[0xda];
  }
  return op_times[0xda];
}

// 0xdc
uint8_t call_c(uint16_t nn) {
  if (FLAG_ISSET(CARRY_FLAG)) {
    push(registers.pc);
    registers.pc = nn;
		return taken_op_times[0xdc];
  }
  return op_times[0xdc];
}

// 0xde
void sbc_n(uint8_t n) { sbc(&(registers.a), n); }

// 0xdf
void rst_18h(void) {
  push(registers.pc);
  registers.pc = 0x18;
}

// 0xe0
void ldh_np_a(uint8_t n) { write8(0xff00 + n, registers.a); }

// 0xe1
void pop_hl(void) { pop(&(registers.hl)); }

// 0xe2
void ldh_cp_a(void) { write8(0xff00 + registers.c, registers.a); }

// 0xe5
void push_hl(void) { push(registers.hl); }

// 0xe6
void and_n(uint8_t n) { and(&(registers.a), n); }

// 0xe7
void rst_20h(void) {
  push(registers.pc);
  registers.pc = 0x20;
}

// 0xe8
void add_sp_n(uint8_t n) {
	add_sp(&(registers.sp), n);
}

// 0xe9
void jp_hl(void) { registers.pc = registers.hl; }

// 0xea
void ld_nnp_a(uint16_t nn) { write8(nn, registers.a); }

// 0xee
void xor_n(uint8_t n) { xor(&(registers.a), n); }

// 0xef
void rst_28h(void) {
  push(registers.pc);
  registers.pc = 0x28;
}

// 0xf0
void ldh_a_np(uint8_t n) { registers.a = read8(0xff00 + n); }

// 0xf1
void pop_af(void) {
	registers.af = read16(registers.sp) & ~0xf;
  registers.sp += 2;
}

// 0xf2
void ldh_a_cp(void) { registers.a = read8(0xff00 + registers.c); }

// 0xf3
void di(void) { 
	interrupt_master = 0;
}

// 0xf5
void push_af(void) { push(registers.af); }

// 0xf6
void or_n(uint8_t n) { or (&(registers.a), n); }

// 0xf7
void rst_30h(void) {
  push(registers.pc);
  registers.pc = 0x30;
}

// 0xf8
void ldhl_sp_n(uint8_t n) {
	uint16_t temp = registers.sp;
	add_sp(&(temp), n);
	registers.hl = temp;
}

// 0xf9
void ld_sp_hl(void) { registers.sp = registers.hl; }

// 0xfa
void ld_a_nnp(uint16_t nn) { registers.a = read8(nn); }

// 0xfb
void ei(void) {
	enable_interrupts(1);
}

// 0xfe
void cp_n(uint8_t n) { cp(registers.a, n); }

// 0xff
void rst_38h(void) {
  push(registers.pc);
  registers.pc = 0x38;
}

/**** prefix cb ****/

// 0x00
void rlc_b(void) { rlc(&(registers.b)); }

// 0x01
void rlc_c(void) { rlc(&(registers.c)); }

// 0x02
void rlc_d(void) { rlc(&(registers.d)); }

// 0x03
void rlc_e(void) { rlc(&(registers.e)); }

// 0x04
void rlc_h(void) { rlc(&(registers.h)); }

// 0x05
void rlc_l(void) { rlc(&(registers.l)); }

// 0x06
void rlc_hlp(void) {
  uint8_t temp = read8(registers.hl);
  rlc(&temp);
  write8(registers.hl, temp);
}

// 0x07
void rlc_a(void) { rlc(&(registers.a)); }

// 0x08
void rrc_b(void) { rrc(&(registers.b)); }

// 0x09
void rrc_c(void) { rrc(&(registers.c)); }

// 0x0a
void rrc_d(void) { rrc(&(registers.d)); }

// 0x0b
void rrc_e(void) { rrc(&(registers.e)); }

// 0x0c
void rrc_h(void) { rrc(&(registers.h)); }

// 0x0d
void rrc_l(void) { rrc(&(registers.l)); }

// 0x0e
void rrc_hlp(void) {
  uint8_t temp = read8(registers.hl);
  rrc(&temp);
  write8(registers.hl, temp);
}

// 0x0f
void rrc_a(void) { rrc(&(registers.a)); }

// 0x10
void rl_b(void) { rl(&(registers.b)); }

// 0x11
void rl_c(void) { rl(&(registers.c)); }

// 0x12
void rl_d(void) { rl(&(registers.d)); }

// 0x13
void rl_e(void) { rl(&(registers.e)); }

// 0x14
void rl_h(void) { rl(&(registers.h)); }

// 0x15
void rl_l(void) { rl(&(registers.l)); }

// 0x16
void rl_hlp(void) {
  uint8_t temp = read8(registers.hl);
  rl(&temp);
  write8(registers.hl, temp);
}

// 0x17
void rl_a(void) { rl(&(registers.a)); }

// 0x18
void rr_b(void) { rr(&(registers.b)); }

// 0x19
void rr_c(void) { rr(&(registers.c)); }

// 0x1a
void rr_d(void) { rr(&(registers.d)); }

// 0x1b
void rr_e(void) { rr(&(registers.e)); }

// 0x1c
void rr_h(void) { rr(&(registers.h)); }

// 0x1d
void rr_l(void) { rr(&(registers.l)); }

// 0x1e
void rr_hlp(void) {
  uint8_t temp = read8(registers.hl);
  rr(&temp);
  write8(registers.hl, temp);
}

// 0x1f
void rr_a(void) { rr(&(registers.a)); }

// 0x20
void sla_b(void) { sla(&(registers.b)); }

// 0x21
void sla_c(void) { sla(&(registers.c)); }

// 0x22
void sla_d(void) { sla(&(registers.d)); }

// 0x23
void sla_e(void) { sla(&(registers.e)); }

// 0x24
void sla_h(void) { sla(&(registers.h)); }

// 0x25
void sla_l(void) { sla(&(registers.l)); }

// 0x26
void sla_hlp(void) {
  uint8_t temp = read8(registers.hl);
  sla(&temp);
  write8(registers.hl, temp);
}

// 0x27
void sla_a(void) { sla(&(registers.a)); }

// 0x28
void sra_b(void) { sra(&(registers.b)); }

// 0x29
void sra_c(void) { sra(&(registers.c)); }

// 0x2a
void sra_d(void) { sra(&(registers.d)); }

// 0x2b
void sra_e(void) { sra(&(registers.e)); }

// 0x2c
void sra_h(void) { sra(&(registers.h)); }

// 0x2d
void sra_l(void) { sra(&(registers.l)); }

// 0x2e
void sra_hlp(void) {
  uint8_t temp = read8(registers.hl);
  sra(&temp);
  write8(registers.hl, temp);
}

// 0x2f
void sra_a(void) { sra(&(registers.a)); }

// 0x30
void swap_b(void) { swap(&(registers.b)); }

// 0x31
void swap_c(void) { swap(&(registers.c)); }

// 0x32
void swap_d(void) { swap(&(registers.d)); }

// 0x33
void swap_e(void) { swap(&(registers.e)); }

// 0x34
void swap_h(void) { swap(&(registers.h)); }

// 0x35
void swap_l(void) { swap(&(registers.l)); }

// 0x36
void swap_hlp(void) {
  uint8_t temp = read8(registers.hl);
  swap(&temp);
  write8(registers.hl, temp);
}

// 0x37
void swap_a(void) { swap(&(registers.a)); }

// 0x38
void srl_b(void) { srl(&(registers.b)); }

// 0x39
void srl_c(void) { srl(&(registers.c)); }

// 0x3a
void srl_d(void) { srl(&(registers.d)); }

// 0x3b
void srl_e(void) { srl(&(registers.e)); }

// 0x3c
void srl_h(void) { srl(&(registers.h)); }

// 0x3d
void srl_l(void) { srl(&(registers.l)); }

// 0x3e
void srl_hlp(void) {
  uint8_t temp = read8(registers.hl);
  srl(&temp);
  write8(registers.hl, temp);
}

// 0x3f
void srl_a(void) { srl(&(registers.a)); }

// 0x40
void bit_0_b(void) { bit(registers.b, 0); }

// 0x41
void bit_0_c(void) { bit(registers.c, 0); }

// 0x42
void bit_0_d(void) { bit(registers.d, 0); }

// 0x43
void bit_0_e(void) { bit(registers.e, 0); }

// 0x44
void bit_0_h(void) { bit(registers.h, 0); }

// 0x45
void bit_0_l(void) { bit(registers.l, 0); }

// 0x46
void bit_0_hlp(void) { bit(read8(registers.hl), 0); }

// 0x47
void bit_0_a(void) { bit(registers.a, 0); }

// 0x48
void bit_1_b(void) { bit(registers.b, 1); }

// 0x49
void bit_1_c(void) { bit(registers.c, 1); }

// 0x4a
void bit_1_d(void) { bit(registers.d, 1); }

// 0x4b
void bit_1_e(void) { bit(registers.e, 1); }

// 0x4c
void bit_1_h(void) { bit(registers.h, 1); }

// 0x4d
void bit_1_l(void) { bit(registers.l, 1); }

// 0x4e
void bit_1_hlp(void) { bit(read8(registers.hl), 1); }

// 0x4f
void bit_1_a(void) { bit(registers.a, 1); }

// 0x50
void bit_2_b(void) { bit(registers.b, 2); }

// 0x51
void bit_2_c(void) { bit(registers.c, 2); }

// 0x52
void bit_2_d(void) { bit(registers.d, 2); }

// 0x53
void bit_2_e(void) { bit(registers.e, 2); }

// 0x54
void bit_2_h(void) { bit(registers.h, 2); }

// 0x55
void bit_2_l(void) { bit(registers.l, 2); }

// 0x56
void bit_2_hlp(void) { bit(read8(registers.hl), 2); }

// 0x57
void bit_2_a(void) { bit(registers.a, 2); }

// 0x58
void bit_3_b(void) { bit(registers.b, 3); }

// 0x59
void bit_3_c(void) { bit(registers.c, 3); }

// 0x5a
void bit_3_d(void) { bit(registers.d, 3); }

// 0x5b
void bit_3_e(void) { bit(registers.e, 3); }

// 0x5c
void bit_3_h(void) { bit(registers.h, 3); }

// 0x5d
void bit_3_l(void) { bit(registers.l, 3); }

// 0x5e
void bit_3_hlp(void) { bit(read8(registers.hl), 3); }

// 0x5f
void bit_3_a(void) { bit(registers.a, 3); }

// 0x60
void bit_4_b(void) { bit(registers.b, 4); }

// 0x61
void bit_4_c(void) { bit(registers.c, 4); }

// 0x62
void bit_4_d(void) { bit(registers.d, 4); }

// 0x63
void bit_4_e(void) { bit(registers.e, 4); }

// 0x64
void bit_4_h(void) { bit(registers.h, 4); }

// 0x65
void bit_4_l(void) { bit(registers.l, 4); }

// 0x66
void bit_4_hlp(void) { bit(read8(registers.hl), 4); }

// 0x67
void bit_4_a(void) { bit(registers.a, 4); }

// 0x68
void bit_5_b(void) { bit(registers.b, 5); }

// 0x69
void bit_5_c(void) { bit(registers.c, 5); }

// 0x6a
void bit_5_d(void) { bit(registers.d, 5); }

// 0x6b
void bit_5_e(void) { bit(registers.e, 5); }

// 0x6c
void bit_5_h(void) { bit(registers.h, 5); }

// 0x6d
void bit_5_l(void) { bit(registers.l, 5); }

// 0x6e
void bit_5_hlp(void) { bit(read8(registers.hl), 5); }

// 0x6f
void bit_5_a(void) { bit(registers.a, 5); }

// 0x70
void bit_6_b(void) { bit(registers.b, 6); }

// 0x71
void bit_6_c(void) { bit(registers.c, 6); }

// 0x72
void bit_6_d(void) { bit(registers.d, 6); }

// 0x73
void bit_6_e(void) { bit(registers.e, 6); }

// 0x74
void bit_6_h(void) { bit(registers.h, 6); }

// 0x75
void bit_6_l(void) { bit(registers.l, 6); }

// 0x76
void bit_6_hlp(void) { bit(read8(registers.hl), 6); }

// 0x77
void bit_6_a(void) { bit(registers.a, 6); }

// 0x78
void bit_7_b(void) { bit(registers.b, 7); }

// 0x79
void bit_7_c(void) { bit(registers.c, 7); }

// 0x7a
void bit_7_d(void) { bit(registers.d, 7); }

// 0x7b
void bit_7_e(void) { bit(registers.e, 7); }

// 0x7c
void bit_7_h(void) { bit(registers.h, 7); }

// 0x7d
void bit_7_l(void) { bit(registers.l, 7); }

// 0x7e
void bit_7_hlp(void) { bit(read8(registers.hl), 7); }

// 0x7f
void bit_7_a(void) { bit(registers.a, 7); }

// 0x80
void res_0_b(void) { res(&(registers.b), 0); }

// 0x81
void res_0_c(void) { res(&(registers.c), 0); }

// 0x82
void res_0_d(void) { res(&(registers.d), 0); }

// 0x83
void res_0_e(void) { res(&(registers.e), 0); }

// 0x84
void res_0_h(void) { res(&(registers.h), 0); }

// 0x85
void res_0_l(void) { res(&(registers.l), 0); }

// 0x86
void res_0_hlp(void) {
  uint8_t temp = read8(registers.hl);
  res(&temp, 0);
  write8(registers.hl, temp);
}

// 0x87
void res_0_a(void) { res(&(registers.a), 0); }

// 0x88
void res_1_b(void) { res(&(registers.b), 1); }

// 0x89
void res_1_c(void) { res(&(registers.c), 1); }

// 0x8a
void res_1_d(void) { res(&(registers.d), 1); }

// 0x8b
void res_1_e(void) { res(&(registers.e), 1); }

// 0x8c
void res_1_h(void) { res(&(registers.h), 1); }

// 0x8d
void res_1_l(void) { res(&(registers.l), 1); }

// 0x8e
void res_1_hlp(void) {
  uint8_t temp = read8(registers.hl);
  res(&temp, 1);
  write8(registers.hl, temp);
}

// 0x8f
void res_1_a(void) { res(&(registers.a), 1); }

// 0x90
void res_2_b(void) { res(&(registers.b), 2); }

// 0x91
void res_2_c(void) { res(&(registers.c), 2); }

// 0x92
void res_2_d(void) { res(&(registers.d), 2); }

// 0x93
void res_2_e(void) { res(&(registers.e), 2); }

// 0x94
void res_2_h(void) { res(&(registers.h), 2); }

// 0x95
void res_2_l(void) { res(&(registers.l), 2); }

// 0x96
void res_2_hlp(void) {
  uint8_t temp = read8(registers.hl);
  res(&temp, 2);
  write8(registers.hl, temp);
}

// 0x97
void res_2_a(void) { res(&(registers.a), 2); }

// 0x98
void res_3_b(void) { res(&(registers.b), 3); }

// 0x99
void res_3_c(void) { res(&(registers.c), 3); }

// 0x9a
void res_3_d(void) { res(&(registers.d), 3); }

// 0x9b
void res_3_e(void) { res(&(registers.e), 3); }

// 0x9c
void res_3_h(void) { res(&(registers.h), 3); }

// 0x9d
void res_3_l(void) { res(&(registers.l), 3); }

// 0x9e
void res_3_hlp(void) {
  uint8_t temp = read8(registers.hl);
  res(&temp, 3);
  write8(registers.hl, temp);
}

// 0x9f
void res_3_a(void) { res(&(registers.a), 3); }

// 0xa0
void res_4_b(void) { res(&(registers.b), 4); }

// 0xa1
void res_4_c(void) { res(&(registers.c), 4); }

// 0xa2
void res_4_d(void) { res(&(registers.d), 4); }

// 0xa3
void res_4_e(void) { res(&(registers.e), 4); }

// 0xa4
void res_4_h(void) { res(&(registers.h), 4); }

// 0xa5
void res_4_l(void) { res(&(registers.l), 4); }

// 0xa6
void res_4_hlp(void) {
  uint8_t temp = read8(registers.hl);
  res(&temp, 4);
  write8(registers.hl, temp);
}

// 0xa7
void res_4_a(void) { res(&(registers.a), 4); }

// 0xa8
void res_5_b(void) { res(&(registers.b), 5); }

// 0xa9
void res_5_c(void) { res(&(registers.c), 5); }

// 0xaa
void res_5_d(void) { res(&(registers.d), 5); }

// 0xab
void res_5_e(void) { res(&(registers.e), 5); }

// 0xac
void res_5_h(void) { res(&(registers.h), 5); }

// 0xad
void res_5_l(void) { res(&(registers.l), 5); }

// 0xae
void res_5_hlp(void) {
  uint8_t temp = read8(registers.hl);
  res(&temp, 5);
  write8(registers.hl, temp);
}

// 0xaf
void res_5_a(void) { res(&(registers.a), 5); }

// 0xb0
void res_6_b(void) { res(&(registers.b), 6); }

// 0xb1
void res_6_c(void) { res(&(registers.c), 6); }

// 0xb2
void res_6_d(void) { res(&(registers.d), 6); }

// 0xb3
void res_6_e(void) { res(&(registers.e), 6); }

// 0xb4
void res_6_h(void) { res(&(registers.h), 6); }

// 0xb5
void res_6_l(void) { res(&(registers.l), 6); }

// 0xb6
void res_6_hlp(void) {
  uint8_t temp = read8(registers.hl);
  res(&temp, 6);
  write8(registers.hl, temp);
}

// 0xb7
void res_6_a(void) { res(&(registers.a), 6); }

// 0xb8
void res_7_b(void) { res(&(registers.b), 7); }

// 0xb9
void res_7_c(void) { res(&(registers.c), 7); }

// 0xba
void res_7_d(void) { res(&(registers.d), 7); }

// 0xbb
void res_7_e(void) { res(&(registers.e), 7); }

// 0xbc
void res_7_h(void) { res(&(registers.h), 7); }

// 0xbd
void res_7_l(void) { res(&(registers.l), 7); }

// 0xbe
void res_7_hlp(void) {
  uint8_t temp = read8(registers.hl);
  res(&temp, 7);
  write8(registers.hl, temp);
}

// 0xbf
void res_7_a(void) { res(&(registers.a), 7); }

// 0xc0
void set_0_b(void) { set(&(registers.b), 0); }

// 0xc1
void set_0_c(void) { set(&(registers.c), 0); }

// 0xc2
void set_0_d(void) { set(&(registers.d), 0); }

// 0xc3
void set_0_e(void) { set(&(registers.e), 0); }

// 0xc4
void set_0_h(void) { set(&(registers.h), 0); }

// 0xc5
void set_0_l(void) { set(&(registers.l), 0); }

// 0xc6
void set_0_hlp(void) {
  uint8_t temp = read8(registers.hl);
  set(&temp, 0);
  write8(registers.hl, temp);
}

// 0xc7
void set_0_a(void) { set(&(registers.a), 0); }

// 0xc8
void set_1_b(void) { set(&(registers.b), 1); }

// 0xc9
void set_1_c(void) { set(&(registers.c), 1); }

// 0xca
void set_1_d(void) { set(&(registers.d), 1); }

// 0xcb
void set_1_e(void) { set(&(registers.e), 1); }

// 0xcc
void set_1_h(void) { set(&(registers.h), 1); }

// 0xcd
void set_1_l(void) { set(&(registers.l), 1); }

// 0xce
void set_1_hlp(void) {
  uint8_t temp = read8(registers.hl);
  set(&temp, 1);
  write8(registers.hl, temp);
}

// 0xcf
void set_1_a(void) { set(&(registers.a), 1); }

// 0xd0
void set_2_b(void) { set(&(registers.b), 2); }

// 0xd1
void set_2_c(void) { set(&(registers.c), 2); }

// 0xd2
void set_2_d(void) { set(&(registers.d), 2); }

// 0xd3
void set_2_e(void) { set(&(registers.e), 2); }

// 0xd4
void set_2_h(void) { set(&(registers.h), 2); }

// 0xd5
void set_2_l(void) { set(&(registers.l), 2); }

// 0xd6
void set_2_hlp(void) {
  uint8_t temp = read8(registers.hl);
  set(&temp, 2);
  write8(registers.hl, temp);
}

// 0xd7
void set_2_a(void) { set(&(registers.a), 2); }

// 0xd8
void set_3_b(void) { set(&(registers.b), 3); }

// 0xd9
void set_3_c(void) { set(&(registers.c), 3); }

// 0xda
void set_3_d(void) { set(&(registers.d), 3); }

// 0xdb
void set_3_e(void) { set(&(registers.e), 3); }

// 0xdc
void set_3_h(void) { set(&(registers.h), 3); }

// 0xdd
void set_3_l(void) { set(&(registers.l), 3); }

// 0xde
void set_3_hlp(void) {
  uint8_t temp = read8(registers.hl);
  set(&temp, 3);
  write8(registers.hl, temp);
}

// 0xdf
void set_3_a(void) { set(&(registers.a), 3); }

// 0xe0
void set_4_b(void) { set(&(registers.b), 4); }

// 0xe1
void set_4_c(void) { set(&(registers.c), 4); }

// 0xe2
void set_4_d(void) { set(&(registers.d), 4); }

// 0xe3
void set_4_e(void) { set(&(registers.e), 4); }

// 0xe4
void set_4_h(void) { set(&(registers.h), 4); }

// 0xe5
void set_4_l(void) { set(&(registers.l), 4); }

// 0xe6
void set_4_hlp(void) {
  uint8_t temp = read8(registers.hl);
  set(&temp, 4);
  write8(registers.hl, temp);
}

// 0xe7
void set_4_a(void) { set(&(registers.a), 4); }

// 0xe8
void set_5_b(void) { set(&(registers.b), 5); }

// 0xe9
void set_5_c(void) { set(&(registers.c), 5); }

// 0xea
void set_5_d(void) { set(&(registers.d), 5); }

// 0xeb
void set_5_e(void) { set(&(registers.e), 5); }

// 0xec
void set_5_h(void) { set(&(registers.h), 5); }

// 0xed
void set_5_l(void) { set(&(registers.l), 5); }

// 0xee
void set_5_hlp(void) {
  uint8_t temp = read8(registers.hl);
  set(&temp, 5);
  write8(registers.hl, temp);
}

// 0xef
void set_5_a(void) { set(&(registers.a), 5); }

// 0xf0
void set_6_b(void) { set(&(registers.b), 6); }

// 0xf1
void set_6_c(void) { set(&(registers.c), 6); }

// 0xf2
void set_6_d(void) { set(&(registers.d), 6); }

// 0xf3
void set_6_e(void) { set(&(registers.e), 6); }

// 0xf4
void set_6_h(void) { set(&(registers.h), 6); }

// 0xf5
void set_6_l(void) { set(&(registers.l), 6); }

// 0xf6
void set_6_hlp(void) {
  uint8_t temp = read8(registers.hl);
  set(&temp, 6);
  write8(registers.hl, temp);
}

// 0xf7
void set_6_a(void) { set(&(registers.a), 6); }

// 0xf8
void set_7_b(void) { set(&(registers.b), 7); }

// 0xf9
void set_7_c(void) { set(&(registers.c), 7); }

// 0xfa
void set_7_d(void) { set(&(registers.d), 7); }

// 0xfb
void set_7_e(void) { set(&(registers.e), 7); }

// 0xfc
void set_7_h(void) { set(&(registers.h), 7); }

// 0xfd
void set_7_l(void) { set(&(registers.l), 7); }

// 0xfe
void set_7_hlp(void) {
  uint8_t temp = read8(registers.hl);
  set(&temp, 7);
  write8(registers.hl, temp);
}

// 0xff
void set_7_a(void) { set(&(registers.a), 7); }
