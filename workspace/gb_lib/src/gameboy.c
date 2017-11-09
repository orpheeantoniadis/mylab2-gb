#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "interrupts.h"
#include "gpu.h"
#include "memory.h"
#include "timer.h"

void draw_tileline(uint16_t pixels) {
	
}

int main(int argc, char **argv) {
  uint8_t cycles;

  if (argc == 2) {
    if (load_rom(argv[1]) == 1)
      return EXIT_FAILURE;
    while (registers.pc < 0x100) {
      // print_instruction();
      cycles = cpu_cycle();
      gpu_cycle(cycles);
      timer_cycle(cycles);
      interrupts_cycle();
      // print_registers();
    }
    print_registers();
    // print_instructions(memory.ROM, 0x100);
  } else {
    printf("Usage : %s <game_file>\n", argv[0]);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
