#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "interrupts.h"
#include "gpu.h"
#include "memory.h"
#include "timer.h"

void draw_tileline(uint16_t pixels, uint8_t tilenum) {
	
}

int main(int argc, char **argv) {
  uint8_t cycles;

  if (argc == 2) {
    load_rom(argv[1]);
    while (1) {
      // print_instruction();
      cycles = cpu_cycle();
      gpu_cycle(cycles);
      timer_cycle(cycles);
      interrupts_cycle();
			// printf("%d\n", BOOT_ROM_IS_ENABLE());
			if (registers.pc == 0x100) print_registers();
    }
    print_registers();
    // print_instructions(memory.ROM, 0x100);
  } else {
    printf("Usage : %s <game_file>\n", argv[0]);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
