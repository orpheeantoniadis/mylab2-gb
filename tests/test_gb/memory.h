#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdint.h>

typedef struct {
  struct {
    union {
      uint8_t ROM[0x8000];              // 0x0000-0x7FFF
      struct {
        uint8_t bootstrap[0x100];       // 0x0000-0x00FF
        uint8_t mapped_ROM[0x3F00];     // 0x0100-0x3FFF
        uint8_t switchable_ROM[0x4000]; // 0x4000-0x7FFF
      };
    };
  };
  uint8_t VRAM[0x2000];                 // 0x8000-0x9FFF
  uint8_t external_RAM[0x2000];         // 0xA000-0xBFFF
  struct {
    union {
      uint8_t WRAM[0x2000];             // 0xC000-0xDFFF
      struct {
        uint8_t WRAM0[0x1000];          // 0xC000-0xCFFF
        uint8_t WRAM1[0x1000];          // 0xD000-0xDFFF
      };
    };
  };
  uint8_t WRAM_ECHO[0x1E00];            // 0xE000-0xFDFF
  uint8_t OAM[0x100];                   // 0xFE00-0xFEFF
  uint8_t IO[0x80];                     // 0xFF00-0xFF7F
  uint8_t HRAM[0x7F];                   // 0xFF80-0xFFFE
  uint8_t INT_En;                       // 0xFFFF
} __attribute((packed)) memory_t;
extern memory_t memory;

#endif
