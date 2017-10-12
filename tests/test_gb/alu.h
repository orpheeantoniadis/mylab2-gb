#ifndef _ALU_H
#define _ALU_H

void nop(void);                           // 0x00
void ld_bc_nn(uint16_t nn);               // 0x01
void ld_bcp_a(void);                      // 0x02
void inc_bc(void);                        // 0x03
void inc_b(void);                         // 0x04
void dec_b(void);                         // 0x05
void ld_b_n(uint8_t n);                   // 0x06
                                          // 0x07
void ld_nnp_sp(uint16_t nn);              // 0x08
void add_hl_bc(void);                     // 0x09
void ld_a_bcp(void);                      // 0x0a
void dec_bc(void);                        // 0x0b
void inc_c(void);                         // 0x0c
void dec_c(void);                         // 0x0d
void ld_c_n(uint8_t n);                   // 0x0e
                                          // 0x0f
                                          // 0x10
void ld_de_nn(uint16_t nn);               // 0x11
void ld_dep_a(void);                      // 0x12
void inc_de(void);                        // 0x13
void inc_d(void);                         // 0x14
void dec_d(void);                         // 0x15
void ld_d_n(uint8_t n);                   // 0x16
                                          // 0x17
void jr_n(uint8_t n);                     // 0x18
void add_hl_de(void);                     // 0x19
void ld_a_dep(void);                      // 0x1a
void dec_de(void);                        // 0x1b
void inc_e(void);                         // 0x1c
void dec_e(void);                         // 0x1d
void ld_e_n(uint8_t n);                   // 0x1e
                                          // 0x1f
void jr_nz(uint8_t n);                    // 0x20
void ld_hl_nn(uint16_t nn);               // 0x21
void ldi_hlp_a(void);                     // 0x22
void inc_hl(void);                        // 0x23
void inc_h(void);                         // 0x24
void dec_h(void);                         // 0x25
void ld_h_n(uint8_t n);                   // 0x26
                                          // 0x27
void jr_z(uint8_t n);                     // 0x28
void add_hl_hl(void);                     // 0x29
void ldi_a_hlp(void);                     // 0x2a
void dec_hl(void);                        // 0x2b
void inc_l(void);                         // 0x2c
void dec_l(void);                         // 0x2d
void ld_l_n(uint8_t n);                   // 0x2e
                                          // 0x2f
void jr_nc(uint8_t n);                    // 0x30
void ld_sp_nn(uint16_t nn);               // 0x31
void ldd_hlp_a(void);                     // 0x32
void inc_sp(void);                        // 0x33
void inc_hlp(void);                       // 0x34
void dec_hlp(void);                       // 0x35
void ld_hlp_n(uint8_t n);                 // 0x36
                                          // 0x37
void jr_c(uint8_t n);                     // 0x38
void add_hl_sp(void);                     // 0x39
void ldd_a_hlp(void);                     // 0x3a
void dec_sp(void);                        // 0x3b
void inc_a(void);                         // 0x3c
void dec_a(void);                         // 0x3d
void ld_a_n(uint8_t n);                   // 0x3e
                                          // 0x3f
void ld_b_b(void);                        // 0x40
void ld_b_c(void);                        // 0x41
void ld_b_d(void);                        // 0x42
void ld_b_e(void);                        // 0x43
void ld_b_h(void);                        // 0x44
void ld_b_l(void);                        // 0x45
void ld_b_hlp(void);                      // 0x46
void ld_b_a(void);                        // 0x47
void ld_c_b(void);                        // 0x48
void ld_c_c(void);                        // 0x49
void ld_c_d(void);                        // 0x4a
void ld_c_e(void);                        // 0x4b
void ld_c_h(void);                        // 0x4c
void ld_c_l(void);                        // 0x4d
void ld_c_hlp(void);                      // 0x4e
void ld_c_a(void);                        // 0x4f
void ld_d_b(void);                        // 0x50
void ld_d_c(void);                        // 0x51
void ld_d_d(void);                        // 0x52
void ld_d_e(void);                        // 0x53
void ld_d_h(void);                        // 0x54
void ld_d_l(void);                        // 0x55
void ld_d_hlp(void);                      // 0x56
void ld_d_a(void);                        // 0x57
void ld_e_b(void);                        // 0x58
void ld_e_c(void);                        // 0x59
void ld_e_d(void);                        // 0x5a
void ld_e_e(void);                        // 0x5b
void ld_e_h(void);                        // 0x5c
void ld_e_l(void);                        // 0x5d
void ld_e_hlp(void);                      // 0x5e
void ld_e_a(void);                        // 0x5f
void ld_h_b(void);                        // 0x60
void ld_h_c(void);                        // 0x61
void ld_h_d(void);                        // 0x62
void ld_h_e(void);                        // 0x63
void ld_h_h(void);                        // 0x64
void ld_h_l(void);                        // 0x65
void ld_h_hlp(void);                      // 0x66
void ld_h_a(void);                        // 0x67
void ld_l_b(void);                        // 0x68
void ld_l_c(void);                        // 0x69
void ld_l_d(void);                        // 0x6a
void ld_l_e(void);                        // 0x6b
void ld_l_h(void);                        // 0x6c
void ld_l_l(void);                        // 0x6d
void ld_l_hlp(void);                      // 0x6e
void ld_l_a(void);                        // 0x6f
void ld_hlp_b(void);                      // 0x70
void ld_hlp_c(void);                      // 0x71
void ld_hlp_d(void);                      // 0x72
void ld_hlp_e(void);                      // 0x73
void ld_hlp_h(void);                      // 0x74
void ld_hlp_l(void);                      // 0x75
                                          // 0x76
void ld_hlp_a(void);                      // 0x77
void ld_a_b(void);                        // 0x78
void ld_a_c(void);                        // 0x79
void ld_a_d(void);                        // 0x7a
void ld_a_e(void);                        // 0x7b
void ld_a_h(void);                        // 0x7c
void ld_a_l(void);                        // 0x7d
void ld_a_hlp(void);                      // 0x7e
void ld_a_a(void);                        // 0x7f
void add_a_b(void);                       // 0x80
void add_a_c(void);                       // 0x81
void add_a_d(void);                       // 0x82
void add_a_e(void);                       // 0x83
void add_a_h(void);                       // 0x84
void add_a_l(void);                       // 0x85
void add_a_hlp(void);                     // 0x86
void add_a_a(void);                       // 0x87
void adc_b(void);                         // 0x88
void adc_c(void);                         // 0x89
void adc_d(void);                         // 0x8a
void adc_e(void);                         // 0x8b
void adc_h(void);                         // 0x8c
void adc_l(void);                         // 0x8d
void adc_hlp(void);                       // 0x8e
void adc_a(void);                         // 0x8f
void sub_b(void);                         // 0x90
void sub_c(void);                         // 0x91
void sub_d(void);                         // 0x92
void sub_e(void);                         // 0x93
void sub_h(void);                         // 0x94
void sub_l(void);                         // 0x95
void sub_hlp(void);                       // 0x96
void sub_a(void);                         // 0x97
void sbc_b(void);                         // 0x98
void sbc_c(void);                         // 0x99
void sbc_d(void);                         // 0x9a
void sbc_e(void);                         // 0x9b
void sbc_h(void);                         // 0x9c
void sbc_l(void);                         // 0x9d
void sbc_hlp(void);                       // 0x9e
void sbc_a(void);                         // 0x9f
void and_b(void);                         // 0xa0
void and_c(void);                         // 0xa1
void and_d(void);                         // 0xa2
void and_e(void);                         // 0xa3
void and_h(void);                         // 0xa4
void and_l(void);                         // 0xa5
void and_hlp(void);                       // 0xa6
void and_a(void);                         // 0xa7
void xor_b(void);                         // 0xa8
void xor_c(void);                         // 0xa9
void xor_d(void);                         // 0xaa
void xor_e(void);                         // 0xab
void xor_h(void);                         // 0xac
void xor_l(void);                         // 0xad
void xor_hlp(void);                       // 0xae
void xor_a(void);                         // 0xaf
void or_b(void);                          // 0xb0
void or_c(void);                          // 0xb1
void or_d(void);                          // 0xb2
void or_e(void);                          // 0xb3
void or_h(void);                          // 0xb4
void or_l(void);                          // 0xb5
void or_hlp(void);                        // 0xb6
void or_a(void);                          // 0xb7
void cp_b(void);                          // 0xb8
void cp_c(void);                          // 0xb9
void cp_d(void);                          // 0xba
void cp_e(void);                          // 0xbb
void cp_h(void);                          // 0xbc
void cp_l(void);                          // 0xbd
void cp_hlp(void);                        // 0xbe
void cp_a(void);                          // 0xbf
void ret_nz(void);                        // 0xc0
void pop_bc(void);                        // 0xc1
void jp_nz(uint16_t nn);                  // 0xc2
void jp(uint16_t nn);                     // 0xc3
void call_nz(uint16_t nn);                // 0xc4
void push_bc(void);                       // 0xc5
void add_a_n(uint8_t n);                  // 0xc6
void rst_00h(void);                       // 0xc7
void ret_z(void);                         // 0xc8
void ret(void);                           // 0xc9
void jp_z(uint16_t);                      // 0xca
                                          // 0xcb
void call_z(uint16_t nn);                 // 0xcc
void call(uint16_t nn);                   // 0xcd
void adc_n(uint8_t n);                    // 0xce
void rst_08h(void);                       // 0xcf
void ret_nc(void);                        // 0xd0
void pop_de(void);                        // 0xd1
void jp_nc(uint16_t nn);                  // 0xd2
void call_nc(uint16_t nn);                // 0xd4
void push_de(void);                       // 0xd5
void sub_n(uint8_t n);                    // 0xd6
void rst_10h(void);                       // 0xd7
void ret_c(void);                         // 0xd8
void reti(void);                          // 0xd9
void jp_c(uint16_t nn);                   // 0xda
void call_c(uint16_t nn);                 // 0xdc
void sbc_n(uint8_t n);                    // 0xde
void rst_18h(void);                       // 0xdf
void ldh_np_a(uint8_t n);                 // 0xe0
void pop_hl(void);                        // 0xe1
void ldh_cp_a(void);                      // 0xe2
void push_hl(void);                       // 0xe5
void and_n(uint8_t n);                    // 0xe6
void rst_20h(void);                       // 0xe7
void add_sp_n(uint8_t n);                 // 0xe8
void jp_hl(void);                         // 0xe9
void ld_nnp_a(uint16_t nn);               // 0xea
void xor_n(uint8_t n);                    // 0xee
void rst_28h(void);                       // 0xef
void ldh_a_np(uint8_t n);                 // 0xf0
void pop_af(void);                        // 0xf1
void ldh_a_cp(void);                      // 0xf2
                                          // 0xf3
void push_af(void);                       // 0xf5
void or_n(uint8_t n);                     // 0xf6
void rst_30h(void);                       // 0xf7
                                          // 0xf8
void ld_sp_hl(void);                      // 0xf9
void ld_a_nnp(uint16_t nn);               // 0xfa
                                          // 0xfb
void cp_n(uint8_t n);                     // 0xfe
void rst_38h(void);                       // 0xff

#endif
