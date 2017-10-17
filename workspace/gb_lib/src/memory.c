#include <cr_section_macros.h>
#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "memory.h"

__DATA(RAM2) memory_t memory = {.bootstrap = { 0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32,
                                  0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
                                  0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3,
                                  0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
                                  0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A,
                                  0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
                                  0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06,
                                  0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
                                  0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99,
                                  0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
                                  0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64,
                                  0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
                                  0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90,
                                  0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
                                  0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62,
                                  0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
                                  0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xE2, 0xF0, 0x42,
                                  0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
                                  0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04,
                                  0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
                                  0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9,
                                  0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
                                  0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
                                  0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
                                  0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
                                  0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
                                  0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E,
                                  0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C,
                                  0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13,
                                  0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
                                  0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20,
                                  0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50
                                }
                  };

void load_rom(char *filename) {
  FILE *rom;
  int cnt = 0;
  if ((rom = fopen(filename,"r")) == NULL) {
    fprintf(stderr,"File not found\n");
    return;
  }
  fseek(rom, 0x100, SEEK_SET);
  // while (!feof(rom)) {
  while (cnt < CART_SPACE) {
    fread(&memory.cart[cnt], sizeof(uint8_t), 1, rom);
    cnt++;
  }
  fclose(rom);
}

uint8_t read8(uint16_t addr) {
  return memory.MEM[addr];
}

void write8(uint16_t addr, uint8_t val) {
  if (addr < 0x8000) {
    print_instruction();
    fprintf(stderr,"Cannot write in ROM space\n");
  }
  else {
    memory.MEM[addr] = val;
  }
}

uint16_t read16(uint16_t addr) {
  return read8(addr) | (read8(addr + 1) << 8);
}

void write16(uint16_t addr, uint16_t val) {
  write8(addr, (val & 0xff));
  write8(addr + 1, ((val & 0xff00) >> 8));
}
