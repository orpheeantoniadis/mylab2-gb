#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cpu.h"

int main(int argc, char** argv) {
  FILE *cartridge;
  char instruction;

  if (argc == 2) {
    if ((cartridge = fopen(argv[1],"r")) == NULL) {
      fprintf(stderr,"File not found\n");
      return EXIT_FAILURE;
    }
    while (!feof(cartridge)) {
      fread(&instruction, sizeof(char), 1, cartridge);
      printf("%02x ", instruction & 0xff);
    }
    fclose(cartridge);
  }
  else {
    printf("Usage : %s <game_file>\n", argv[0]);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
