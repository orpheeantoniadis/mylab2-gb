#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "cpu.h"
#include "memory.h"
#include "lcdc.h"

int main(int argc, char** argv) {
  if (argc == 2) {
    if (load_rom(argv[1]) == 1) return EXIT_FAILURE;
    while(registers.pc < 0x100) {
      // print_instruction();
      cpu_cycle();
      lcdc_cycle();
      // print_registers();
    }
    print_registers();
    // print_instructions(memory.ROM, 0x100);
  }
  else {
    printf("Usage : %s <game_file>\n", argv[0]);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
