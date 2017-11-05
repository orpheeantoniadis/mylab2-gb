#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "interrupts.h"
#include "lcd.h"
#include "memory.h"
#include "timer.h"

int main(int argc, char **argv) {
  uint8_t cycles;

  if (argc == 2) {
    if (load_rom(argv[1]) == 1)
      return EXIT_FAILURE;
    while (registers.pc < 0x100) {
      // print_instruction();
      cycles = cpu_cycle();
      lcd_cycle(cycles);
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
