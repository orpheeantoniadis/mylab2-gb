#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdint.h>

#define MEM_SPACE       0x10000
#define ROM_SPACE       0x8000
#define BOOTSRAP_SPACE  0x100
#define CART_SPACE      0x7F00
#define VRAM_SPACE      0x2000
#define EXT_RAM_SPACE   0x2000
#define WRAM_SPACE      0x2000
#define WRAM_ECHO_SPACE 0x2000
#define OAM_SPACE       0x100
#define IO_SPACE        0x80
#define HRAM_SPACE      0x7F

typedef struct {
  union {
    uint8_t MEM[MEM_SPACE];                         // 0x0000-0xFFFF
    struct {
      struct {
        union {
          uint8_t ROM[ROM_SPACE];                   // 0x0000-0x7FFF
          struct {
            uint8_t bootstrap[BOOTSRAP_SPACE];      // 0x0000-0x00FF
            struct {
              union {
                uint8_t cart[CART_SPACE];           // 0x0100-0x7FFF
                struct {
                  uint8_t mapped_ROM[0x3F00];       // 0x0100-0x3FFF
                  uint8_t switchable_ROM[0x4000];   // 0x4000-0x7FFF
                };
              };
            };
          };
        };
      };
      uint8_t VRAM[VRAM_SPACE];                     // 0x8000-0x9FFF
      uint8_t external_RAM[EXT_RAM_SPACE];          // 0xA000-0xBFFF
      struct {
        union {
          uint8_t WRAM[WRAM_SPACE];                 // 0xC000-0xDFFF
          struct {
            uint8_t WRAM0[WRAM_SPACE/2];            // 0xC000-0xCFFF
            uint8_t WRAM1[WRAM_SPACE/2];            // 0xD000-0xDFFF
          };
        };
      };
      uint8_t WRAM_ECHO[WRAM_ECHO_SPACE];           // 0xE000-0xFDFF
      uint8_t OAM[OAM_SPACE];                       // 0xFE00-0xFEFF
      uint8_t IO[IO_SPACE];                         // 0xFF00-0xFF7F
      uint8_t HRAM[HRAM_SPACE];                     // 0xFF80-0xFFFE
      uint8_t INT_En;                               // 0xFFFF
    };
  };
} __attribute((packed)) memory_t;
extern memory_t memory;

void load_rom(char *fiSPACEame);
uint8_t read_byte(uint16_t addr);
void write_byte(uint16_t addr, uint8_t val);

#endif
