#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdint.h>

#define __USE_MYLAB2

#ifndef __USE_MYLAB2
#define __DATA(RAM2)
#else
#include <cr_section_macros.h>
#endif

#define MEM_SPACE       0x10000
#define ROM_SPACE       0x8000
#define VRAM_SPACE      0x2000
#define EXT_RAM_SPACE   0x2000
#define WRAM_SPACE      0x2000
#define WRAM_ECHO_SPACE 0x1E00
#define OAM_SPACE       0x100
#define IO_SPACE        0x80
#define HRAM_SPACE      0x7F

//extern uint8_t mapped_ROM[0x4000];			// 0x0000-0x3FFF
extern uint8_t switchable_ROM[0x4000];		// 0x4000-0x7FFF
extern uint8_t VRAM[VRAM_SPACE];			// 0x8000-0x9FFF
extern uint8_t external_RAM[EXT_RAM_SPACE];	// 0xA000-0xBFFF
extern uint8_t WRAM[WRAM_SPACE];			// 0xC000-0xDFFF
extern uint8_t OAM[OAM_SPACE];				// 0xFE00-0xFEFF
extern uint8_t IO[IO_SPACE];				// 0xFF00-0xFF7F
extern uint8_t HRAM[HRAM_SPACE];			// 0xFF80-0xFFFE
extern uint8_t INT_En;						// 0xFFFF

#define BOOT_ROM_IS_ENABLE() (read8(0xff50) == 0)

void load_rom(char *filename);
uint8_t read8(uint16_t addr);
void write8(uint16_t addr, uint8_t val);
uint16_t read16(uint16_t addr);
void write16(uint16_t addr, uint16_t val);

#endif
