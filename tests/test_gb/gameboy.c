#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "cpu.h"

int main(int argc, char** argv) {
  FILE *cartridge;
  char buffer[30];
  uint8_t instruction;
  uint8_t n;
  uint16_t nn;
  uint32_t cnt = 0;

  if (argc == 2) {
    if ((cartridge = fopen(argv[1],"r")) == NULL) {
      fprintf(stderr,"File not found\n");
      return EXIT_FAILURE;
    }
    while (!feof(cartridge)) {
      fread(&instruction, sizeof(uint8_t), 1, cartridge);
      switch (instructions[(int)instruction].operandLength) {
        case 0 :
          printf("0x%02x : %s\n", cnt, instructions[(int)instruction].disassembly);
          break;
        case 1 :
          fread(&n, sizeof(uint8_t), 1, cartridge);
          sprintf(buffer, instructions[(int)instruction].disassembly, n);
          printf("0x%02x : %s\n", cnt, buffer);
          break;
        case 2 :
          fread(&nn, sizeof(uint16_t), 1, cartridge);
          sprintf(buffer, instructions[(int)instruction].disassembly, nn);
          printf("0x%02x : %s\n", cnt, buffer);
          break;
        default :
          break;
      }
      cnt++;
    }
    fclose(cartridge);
  }
  else {
    printf("Usage : %s <game_file>\n", argv[0]);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
