#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdint.h>

//#define __USE_MYLAB2

#ifndef __USE_MYLAB2
#define __DATA(RAM2)
#else
#include <cr_section_macros.h>
#endif

#define VRAM_SPACE      0x2000
#define WRAM_SPACE      0x2000
#define OAM_SPACE       0x100
#define IO_SPACE        0x80
#define HRAM_SPACE      0x7F
#define OAM_OFFSET			0xBE00

#ifndef __USE_MYLAB2
extern uint8_t rom[];
#else
extern const uint8_t rom[];
#endif

typedef struct {
	struct {
		union {
			uint8_t MEM[0x4200];
			struct {
				uint8_t VRAM[VRAM_SPACE];				// 0x8000-0x9FFF
				uint8_t WRAM[WRAM_SPACE];				// 0xC000-0xDFFF
				uint8_t OAM[OAM_SPACE];					// 0xFE00-0xFEFF
				uint8_t IO[IO_SPACE];						// 0xFF00-0xFF7F
				uint8_t HRAM[HRAM_SPACE];				// 0xFF80-0xFFFE
				uint8_t INT_En;									// 0xFFFF
			};
		};
	};
} memory_t;
extern memory_t memory;

#define SB (memory.MEM[0xff01-OAM_OFFSET])
#define SC (memory.MEM[0xff02-OAM_OFFSET])

#define DIV   (memory.MEM[0xff04-OAM_OFFSET])
#define TIMA  (memory.MEM[0xff05-OAM_OFFSET])
#define TMA   (memory.MEM[0xff06-OAM_OFFSET])
#define TAC   (memory.MEM[0xff07-OAM_OFFSET])

#define LCDC    (memory.MEM[0xff40-OAM_OFFSET])
#define STAT    (memory.MEM[0xff41-OAM_OFFSET])
#define SCROLLY (memory.MEM[0xff42-OAM_OFFSET])
#define SCROLLX (memory.MEM[0xff43-OAM_OFFSET])
#define LY      (memory.MEM[0xff44-OAM_OFFSET])
#define LYC     (memory.MEM[0xff45-OAM_OFFSET])
#define WINDOWY (memory.MEM[0xff4a-OAM_OFFSET])
#define WINDOWX (memory.MEM[0xff4b-OAM_OFFSET])

#define IF  (memory.MEM[0xff0f-OAM_OFFSET])
#define IE  (memory.MEM[0xffff-OAM_OFFSET])

#define BOOT_ROM_IS_ENABLE() (memory.MEM[0xff50-OAM_OFFSET]==0)

void load_rom(void);
uint8_t read8(uint16_t addr);
void write8(uint16_t addr, uint8_t val);
uint16_t read16(uint16_t addr);
void write16(uint16_t addr, uint16_t val);

#endif
