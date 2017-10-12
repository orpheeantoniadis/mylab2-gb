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

  if (((val + carry) & 0x0f) > ((*dst) & 0x0f)) FLAG_SET(HALFCARRY_FLAG);
  else FLAG_CLEAR(HALFCARRY_FLAG);

  if ((val + carry) > (*dst)) FLAG_SET(CARRY_FLAG);
  else FLAG_CLEAR(CARRY_FLAG);

  (*dst) -= (val + carry);

  if ((*dst) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_SET(NEGATIVE_FLAG);
}

static void and(uint8_t *dst, uint8_t val) {
  (*dst) &= val;

  if ((*dst) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_SET(HALFCARRY_FLAG);
  FLAG_CLEAR(CARRY_FLAG);
}

static void or(uint8_t *dst, uint8_t val) {
  (*dst) |= val;

  if ((*dst) == 0) FLAG_SET(ZERO_FLAG);
  else FLAG_CLEAR(ZERO_FLAG);

  FLAG_CLEAR(NEGATIVE_FLAG);
  FLAG_CLEAR(HALFCARRY_FLAG);
  FLAG_CLEAR(CARRY_FLAG);
}

static void xor(uint8_t *dst, uint8_t val) {
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

static void push(uint16_t val) {
  registers.sp-=2;
  write16(val, registers.sp);
}

static void pop(uint16_t *dst) {
  (*dst) = read16(registers.sp);
  registers.sp+=2;
}

// 0x00
void nop(void) { }

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

// 0x08
void ld_nnp_sp(uint16_t nn) { write8(nn, registers.sp); }

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

// 0x10

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

// 0x18
void jr_n(uint8_t n) { registers.pc += n; }

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

// 0x20
void jr_nz(uint8_t n) { if (!FLAG_ISSET(ZERO_FLAG)) registers.pc += n; }

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

// 0x28
void jr_z(uint8_t n) { if (FLAG_ISSET(ZERO_FLAG)) registers.pc += n; }

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

// 0x30
void jr_nc(uint8_t n) { if (!FLAG_ISSET(CARRY_FLAG)) registers.pc += n; }

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

// 0x38
void jr_c(uint8_t n) { if (FLAG_ISSET(CARRY_FLAG)) registers.pc += n; }

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
void or_b(void) { or(&(registers.a), registers.b); }

// 0xb1
void or_c(void) { or(&(registers.a), registers.c); }

// 0xb2
void or_d(void) { or(&(registers.a), registers.d); }

// 0xb3
void or_e(void) { or(&(registers.a), registers.e); }

// 0xb4
void or_h(void) { or(&(registers.a), registers.h); }

// 0xb5
void or_l(void) { or(&(registers.a), registers.l); }

// 0xb6
void or_hlp(void) { or(&(registers.a), read8(registers.hl)); }

// 0xb7
void or_a(void) { or(&(registers.a), registers.a); }

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
void ret_nz(void) { if (!FLAG_ISSET(ZERO_FLAG)) pop(&(registers.pc)); }

// 0xc1
void pop_bc(void) { pop(&(registers.bc)); }

// 0xc2
void jp_nz(uint16_t nn) { if (!FLAG_ISSET(ZERO_FLAG)) registers.pc = nn; }

// 0xc3
void jp(uint16_t nn) { registers.pc = nn; }

// 0xc4
void call_nz(uint16_t nn) {
  if (!FLAG_ISSET(ZERO_FLAG)) {
    push(nn);
    registers.pc = nn;
  }
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
void ret_z(void) { if (FLAG_ISSET(ZERO_FLAG)) pop(&(registers.pc)); }

// 0xc9
void ret(void) { pop(&(registers.pc)); }

// 0xca
void jp_z(uint16_t nn) { if (FLAG_ISSET(ZERO_FLAG)) registers.pc = nn; }

// 0xcb

// 0xcc
void call_z(uint16_t nn) {
  if (FLAG_ISSET(ZERO_FLAG)) {
    push(nn);
    registers.pc = nn;
  }
}

// 0xcd
void call(uint16_t nn) {
  push(nn);
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
void ret_nc(void) { if (!FLAG_ISSET(CARRY_FLAG)) pop(&(registers.pc)); }

// 0xd1
void pop_de(void) { pop(&(registers.de)); }

// 0xd2
void jp_nc(uint16_t nn) { if (!FLAG_ISSET(CARRY_FLAG)) registers.pc = nn; }

// 0xd4
void call_nc(uint16_t nn) {
  if (!FLAG_ISSET(CARRY_FLAG)) {
    push(nn);
    registers.pc = nn;
  }
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
void ret_c(void) { if (FLAG_ISSET(CARRY_FLAG)) pop(&(registers.pc)); }

// 0xd9
void reti(void) {
  pop(&(registers.pc));
  // enable interrupts
}

// 0xda
void jp_c(uint16_t nn) { if (FLAG_ISSET(CARRY_FLAG)) registers.pc = nn; }

// 0xdc
void call_c(uint16_t nn) {
  if (FLAG_ISSET(CARRY_FLAG)) {
    push(nn);
    registers.pc = nn;
  }
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
void and_n(uint8_t n) { and(&(registers.a), n ); }

// 0xe7
void rst_20h(void) {
  push(registers.pc);
  registers.pc = 0x20;
}

// 0xe8
void add_sp_n(uint8_t n) { add16(&(registers.sp), n); }

// 0xe9
void jp_hl(void) { registers.pc = read8(registers.hl); }

// 0xea
void ld_nnp_a(uint16_t nn) { write8(nn, registers.a); }

// 0xee
void xor_n(uint8_t n) { xor(&(registers.a), n); }

//0xef
void rst_28h(void) {
  push(registers.pc);
  registers.pc = 0x28;
}

// 0xf0
void ldh_a_np(uint8_t n) { registers.a = read8(0xff00 + n); }

// 0xf1
void pop_af(void) { pop(&(registers.af)); }

// 0xf2
void ldh_a_cp(void) { registers.a = read8(0xff00 + registers.c); }

// 0xf3

// 0xf5
void push_af(void) { push(registers.af); }

// 0xf6
void or_n(uint8_t n) { or(&(registers.a), n); }

// 0xf7
void rst_30h(void) {
  push(registers.pc);
  registers.pc = 0x30;
}

// 0xf8

// 0xf9
void ld_sp_hl(void) { registers.sp = registers.hl; }

// 0xfa
void ld_a_nnp(uint16_t nn) { registers.a = read8(nn); }

// 0xfb

// 0xfe
void cp_n(uint8_t n) { cp(registers.a, n); }

//0xff
void rst_38h(void) {
  push(registers.pc);
  registers.pc = 0x38;
}
