#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "interrupts.h"
#include "gpu.h"
#include "memory.h"
#include "timer.h"

#define SB (memory.MEM[0xff01 - 0x9e00])
#define SC (memory.MEM[0xff02 - 0x9e00])

void draw_tileline(uint16_t pixels, uint8_t tilenum) {
	
}

int main(int argc, char **argv) {
  uint8_t cycles;
  load_rom();
  while (1) {
    cycles = cpu_cycle();
		interrupts_cycle();
    gpu_cycle(cycles);
    timer_cycle(cycles);
		if (SC == 0x81) {
			printf("%c", SB);
			SC = 0;
		}
  }
  return EXIT_SUCCESS;
}
