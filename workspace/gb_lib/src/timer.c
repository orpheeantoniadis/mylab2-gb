#include <stdio.h>
#include <stdlib.h>
#include "interrupts.h"
#include "timer.h"

static uint16_t timer_cycles_counter = 0;
static uint16_t divider_cycles_counter = 0;

static uint16_t get_frequency(void) {
  switch (TAC & 0b11) {
  case 0: return 1024; // CLOCKSPEED / 4096
  case 1: return 16; // CLOCKSPEED / 262144
  case 2: return 64; // CLOCKSPEED / 65536
  case 3: return 255; // CLOCKSPEED / 16384
	default: return 0;
  }
}

void timer_cycle(uint8_t cycles) {
  if (TIMER_IS_ENABLE()) {
		uint16_t freq = get_frequency();
		timer_cycles_counter += cycles;
    if (timer_cycles_counter > freq) {
      if (TIMA == TIMER_OVERFLOW) {
        TIMA = TMA;
        interrupt_request(IR_TIMER);
      } else TIMA++;
			timer_cycles_counter -= freq;
    }
  }
	divider_cycles_counter += cycles;
  if (divider_cycles_counter < cycles) {
    DIV++;
		divider_cycles_counter -= 255;
  }
}
