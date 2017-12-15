#include <stdio.h>
#include <stdlib.h>
#include "alu.h"
#include "commands.h"

const instruction_t instruction_set[256] = {
  {"NOP", 0, nop, 0},                           // 0x00
  {"LD BC, 0x%04x", 2, ld_bc_nn, 0},           // 0x01
  {"LD (BC), A", 0, ld_bcp_a, 0},               // 0x02
  {"INC BC", 0, inc_bc, 0},                     // 0x03
  {"INC B", 0, inc_b, 0},                       // 0x04
  {"DEC B", 0, dec_b, 0},                       // 0x05
  {"LD B, 0x%02x", 1, ld_b_n, 0},               // 0x06
  {"RLCA", 0, rlca, 0},                         // 0x07
  {"LD (0x%04x), SP", 2, ld_nnp_sp, 0},        // 0x08
  {"ADD HL, BC", 0, add_hl_bc, 0},              // 0x09
  {"LD A, (BC)", 0, ld_a_bcp, 0},               // 0x0a
  {"DEC BC", 0, dec_bc, 0},                     // 0x0b
  {"INC C", 0, inc_c, 0},                       // 0x0c
  {"DEC C", 0, dec_c, 0},                       // 0x0d
  {"LD C, 0x%02x", 1, ld_c_n, 0},               // 0x0e
  {"RRCA", 0, rrca, 0},                         // 0x0f
  {"STOP", 1, stop, 0},                         // 0x10
  {"LD DE, 0x%04x", 2, ld_de_nn, 0},           // 0x11
  {"LD (DE), A", 0, ld_dep_a, 0},               // 0x12
  {"INC DE", 0, inc_de, 0},                     // 0x13
  {"INC D", 0, inc_d, 0},                       // 0x14
  {"DEC D", 0, dec_d, 0},                       // 0x15
  {"LD D, 0x%02x", 1, ld_d_n, 0},               // 0x16
  {"RLA", 0, rla, 0},                           // 0x17
  {"JR 0x%02x", 1, jr_n, 0},                   // 0x18
  {"ADD HL, DE", 0, add_hl_de, 0},              // 0x19
  {"LD A, (DE)", 0, ld_a_dep, 0},               // 0x1a
  {"DEC DE", 0, dec_de, 0},                     // 0x1b
  {"INC E", 0, inc_e, 0},                       // 0x1c
  {"DEC E", 0, dec_e, 0},                       // 0x1d
  {"LD E, 0x%02x", 1, ld_e_n, 0},               // 0x1e
  {"RRA", 0, rra, 0},                           // 0x1f
  {"JR NZ, 0x%02x", 1, jr_nz, 1},               // 0x20
  {"LD HL, 0x%04x", 2, ld_hl_nn, 0},           // 0x21
  {"LDI (HL), A", 0, ldi_hlp_a, 0},             // 0x22
  {"INC HL", 0, inc_hl, 0},                     // 0x23
  {"INC H", 0, inc_h, 0},                       // 0x24
  {"DEC H", 0, dec_h, 0},                       // 0x25
  {"LD H, 0x%02x", 1, ld_h_n, 0},               // 0x26
  {"DAA", 0, daa, 0},                           // 0x27
  {"JR Z, 0x%02x", 1, jr_z, 1},                 // 0x28
  {"ADD HL, HL", 0, add_hl_hl, 0},              // 0x29
  {"LDI A, (HL)", 0, ldi_a_hlp, 0},             // 0x2a
  {"DEC HL", 0, dec_hl, 0},                     // 0x2b
  {"INC L", 0, inc_l, 0},                       // 0x2c
  {"DEC L", 0, dec_l, 0},                       // 0x2d
  {"LD L, 0x%02x", 1, ld_l_n, 0},               // 0x2e
  {"CPL", 0, cpl, 0},                           // 0x2f
  {"JR NC, 0x%02x", 1, jr_nc, 1},               // 0x30
  {"LD SP, 0x%04x", 2, ld_sp_nn, 0},           // 0x31
  {"LDD (HL), A", 0, ldd_hlp_a, 0},             // 0x32
  {"INC SP", 0, inc_sp, 0},                     // 0x33
  {"INC (HL)", 0, inc_hlp, 0},                 // 0x34
  {"DEC (HL)", 0, dec_hlp, 0},                 // 0x35
  {"LD (HL), 0x%02x", 1, ld_hlp_n, 0},         // 0x36
  {"SCF", 0, scf, 0},                           // 0x37
  {"JR C, 0x%02x", 1, jr_c, 1},                 // 0x38
  {"ADD HL, SP", 0, add_hl_sp, 0},              // 0x39
  {"LDD A, (HL)", 0, ldd_a_hlp, 0},             // 0x3a
  {"DEC SP", 0, dec_sp, 0},                     // 0x3b
  {"INC A", 0, inc_a, 0},                       // 0x3c
  {"DEC A", 0, dec_a, 0},                       // 0x3d
  {"LD A, 0x%02x", 1, ld_a_n, 0},               // 0x3e
  {"CCF", 0, cff, 0},                           // 0x3f
  {"LD B, B", 0, ld_b_b, 0},                    // 0x40
  {"LD B, C", 0, ld_b_c, 0},                    // 0x41
  {"LD B, D", 0, ld_b_d, 0},                    // 0x42
  {"LD B, E", 0, ld_b_e, 0},                    // 0x43
  {"LD B, H", 0, ld_b_h, 0},                    // 0x44
  {"LD B, L", 0, ld_b_l, 0},                    // 0x45
  {"LD B, (HL)", 0, ld_b_hlp, 0},               // 0x46
  {"LD B, A", 0, ld_b_a, 0},                    // 0x47
  {"LD C, B", 0, ld_c_b, 0},                    // 0x48
  {"LD C, C", 0, ld_c_c, 0},                    // 0x49
  {"LD C, D", 0, ld_c_d, 0},                    // 0x4a
  {"LD C, E", 0, ld_c_e, 0},                    // 0x4b
  {"LD C, H", 0, ld_c_h, 0},                    // 0x4c
  {"LD C, L", 0, ld_c_l, 0},                    // 0x4d
  {"LD C, (HL)", 0, ld_c_hlp, 0},               // 0x4e
  {"LD C, A", 0, ld_c_a, 0},                    // 0x4f
  {"LD D, B", 0, ld_d_b, 0},                    // 0x50
  {"LD D, C", 0, ld_d_c, 0},                    // 0x51
  {"LD D, D", 0, ld_d_d, 0},                    // 0x52
  {"LD D, E", 0, ld_d_e, 0},                    // 0x53
  {"LD D, H", 0, ld_d_h, 0},                    // 0x54
  {"LD D, L", 0, ld_d_l, 0},                    // 0x55
  {"LD D, (HL)", 0, ld_d_hlp, 0},               // 0x56
  {"LD D, A", 0, ld_d_a, 0},                    // 0x57
  {"LD E, B", 0, ld_e_b, 0},                    // 0x58
  {"LD E, C", 0, ld_e_c, 0},                    // 0x59
  {"LD E, D", 0, ld_e_d, 0},                    // 0x5a
  {"LD E, E", 0, ld_e_e, 0},                    // 0x5b
  {"LD E, H", 0, ld_e_h, 0},                    // 0x5c
  {"LD E, L", 0, ld_e_l, 0},                    // 0x5d
  {"LD E, (HL)", 0, ld_e_hlp, 0},               // 0x5e
  {"LD E, A", 0, ld_e_a, 0},                    // 0x5f
  {"LD H, B", 0, ld_h_b, 0},                    // 0x60
  {"LD H, C", 0, ld_h_c, 0},                    // 0x61
  {"LD H, D", 0, ld_h_d, 0},                    // 0x62
  {"LD H, E", 0, ld_h_e, 0},                    // 0x63
  {"LD H, H", 0, ld_h_h, 0},                    // 0x64
  {"LD H, L", 0, ld_h_l, 0},                    // 0x65
  {"LD H, (HL)", 0, ld_h_hlp, 0},               // 0x66
  {"LD H, A", 0, ld_h_a, 0},                    // 0x67
  {"LD L, B", 0, ld_l_b, 0},                    // 0x68
  {"LD L, C", 0, ld_l_c, 0},                    // 0x69
  {"LD L, D", 0, ld_l_d, 0},                    // 0x6a
  {"LD L, E", 0, ld_l_e, 0},                    // 0x6b
  {"LD L, H", 0, ld_l_h, 0},                    // 0x6c
  {"LD L, L", 0, ld_l_l, 0},                    // 0x6d
  {"LD L, (HL)", 0, ld_l_hlp, 0},               // 0x6e
  {"LD L, A", 0, ld_l_a, 0},                    // 0x6f
  {"LD (HL), B", 0, ld_hlp_b, 0},               // 0x70
  {"LD (HL), C", 0, ld_hlp_c, 0},               // 0x71
  {"LD (HL), D", 0, ld_hlp_d, 0},               // 0x72
  {"LD (HL), E", 0, ld_hlp_e, 0},               // 0x73
  {"LD (HL), H", 0, ld_hlp_h, 0},               // 0x74
  {"LD (HL), L", 0, ld_hlp_l, 0},               // 0x75
  {"HALT", 0, halt, 0},                         // 0x76
  {"LD (HL), A", 0, ld_hlp_a, 0},               // 0x77
  {"LD A, B", 0, ld_a_b, 0},                    // 0x78
  {"LD A, C", 0, ld_a_c, 0},                    // 0x79
  {"LD A, D", 0, ld_a_d, 0},                    // 0x7a
  {"LD A, E", 0, ld_a_e, 0},                    // 0x7b
  {"LD A, H", 0, ld_a_h, 0},                    // 0x7c
  {"LD A, L", 0, ld_a_l, 0},                    // 0x7d
  {"LD A, (HL)", 0, ld_a_hlp, 0},               // 0x7e
  {"LD A, A", 0, ld_a_a, 0},                    // 0x7f
  {"ADD A, B", 0, add_a_b, 0},                  // 0x80
  {"ADD A, C", 0, add_a_c, 0},                  // 0x81
  {"ADD A, D", 0, add_a_d, 0},                  // 0x82
  {"ADD A, E", 0, add_a_e, 0},                  // 0x83
  {"ADD A, H", 0, add_a_h, 0},                  // 0x84
  {"ADD A, L", 0, add_a_l, 0},                  // 0x85
  {"ADD A, (HL)", 0, add_a_hlp, 0},             // 0x86
  {"ADD A", 0, add_a_a, 0},                     // 0x87
  {"ADC B", 0, adc_b, 0},                       // 0x88
  {"ADC C", 0, adc_c, 0},                       // 0x89
  {"ADC D", 0, adc_d, 0},                       // 0x8a
  {"ADC E", 0, adc_e, 0},                       // 0x8b
  {"ADC H", 0, adc_h, 0},                       // 0x8c
  {"ADC L", 0, adc_l, 0},                       // 0x8d
  {"ADC (HL)", 0, adc_hlp, 0},                  // 0x8e
  {"ADC A", 0, adc_a, 0},                       // 0x8f
  {"SUB B", 0, sub_b, 0},                       // 0x90
  {"SUB C", 0, sub_c, 0},                       // 0x91
  {"SUB D", 0, sub_d, 0},                       // 0x92
  {"SUB E", 0, sub_e, 0},                       // 0x93
  {"SUB H", 0, sub_h, 0},                       // 0x94
  {"SUB L", 0, sub_l, 0},                       // 0x95
  {"SUB (HL)", 0, sub_hlp, 0},                  // 0x96
  {"SUB A", 0, sub_a, 0},                       // 0x97
  {"SBC B", 0, sbc_b, 0},                       // 0x98
  {"SBC C", 0, sbc_c, 0},                       // 0x99
  {"SBC D", 0, sbc_d, 0},                       // 0x9a
  {"SBC E", 0, sbc_e, 0},                       // 0x9b
  {"SBC H", 0, sbc_h, 0},                       // 0x9c
  {"SBC L", 0, sbc_l, 0},                       // 0x9d
  {"SBC (HL)", 0, sbc_hlp, 0},                  // 0x9e
  {"SBC A", 0, sbc_a, 0},                       // 0x9f
  {"AND B", 0, and_b, 0},                       // 0xa0
  {"AND C", 0, and_c, 0},                       // 0xa1
  {"AND D", 0, and_d, 0},                       // 0xa2
  {"AND E", 0, and_e, 0},                       // 0xa3
  {"AND H", 0, and_h, 0},                       // 0xa4
  {"AND L", 0, and_l, 0},                       // 0xa5
  {"AND (HL)", 0, and_hlp, 0},                  // 0xa6
  {"AND A", 0, and_a, 0},                       // 0xa7
  {"XOR B", 0, xor_b, 0},                       // 0xa8
  {"XOR C", 0, xor_c, 0},                       // 0xa9
  {"XOR D", 0, xor_d, 0},                       // 0xaa
  {"XOR E", 0, xor_e, 0},                       // 0xab
  {"XOR H", 0, xor_h, 0},                       // 0xac
  {"XOR L", 0, xor_l, 0},                       // 0xad
  {"XOR (HL)", 0, xor_hlp, 0},                  // 0xae
  {"XOR A", 0, xor_a, 0},                       // 0xaf
  {"OR B", 0, or_b, 0},                         // 0xb0
  {"OR C", 0, or_c, 0},                         // 0xb1
  {"OR D", 0, or_d, 0},                         // 0xb2
  {"OR E", 0, or_e, 0},                         // 0xb3
  {"OR H", 0, or_h, 0},                         // 0xb4
  {"OR L", 0, or_l, 0},                         // 0xb5
  {"OR (HL)", 0, or_hlp, 0},                    // 0xb6
  {"OR A", 0, or_a, 0},                         // 0xb7
  {"CP B", 0, cp_b, 0},                         // 0xb8
  {"CP C", 0, cp_c, 0},                         // 0xb9
  {"CP D", 0, cp_d, 0},                         // 0xba
  {"CP E", 0, cp_e, 0},                         // 0xbb
  {"CP H", 0, cp_h, 0},                         // 0xbc
  {"CP L", 0, cp_l, 0},                         // 0xbd
  {"CP (HL)", 0, cp_hlp, 0},                    // 0xbe
  {"CP A", 0, cp_a, 0},                         // 0xbf
  {"RET NZ", 0, ret_nz, 1},                     // 0xc0
  {"POP BC", 0, pop_bc, 0},                    // 0xc1
  {"JP NZ, 0x%04x", 2, jp_nz, 1},               // 0xc2
  {"JP 0x%04x", 2, jp, 0},                     // 0xc3
  {"CALL NZ, 0x%04x", 2, call_nz, 1},           // 0xc4
  {"PUSH BC", 0, push_bc, 0},                  // 0xc5
  {"ADD A, 0x%02x", 1, add_a_n, 0},             // 0xc6
  {"RST 0x00", 0, rst_00h, 0},                 // 0xc7
  {"RET Z", 0, ret_z, 1},                       // 0xc8
  {"RET", 0, ret, 0},                          // 0xc9
  {"JP Z, 0x%04x", 2, jp_z, 1},                 // 0xca
  {"CB %02x", 1, NULL, 0},                      // 0xcb
  {"CALL Z, 0x%04x", 2, call_z, 1},             // 0xcc
  {"CALL 0x%04x", 2, call, 0},                 // 0xcd
  {"ADC 0x%02x", 1, adc_n, 0},                  // 0xce
  {"RST 0x08", 0, rst_08h, 0},                 // 0xcf
  {"RET NC", 0, ret_nc, 1},                     // 0xd0
  {"POP DE", 0, pop_de, 0},                    // 0xd1
  {"JP NC, 0x%04x", 2, jp_nc, 1},               // 0xd2
  {"UNKNOWN", 0, NULL, 0},                      // 0xd3
  {"CALL NC, 0x%04x", 2, call_nc, 1},           // 0xd4
  {"PUSH DE", 0, push_de, 0},                  // 0xd5
  {"SUB 0x%02x", 1, sub_n, 0},                  // 0xd6
  {"RST 0x10", 0, rst_10h, 0},                 // 0xd7
  {"RET C", 0, ret_c, 1},                       // 0xd8
  {"RETI", 0, reti, 0},                        // 0xd9
  {"JP C, 0x%04x", 2, jp_c, 1},                 // 0xda
  {"UNKNOWN", 0, NULL, 0},                      // 0xdb
  {"CALL C, 0x%04x", 2, call_c, 1},             // 0xdc
  {"UNKNOWN", 0, NULL, 0},                      // 0xdd
  {"SBC 0x%02x", 1, sbc_n, 0},                  // 0xde
  {"RST 0x18", 0, rst_18h, 0},                 // 0xdf
  {"LD (0xFF00 + 0x%02x), A", 1, ldh_np_a, 0}, // 0xe0
  {"POP HL", 0, pop_hl, 0},                    // 0xe1
  {"LD (0xFF00 + C), A", 0, ldh_cp_a, 0},       // 0xe2
  {"UNKNOWN", 0, NULL, 0},                      // 0xe3
  {"UNKNOWN", 0, NULL, 0},                      // 0xe4
  {"PUSH HL", 0, push_hl, 0},                  // 0xe5
  {"AND 0x%02x", 1, and_n, 0},                  // 0xe6
  {"RST 0x20", 0, rst_20h, 0},                 // 0xe7
  {"ADD SP,0x%02x", 1, add_sp_n, 0},           // 0xe8
  {"JP HL", 0, jp_hl, 0},                       // 0xe9
  {"LD (0x%04x), A", 2, ld_nnp_a, 0},          // 0xea
  {"UNKNOWN", 0, NULL, 0},                      // 0xeb
  {"UNKNOWN", 0, NULL, 0},                      // 0xec
  {"UNKNOWN", 0, NULL, 0},                      // 0xed
  {"XOR 0x%02x", 1, xor_n, 0},                  // 0xee
  {"RST 0x28", 0, rst_28h, 0},                 // 0xef
  {"LD A, (0xFF00 + 0x%02x)", 1, ldh_a_np, 0}, // 0xf0
  {"POP AF", 0, pop_af, 0},                    // 0xf1
  {"LD A, (0xFF00 + C)", 0, ldh_a_cp, 0},       // 0xf2
  {"DI", 0, di, 0},                             // 0xf3
  {"UNKNOWN", 0, NULL, 0},                      // 0xf4
  {"PUSH AF", 0, push_af, 0},                  // 0xf5
  {"OR 0x%02x", 1, or_n, 0},                    // 0xf6
  {"RST 0x30", 0, rst_30h, 0},                 // 0xf7
  {"LD HL, SP+0x%02x", 1, ldhl_sp_n, 0},       // 0xf8
  {"LD SP, HL", 0, ld_sp_hl, 0},                // 0xf9
  {"LD A, (0x%04x)", 2, ld_a_nnp, 0},          // 0xfa
  {"EI", 0, ei, 0},                             // 0xfb
  {"UNKNOWN", 0, NULL, 0},                      // 0xfc
  {"UNKNOWN", 0, NULL, 0},                      // 0xfd
  {"CP 0x%02x", 1, cp_n, 0},                    // 0xfe
  {"RST 0x38", 0, rst_38h, 0},                 // 0xff
};

const uint8_t op_times[256] = {
1,3,2,2,1,1,2,1,5,2,2,2,1,1,2,1,
0,3,2,2,1,1,2,1,3,2,2,2,1,1,2,1,
2,3,2,2,1,1,2,1,2,2,2,2,1,1,2,1,
2,3,2,2,3,3,3,1,2,2,2,2,1,1,2,1,
1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
2,2,2,2,2,2,0,2,1,1,1,1,1,1,2,1,
1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
2,3,3,4,3,4,2,4,2,4,3,0,3,6,2,4,
2,3,3,0,3,4,2,4,2,4,3,0,3,0,2,4,
3,3,2,0,0,4,2,4,4,1,4,0,0,0,2,4,
3,3,2,1,0,4,2,4,3,2,4,1,0,0,2,4,
};

const uint8_t taken_op_times[256] = {
1,3,2,2,1,1,2,1,5,2,2,2,1,1,2,1,
0,3,2,2,1,1,2,1,3,2,2,2,1,1,2,1,
3,3,2,2,1,1,2,1,3,2,2,2,1,1,2,1,
3,3,2,2,3,3,3,1,3,2,2,2,1,1,2,1,
1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
2,2,2,2,2,2,0,2,1,1,1,1,1,1,2,1,
1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
1,1,1,1,1,1,2,1,1,1,1,1,1,1,2,1,
5,3,4,4,6,4,2,4,5,4,4,0,6,6,2,4,
5,3,4,0,6,4,2,4,5,4,4,0,6,0,2,4,
3,3,2,0,0,4,2,4,4,1,4,0,0,0,2,4,
3,3,2,1,0,4,2,4,3,2,4,1,0,0,2,4,
};

const instruction_t prefix_cb[256] = {
  {"RLC B", 1, rlc_b, 0},            // 0x00
  {"RLC C", 1, rlc_c, 0},            // 0x01
  {"RLC D", 1, rlc_d, 0},            // 0x02
  {"RLC E", 1, rlc_e, 0},            // 0x03
  {"RLC H", 1, rlc_h, 0},            // 0x04
  {"RLC L", 1, rlc_l, 0},            // 0x05
  {"RLC (HL)", 1, rlc_hlp, 0},      // 0x06
  {"RLC A", 1, rlc_a, 0},            // 0x07
  {"RRC B", 1, rrc_b, 0},            // 0x08
  {"RRC C", 1, rrc_c, 0},            // 0x09
  {"RRC D", 1, rrc_d, 0},            // 0x0a
  {"RRC E", 1, rrc_e, 0},            // 0x0b
  {"RRC H", 1, rrc_h, 0},            // 0x0c
  {"RRC L", 1, rrc_l, 0},            // 0x0d
  {"RRC (HL)", 1, rrc_hlp, 0},      // 0x0e
  {"RRC A", 1, rrc_a, 0},            // 0x0f
  {"RL B", 1, rl_b, 0},              // 0x10
  {"RL C", 1, rl_c, 0},              // 0x11
  {"RL D", 1, rl_d, 0},              // 0x12
  {"RL E", 1, rl_e, 0},              // 0x13
  {"RL H", 1, rl_h, 0},              // 0x14
  {"RL L", 1, rl_l, 0},              // 0x15
  {"RL (HL)", 1, rl_hlp, 0},        // 0x16
  {"RL A", 1, rl_a, 0},              // 0x17
  {"RR B", 1, rr_b, 0},              // 0x18
  {"RR C", 1, rr_c, 0},              // 0x19
  {"RR D", 1, rr_d, 0},              // 0x1a
  {"RR E", 1, rr_e, 0},              // 0x1b
  {"RR H", 1, rr_h, 0},              // 0x1c
  {"RR L", 1, rr_l, 0},              // 0x1d
  {"RR (HL)", 1, rr_hlp, 0},        // 0x1e
  {"RR A", 1, rr_a, 0},              // 0x1f
  {"SLA B", 1, sla_b, 0},            // 0x20
  {"SLA C", 1, sla_c, 0},            // 0x21
  {"SLA D", 1, sla_d, 0},            // 0x22
  {"SLA E", 1, sla_e, 0},            // 0x23
  {"SLA H", 1, sla_h, 0},            // 0x24
  {"SLA L", 1, sla_l, 0},            // 0x25
  {"SLA (HL)", 1, sla_hlp, 0},      // 0x26
  {"SLA A", 1, sla_a, 0},            // 0x27
  {"SRA B", 1, sra_b, 0},            // 0x28
  {"SRA C", 1, sra_c, 0},            // 0x29
  {"SRA D", 1, sra_d, 0},            // 0x2a
  {"SRA E", 1, sra_e, 0},            // 0x2b
  {"SRA H", 1, sra_h, 0},            // 0x2c
  {"SRA L", 1, sra_l, 0},            // 0x2d
  {"SRA (HL)", 1, sra_hlp, 0},      // 0x2e
  {"SRA A", 1, sra_a, 0},            // 0x2f
  {"SWAP B", 1, swap_b, 0},          // 0x30
  {"SWAP C", 1, swap_c, 0},          // 0x31
  {"SWAP D", 1, swap_d, 0},          // 0x32
  {"SWAP E", 1, swap_e, 0},          // 0x33
  {"SWAP H", 1, swap_h, 0},          // 0x34
  {"SWAP L", 1, swap_l, 0},          // 0x35
  {"SWAP (HL)", 1, swap_hlp, 0},    // 0x36
  {"SWAP A", 1, swap_a, 0},          // 0x37
  {"SRL B", 1, srl_b, 0},            // 0x38
  {"SRL C", 1, srl_c, 0},            // 0x39
  {"SRL D", 1, srl_d, 0},            // 0x3a
  {"SRL E", 1, srl_e, 0},            // 0x3b
  {"SRL H", 1, srl_h, 0},            // 0x3c
  {"SRL L", 1, srl_l, 0},            // 0x3d
  {"SRL (HL)", 1, srl_hlp, 0},      // 0x3e
  {"SRL A", 1, srl_a, 0},            // 0x3f
  {"BIT 0, B", 1, bit_0_b, 0},       // 0x40
  {"BIT 0, C", 1, bit_0_c, 0},       // 0x41
  {"BIT 0, D", 1, bit_0_d, 0},       // 0x42
  {"BIT 0, E", 1, bit_0_e, 0},       // 0x43
  {"BIT 0, H", 1, bit_0_h, 0},       // 0x44
  {"BIT 0, L", 1, bit_0_l, 0},       // 0x45
  {"BIT 0, (HL)", 1, bit_0_hlp, 0}, // 0x46
  {"BIT 0, A", 1, bit_0_a, 0},       // 0x47
  {"BIT 1, B", 1, bit_1_b, 0},       // 0x48
  {"BIT 1, C", 1, bit_1_c, 0},       // 0x49
  {"BIT 1, D", 1, bit_1_d, 0},       // 0x4a
  {"BIT 1, E", 1, bit_1_e, 0},       // 0x4b
  {"BIT 1, H", 1, bit_1_h, 0},       // 0x4c
  {"BIT 1, L", 1, bit_1_l, 0},       // 0x4d
  {"BIT 1, (HL)", 1, bit_1_hlp, 0}, // 0x4e
  {"BIT 1, A", 1, bit_1_a, 0},       // 0x4f
  {"BIT 2, B", 1, bit_2_b, 0},       // 0x50
  {"BIT 2, C", 1, bit_2_c, 0},       // 0x51
  {"BIT 2, D", 1, bit_2_d, 0},       // 0x52
  {"BIT 2, E", 1, bit_2_e, 0},       // 0x53
  {"BIT 2, H", 1, bit_2_h, 0},       // 0x54
  {"BIT 2, L", 1, bit_2_l, 0},       // 0x55
  {"BIT 2, (HL)", 1, bit_2_hlp, 0}, // 0x56
  {"BIT 2, A", 1, bit_2_a, 0},       // 0x57
  {"BIT 3, B", 1, bit_3_b, 0},       // 0x58
  {"BIT 3, C", 1, bit_3_c, 0},       // 0x59
  {"BIT 3, D", 1, bit_3_d, 0},       // 0x5a
  {"BIT 3, E", 1, bit_3_e, 0},       // 0x5b
  {"BIT 3, H", 1, bit_3_h, 0},       // 0x5c
  {"BIT 3, L", 1, bit_3_l, 0},       // 0x5d
  {"BIT 3, (HL)", 1, bit_3_hlp, 0}, // 0x5e
  {"BIT 3, A", 1, bit_3_a, 0},       // 0x5f
  {"BIT 4, B", 1, bit_4_b, 0},       // 0x60
  {"BIT 4, C", 1, bit_4_c, 0},       // 0x61
  {"BIT 4, D", 1, bit_4_d, 0},       // 0x62
  {"BIT 4, E", 1, bit_4_e, 0},       // 0x63
  {"BIT 4, H", 1, bit_4_h, 0},       // 0x64
  {"BIT 4, L", 1, bit_4_l, 0},       // 0x65
  {"BIT 4, (HL)", 1, bit_4_hlp, 0}, // 0x66
  {"BIT 4, A", 1, bit_4_a, 0},       // 0x67
  {"BIT 5, B", 1, bit_5_b, 0},       // 0x68
  {"BIT 5, C", 1, bit_5_c, 0},       // 0x69
  {"BIT 5, D", 1, bit_5_d, 0},       // 0x6a
  {"BIT 5, E", 1, bit_5_e, 0},       // 0x6b
  {"BIT 6, H", 1, bit_5_h, 0},       // 0x6c
  {"BIT 6, L", 1, bit_5_l, 0},       // 0x6d
  {"BIT 5, (HL)", 1, bit_5_hlp, 0}, // 0x6e
  {"BIT 5, A", 1, bit_5_a, 0},       // 0x6f
  {"BIT 6, B", 1, bit_6_b, 0},       // 0x70
  {"BIT 6, C", 1, bit_6_c, 0},       // 0x71
  {"BIT 6, D", 1, bit_6_d, 0},       // 0x72
  {"BIT 6, E", 1, bit_6_e, 0},       // 0x73
  {"BIT 6, H", 1, bit_6_h, 0},       // 0x74
  {"BIT 6, L", 1, bit_6_l, 0},       // 0x75
  {"BIT 6, (HL)", 1, bit_6_hlp, 0}, // 0x76
  {"BIT 6, A", 1, bit_6_a, 0},       // 0x77
  {"BIT 7, B", 1, bit_7_b, 0},       // 0x78
  {"BIT 7, C", 1, bit_7_c, 0},       // 0x79
  {"BIT 7, D", 1, bit_7_d, 0},       // 0x7a
  {"BIT 7, E", 1, bit_7_e, 0},       // 0x7b
  {"BIT 7, H", 1, bit_7_h, 0},       // 0x7c
  {"BIT 7, L", 1, bit_7_l, 0},       // 0x7d
  {"BIT 7, (HL)", 1, bit_7_hlp, 0}, // 0x7e
  {"BIT 7, A", 1, bit_7_a, 0},       // 0x7f
  {"RES 0, B", 1, res_0_b, 0},       // 0x80
  {"RES 0, C", 1, res_0_c, 0},       // 0x81
  {"RES 0, D", 1, res_0_d, 0},       // 0x82
  {"RES 0, E", 1, res_0_e, 0},       // 0x83
  {"RES 0, H", 1, res_0_h, 0},       // 0x84
  {"RES 0, L", 1, res_0_l, 0},       // 0x85
  {"RES 0, (HL)", 1, res_0_hlp, 0}, // 0x86
  {"RES 0, A", 1, res_0_a, 0},       // 0x87
  {"RES 1, B", 1, res_1_b, 0},       // 0x88
  {"RES 1, C", 1, res_1_c, 0},       // 0x89
  {"RES 1, D", 1, res_1_d, 0},       // 0x8a
  {"RES 1, E", 1, res_1_e, 0},       // 0x8b
  {"RES 1, H", 1, res_1_h, 0},       // 0x8c
  {"RES 1, L", 1, res_1_l, 0},       // 0x8d
  {"RES 1, (HL)", 1, res_1_hlp, 0}, // 0x8e
  {"RES 1, A", 1, res_1_a, 0},       // 0x8f
  {"RES 2, B", 1, res_2_b, 0},       // 0x90
  {"RES 2, C", 1, res_2_c, 0},       // 0x91
  {"RES 2, D", 1, res_2_d, 0},       // 0x92
  {"RES 2, E", 1, res_2_e, 0},       // 0x93
  {"RES 2, H", 1, res_2_h, 0},       // 0x94
  {"RES 2, L", 1, res_2_l, 0},       // 0x95
  {"RES 2, (HL)", 1, res_2_hlp, 0}, // 0x96
  {"RES 2, A", 1, res_2_a, 0},       // 0x97
  {"RES 3, B", 1, res_3_b, 0},       // 0x98
  {"RES 3, C", 1, res_3_c, 0},       // 0x99
  {"RES 3, D", 1, res_3_d, 0},       // 0x9a
  {"RES 3, E", 1, res_3_e, 0},       // 0x9b
  {"RES 3, H", 1, res_3_h, 0},       // 0x9c
  {"RES 3, L", 1, res_3_l, 0},       // 0x9d
  {"RES 3, (HL)", 1, res_3_hlp, 0}, // 0x9e
  {"RES 3, A", 1, res_3_a, 0},       // 0x9f
  {"RES 4, B", 1, res_4_b, 0},       // 0xa0
  {"RES 4, C", 1, res_4_c, 0},       // 0xa1
  {"RES 4, D", 1, res_4_d, 0},       // 0xa2
  {"RES 4, E", 1, res_4_e, 0},       // 0xa3
  {"RES 4, H", 1, res_4_h, 0},       // 0xa4
  {"RES 4, L", 1, res_4_l, 0},       // 0xa5
  {"RES 4, (HL)", 1, res_4_hlp, 0}, // 0xa6
  {"RES 4, A", 1, res_4_a, 0},       // 0xa7
  {"RES 5, B", 1, res_5_b, 0},       // 0xa8
  {"RES 5, C", 1, res_5_c, 0},       // 0xa9
  {"RES 5, D", 1, res_5_d, 0},       // 0xaa
  {"RES 5, E", 1, res_5_e, 0},       // 0xab
  {"RES 5, H", 1, res_5_h, 0},       // 0xac
  {"RES 5, L", 1, res_5_l, 0},       // 0xad
  {"RES 5, (HL)", 1, res_5_hlp, 0}, // 0xae
  {"RES 5, A", 1, res_5_a, 0},       // 0xaf
  {"RES 6, B", 1, res_6_b, 0},       // 0xb0
  {"RES 6, C", 1, res_6_c, 0},       // 0xb1
  {"RES 6, D", 1, res_6_d, 0},       // 0xb2
  {"RES 6, E", 1, res_6_e, 0},       // 0xb3
  {"RES 6, H", 1, res_6_h, 0},       // 0xb4
  {"RES 6, L", 1, res_6_l, 0},       // 0xb5
  {"RES 6, (HL)", 1, res_6_hlp, 0}, // 0xb6
  {"RES 6, A", 1, res_6_a, 0},       // 0xb7
  {"RES 7, B", 1, res_7_b, 0},       // 0xb8
  {"RES 7, C", 1, res_7_c, 0},       // 0xb9
  {"RES 7, D", 1, res_7_d, 0},       // 0xba
  {"RES 7, E", 1, res_7_e, 0},       // 0xbb
  {"RES 7, H", 1, res_7_h, 0},       // 0xbc
  {"RES 7, L", 1, res_7_l, 0},       // 0xbd
  {"RES 7, (HL)", 1, res_7_hlp, 0}, // 0xbe
  {"RES 7, A", 1, res_7_a, 0},       // 0xbf
  {"SET 0, B", 1, set_0_b, 0},       // 0xc0
  {"SET 0, C", 1, set_0_c, 0},       // 0xc1
  {"SET 0, D", 1, set_0_d, 0},       // 0xc2
  {"SET 0, E", 1, set_0_e, 0},       // 0xc3
  {"SET 0, H", 1, set_0_h, 0},       // 0xc4
  {"SET 0, L", 1, set_0_l, 0},       // 0xc5
  {"SET 0, (HL)", 1, set_0_hlp, 0}, // 0xc6
  {"SET 0, A", 1, set_0_a, 0},       // 0xc7
  {"SET 1, B", 1, set_1_b, 0},       // 0xc8
  {"SET 1, C", 1, set_1_c, 0},       // 0xc9
  {"SET 1, D", 1, set_1_d, 0},       // 0xca
  {"SET 1, E", 1, set_1_e, 0},       // 0xcb
  {"SET 1, H", 1, set_1_h, 0},       // 0xcc
  {"SET 1, L", 1, set_1_l, 0},       // 0xcd
  {"SET 1, (HL)", 1, set_1_hlp, 0}, // 0xce
  {"SET 1, A", 1, set_1_a, 0},       // 0xcf
  {"SET 2, B", 1, set_2_b, 0},       // 0xd0
  {"SET 2, C", 1, set_2_c, 0},       // 0xd1
  {"SET 2, D", 1, set_2_d, 0},       // 0xd2
  {"SET 2, E", 1, set_2_e, 0},       // 0xd3
  {"SET 2, H", 1, set_2_h, 0},       // 0xd4
  {"SET 2, L", 1, set_2_l, 0},       // 0xd5
  {"SET 2, (HL)", 1, set_2_hlp, 0}, // 0xd6
  {"SET 2, A", 1, set_2_a, 0},       // 0xd7
  {"SET 3, B", 1, set_3_b, 0},       // 0xd8
  {"SET 3, C", 1, set_3_c, 0},       // 0xd9
  {"SET 3, D", 1, set_3_d, 0},       // 0xda
  {"SET 3, E", 1, set_3_e, 0},       // 0xdb
  {"SET 3, H", 1, set_3_h, 0},       // 0xdc
  {"SET 3, L", 1, set_3_l, 0},       // 0xdd
  {"SET 3, (HL)", 1, set_3_hlp, 0}, // 0xde
  {"SET 3, A", 1, set_3_a, 0},       // 0xdf
  {"SET 4, B", 1, set_4_b, 0},       // 0xe0
  {"SET 4, C", 1, set_4_c, 0},       // 0xe1
  {"SET 4, D", 1, set_4_d, 0},       // 0xe2
  {"SET 4, E", 1, set_4_e, 0},       // 0xe3
  {"SET 4, H", 1, set_4_h, 0},       // 0xe4
  {"SET 4, L", 1, set_4_l, 0},       // 0xe5
  {"SET 4, (HL)", 1, set_4_hlp, 0}, // 0xe6
  {"SET 4, A", 1, set_4_a, 0},       // 0xe7
  {"SET 5, B", 1, set_5_b, 0},       // 0xe8
  {"SET 5, C", 1, set_5_c, 0},       // 0xe9
  {"SET 5, D", 1, set_5_d, 0},       // 0xea
  {"SET 5, E", 1, set_5_e, 0},       // 0xeb
  {"SET 5, H", 1, set_5_h, 0},       // 0xec
  {"SET 5, L", 1, set_5_l, 0},       // 0xed
  {"SET 5, (HL)", 1, set_5_hlp, 0}, // 0xee
  {"SET 5, A", 1, set_5_a, 0},       // 0xef
  {"SET 6, B", 1, set_6_b, 0},       // 0xf0
  {"SET 6, C", 1, set_6_c, 0},       // 0xf1
  {"SET 6, D", 1, set_6_d, 0},       // 0xf2
  {"SET 6, E", 1, set_6_e, 0},       // 0xf3
  {"SET 6, H", 1, set_6_h, 0},       // 0xf4
  {"SET 6, L", 1, set_6_l, 0},       // 0xf5
  {"SET 6, (HL)", 1, set_6_hlp, 0}, // 0xf6
  {"SET 6, A", 1, set_6_a, 0},       // 0xf7
  {"SET 7, B", 1, set_7_b, 0},       // 0xf8
  {"SET 7, C", 1, set_7_c, 0},       // 0xf9
  {"SET 7, D", 1, set_7_d, 0},       // 0xfa
  {"SET 7, E", 1, set_7_e, 0},       // 0xfb
  {"SET 7, H", 1, set_7_h, 0},       // 0xfc
  {"SET 7, L", 1, set_7_l, 0},       // 0xfd
  {"SET 7, (HL)", 1, set_7_hlp, 0}, // 0xfe
  {"SET 7, A", 1, set_7_a, 0},       // 0xff
};

const uint8_t cb_op_times[256] = {
	2,2,2,2,2,2,4,2,2,2,2,2,2,2,4,2,
	2,2,2,2,2,2,4,2,2,2,2,2,2,2,4,2,
	2,2,2,2,2,2,4,2,2,2,2,2,2,2,4,2,
	2,2,2,2,2,2,4,2,2,2,2,2,2,2,4,2,
	2,2,2,2,2,2,3,2,2,2,2,2,2,2,3,2,
	2,2,2,2,2,2,3,2,2,2,2,2,2,2,3,2,
	2,2,2,2,2,2,3,2,2,2,2,2,2,2,3,2,
	2,2,2,2,2,2,3,2,2,2,2,2,2,2,3,2,
	2,2,2,2,2,2,4,2,2,2,2,2,2,2,4,2,
	2,2,2,2,2,2,4,2,2,2,2,2,2,2,4,2,
	2,2,2,2,2,2,4,2,2,2,2,2,2,2,4,2,
	2,2,2,2,2,2,4,2,2,2,2,2,2,2,4,2,
	2,2,2,2,2,2,4,2,2,2,2,2,2,2,4,2,
	2,2,2,2,2,2,4,2,2,2,2,2,2,2,4,2,
	2,2,2,2,2,2,4,2,2,2,2,2,2,2,4,2,
	2,2,2,2,2,2,4,2,2,2,2,2,2,2,4,2,
};

void print_instructions(uint8_t *opcodes, int size) {
  char buffer[30];
  uint8_t opcode;
  uint8_t n;
  uint16_t nn;

  for (int i = 0; i < size; i++) {
    opcode = opcodes[i];
    switch (instruction_set[opcode].length) {
    case 0:
      printf("0x%02x : %s (0x%02x)\n", i, instruction_set[opcode].disassembly, opcode);
      break;
    case 1:
      n = opcodes[i + 1];
      if (opcode == 0xcb) {
        printf("0x%02x : %s (0x%02x 0x%02x)\n", i, prefix_cb[n].disassembly, opcode, n);
      } else {
        sprintf(buffer, instruction_set[opcode].disassembly, n);
        printf("0x%02x : %s (0x%02x)\n", i, buffer, opcode);
      }
      i++;
      break;
    case 2:
      nn = opcodes[i + 1] | (opcodes[i + 2] << 8);
      sprintf(buffer, instruction_set[opcode].disassembly, nn);
      printf("0x%02x : %s (0x%02x)\n", i, buffer, opcode);
      i += 2;
      break;
    }
  }
}
