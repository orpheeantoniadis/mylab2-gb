#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "cpu.h"
#include "memory.h"

int main(int argc, char** argv) {
  if (argc == 2) {
    load_rom(argv[1]);
    print_instructions(memory.ROM, 0x8000);
  }
  else {
    printf("Usage : %s <game_file>\n", argv[0]);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
