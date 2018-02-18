#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <SDL2/SDL.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#include "gui.h"

SDL_Window* window;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Event e;
uint32_t pixels[GB_LCD_WIDTH*GB_LCD_HEIGHT];
uint8_t rom[0x100000];
bool quit = false;
double period = ((1.0 / FREQ) * 1e6);

void current_utc_time(struct timespec *ts) {
#ifdef __MACH__
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  ts->tv_sec = mts.tv_sec;
  ts->tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_REALTIME, ts);
#endif
}

void SDL2_init(void) {
	SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("myLab2 GB",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GB_LCD_WIDTH, GB_LCD_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, GB_LCD_WIDTH, GB_LCD_HEIGHT);
	memset(pixels, 255, GB_LCD_WIDTH * GB_LCD_HEIGHT * sizeof(uint32_t));
}

void SDL2_destroy(void) {
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void SDL2_render(void) {
	SDL_UpdateTexture(texture, NULL, pixels, GB_LCD_WIDTH * sizeof(uint32_t));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void set_pixel(uint16_t id, uint32_t data) { pixels[id] = data; }

uint32_t get_pixel(uint16_t id) { return pixels[id]; }

void draw_screen(void) { SDL2_render(); }

int gb_thread(void *arg) {
	struct timespec start, finish;
	double elapsed;
	while(!quit && arg == NULL) {
		current_utc_time(&start);
		gb_update();
		current_utc_time(&finish);
		elapsed = (finish.tv_sec - start.tv_sec) * 1e6;
		elapsed += (finish.tv_nsec - start.tv_nsec) / 1e3;
		if (elapsed < period) {
			useconds_t uperiod_sleep = period - elapsed;
			usleep(uperiod_sleep);
		}
	}
	return 0;
}

int main(int argc, char **argv) {	
	if (argc == 2) {
		if (boot_gameboy(argv[1]) == -1) {
      gb_log(INFO, "Closing emulator\n");
      return EXIT_FAILURE;
    }
		if (USE_LCD == 1) SDL2_init();
		SDL_Thread *t = SDL_CreateThread(gb_thread, "Gameboy Thread", NULL);
		while (!quit){
			if (USE_LCD == 1) {
				if (SDL_PollEvent(&e)) {
          switch (e.type) {
            case SDL_QUIT: quit = true; break;
            case SDL_KEYDOWN:
        			switch(e.key.keysym.sym) {
                case SDLK_d: key_pressed(0); break;
        				case SDLK_q: key_pressed(1); break;
        				case SDLK_z: key_pressed(2); break;
        				case SDLK_s: key_pressed(3); break;
                case SDLK_p: key_pressed(4); break;
        				case SDLK_l: key_pressed(5); break;
        				case SDLK_LSHIFT: key_pressed(6); break;
        				case SDLK_RETURN: key_pressed(7); break;
        			}
        		  break;
            case SDL_KEYUP:
        			switch(e.key.keysym.sym) {
                case SDLK_d: key_released(0); break;
        				case SDLK_q: key_released(1); break;
        				case SDLK_z: key_released(2); break;
        				case SDLK_s: key_released(3); break;
                case SDLK_p: key_released(4); break;
        				case SDLK_l: key_released(5); break;
        				case SDLK_LSHIFT: key_released(6); break;
        				case SDLK_RETURN: key_released(7); break;
        			}
        		  break;
          }
        }
			}
		}
		SDL_WaitThread(t, NULL);
		if (USE_LCD == 1) SDL2_destroy();
	} else {
		printf("Usage : %s <game_file>\n", argv[0]);
		return EXIT_FAILURE;
	}
  gb_log(INFO, "Closing emulator\n");
	return EXIT_SUCCESS;
}
