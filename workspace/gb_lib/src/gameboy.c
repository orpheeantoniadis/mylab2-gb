#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#include "cpu.h"
#include "interrupts.h"
#include "gpu.h"
#include "memory.h"
#include "timer.h"

#define USE_LCD			1
#define USE_BOOTROM 1

SDL_Window* window;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Event e;
uint32_t pixels[GB_LCD_WIDTH*GB_LCD_HEIGHT];
uint8_t rom[0x10000];

void SDL2_init(void) {
	SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("myLab2 GB",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GB_LCD_WIDTH, GB_LCD_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, GB_LCD_WIDTH, GB_LCD_HEIGHT);
	memset(pixels, 255, GB_LCD_WIDTH * GB_LCD_HEIGHT * sizeof(uint32_t));
}

void init_project(void) {
	load_rom();
	//memcpy(memory.IO, bootstrap, sizeof(memory.IO));
	if (USE_BOOTROM == 0) {
		registers.af = 0x01b0;
		registers.bc = 0x0013;
		registers.de = 0x00d8;
		registers.hl = 0x014d;
		registers.sp = 0xfffe;
		registers.pc = 0x100;
		interrupt_master = 1;
		write8(0xFF10, 0x80);
		write8(0xFF11, 0xBF);
		write8(0xFF12, 0xF3);
		write8(0xFF14, 0xBF);
		write8(0xFF16, 0x3F);
		write8(0xFF19, 0xBF);
		write8(0xFF1A, 0x7A);
		write8(0xFF1B, 0xFF);
		write8(0xFF1C, 0x9F);
		write8(0xFF1E, 0xBF);
		write8(0xFF20, 0xFF);
		write8(0xFF23, 0xBF);
		write8(0xFF24, 0x77);
		write8(0xFF25, 0xF3);
		write8(0xFF26, 0xF1);
		write8(0xFF40, 0x91);
		write8(0xFF47, 0xFC);
		write8(0xFF48, 0xFF);
		write8(0xFF49, 0xFF);
		write8(0xFF50, 0x01);
	}
}

uint8_t load_rom2(char *filename) {
	FILE *file;
	int cnt = 0;
	if ((file = fopen(filename,"r")) == NULL) {
		fprintf(stderr,"File not found\n");
		return 1;
	}
	while (!feof(file)) {
		fread(&rom[cnt], sizeof(uint8_t), 1, file);
		cnt++;
	}
	fclose(file);
	return 0;
}

void draw_tileline(uint16_t data, uint8_t tilenum) {
	uint8_t i, x;
	uint8_t part1, part2, color;

	part1 = data & 0xff;
	part2 = (data >> 8) & 0xff;
	x = 8 * tilenum;
	
	for (i = 0; i < 8; i++) {
		color = (part1 >> (7 - i) & 1) | ((part2 >> (7 - i) & 1) << 1);
		switch(color) {
		case 0:
			pixels[LY * GB_LCD_WIDTH + x+i] = 0x9bbc0f;
			break;
		case 1:
			pixels[LY * GB_LCD_WIDTH + x+i] = 0x8bac0f;
			break;
		case 2:
			pixels[LY * GB_LCD_WIDTH + x+i] = 0x306230;
			break;
		case 3:
			pixels[LY * GB_LCD_WIDTH + x+i] = 0x0f380f;
			break;
		}
	}
	if (USE_LCD == 1) {
		if (LY == GB_LCD_HEIGHT - 1) {
			SDL_UpdateTexture(texture, NULL, pixels, GB_LCD_WIDTH * sizeof(uint32_t));
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
		}
	}
}

int main(int argc, char** argv) {
  uint8_t cycles;
	
	if (argc == 2) {
		if (load_rom2(argv[1]) == 1) return EXIT_FAILURE;
		init_project();
		if (USE_LCD == 1) SDL2_init();
		bool quit = false;
		while (!quit){
			// print_registers();
	    cycles = cpu_cycle();
			interrupts_cycle();
	    gpu_cycle(cycles);
	    timer_cycle(cycles);
			if (SC == 0x81) {
				printf("%c", SB);
				SC = 0;
			}
			if (USE_LCD == 1) if (SDL_PollEvent(&e) && e.type == SDL_QUIT) quit = true;
	  }
		print_registers();
		if (USE_LCD == 1) {
			SDL_DestroyTexture(texture);
		  SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();
		}
	} else {
		printf("Usage : %s <game_file>\n", argv[0]);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
