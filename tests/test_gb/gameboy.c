#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "cpu.h"
#include "memory.h"

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
    // while (!feof(cartridge)) {
    //   fread(&instruction, sizeof(uint8_t), 1, cartridge);
    //   switch (instructions[instruction].operandLength) {
    //     case 0 :
    //       printf("0x%02x : %s (0x%02x)\n", cnt, instructions[(int)instruction].disassembly, instruction);
    //       break;
    //     case 1 :
    //       fread(&n, sizeof(uint8_t), 1, cartridge);
    //       sprintf(buffer, instructions[instruction].disassembly, n);
    //       printf("0x%02x : %s (0x%02x)\n", cnt, buffer, instruction);
    //       break;
    //     case 2 :
    //       fread(&nn, sizeof(uint16_t), 1, cartridge);
    //       sprintf(buffer, instructions[instruction].disassembly, nn);
    //       printf("0x%02x : %s (0x%02x)\n", cnt, buffer, instruction);
    //       break;
    //     default :
    //       break;
    //   }
    //   cnt++;
    // }
    for (int i = 0; i < 0x100; i++) {
      fread(&instruction, sizeof(uint8_t), 1, cartridge);
      instruction = memory.bootstrap[i];
      switch (instructions[instruction].operandLength) {
        case 0 :
          printf("0x%02x : %s (0x%02x)\n", cnt, instructions[(int)instruction].disassembly, instruction);
          break;
        case 1 :
          n = memory.bootstrap[i+1];
          i++;
          sprintf(buffer, instructions[instruction].disassembly, n);
          printf("0x%02x : %s (0x%02x)\n", cnt, buffer, instruction);
          break;
        case 2 :
          nn = memory.bootstrap[i+1] | (memory.bootstrap[i+2] << 8);
          i+=2;
          sprintf(buffer, instructions[instruction].disassembly, nn);
          printf("0x%02x : %s (0x%02x)\n", cnt, buffer, instruction);
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
