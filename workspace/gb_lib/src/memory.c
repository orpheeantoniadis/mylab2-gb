#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "joypad.h"
#include "gui.h"
#include "memory.h"

__DATA(RAM2) memory_t memory;
static uint8_t MBC = 0;
static uint8_t rombank = 1;
static uint8_t rambank = 0;
static uint8_t ram_enable = 0;

#ifdef __UNIX
static uint8_t rambanks[0x8000] ;
#else
static uint8_t rambanks[0x2000];
#endif

const uint8_t bootstrap[0x100] = {
0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32,
0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3,
0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A,
0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06,
0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99,
0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64,
0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90,
0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62,
0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xE2, 0xF0, 0x42,
0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04,
0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9,
0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E,
0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C,
0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13,
0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20,
0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50
};

static void banking_handler(uint16_t addr, uint8_t val) {
	if (addr < 0x2000) {
		if (MBC == 1 || MBC == 2) {
			if (MBC == 2) {
     		if ((val >> 4) & 1) return;
   		}
			if (val == 0xa) ram_enable = 1;
			else if (val == 0) ram_enable = 0;
		}
	} else if ((addr >= 0x2000) && (addr < 0x4000)) {
		if (MBC == 1 || MBC == 2) {
			if (MBC == 2) {
				rombank = val;
				if (rombank == 0) rombank++;
				return;
			}
			rombank &= 0xe0;
			rombank |= val & 0x1f;
			if (rombank == 0) rombank++;
		}
	} else if ((addr >= 0x4000) && (addr < 0x6000)) {
		if (MBC == 1) {
			if(rombank) {
				rombank &= 0x1f; 
				rombank |= val & 0xe0;
				if (rombank == 0) rombank++;
			} else {
				rambank = val & 3;
			} 
		}
	} else if ((addr >= 0x6000) && (addr < 0x8000)) {
		if (MBC == 1) {
			if ((val & 1) == 0) rambank = 0 ; 
		}
	}
}

static void dma_transfer(uint8_t data) {
	uint8_t i;
	uint16_t addr = data << 8;
	for (i = 0; i < 0xa0; i++) {
		memory.OAM[i] = read8(addr+i);
	}
}

int load_rom(char *filename) {
	uint16_t i;
	uint8_t checksum;
	char name[16];
	char *type, *rom_size, *ram_size;
	
#ifdef __UNIX
	FILE *file;
	int cnt = 0;
	if ((file = fopen(filename,"r")) == NULL) {
		gb_log(ERROR, "File not found\n");
		return -1;
	}
	while (!feof(file)) {
		fread(&rom[cnt], sizeof(uint8_t), 1, file);
		cnt++;
	}
	fclose(file);
#endif

	checksum = 0;
	for (i = 0x134; i < 0x14d; i++) checksum = checksum - rom[i] - 1;
	if (checksum != rom[CHECKSUM_OFFSET]){
		gb_log(ERROR, "Checksum incorrect\n");
		return -1;
	}

	for (i = 0; i < 16; i++) name[i] = rom[NAME_OFFSET+i];
	gb_log(INFO, "ROM name: %s\n", name);
	
	switch (rom[TYPE_OFFSET]) {
		case 0x00: type = "ROM ONLY"; break;
		case 0x01: type = "MBC1"; MBC = 1; break;
		case 0x02: type = "MBC1+RAM"; MBC = 1; break;
		case 0x03: type = "MBC1+RAM+BATTERY"; MBC = 1; break;
		case 0x05: type = "MBC2"; MBC = 2; break;
		case 0x06: type = "MBC2+BATTERY"; MBC = 2; break;
		case 0x08: type = "ROM+RAM"; break;
		case 0x09: type = "ROM+RAM+BATTERY"; break;
		case 0x0b: type = "MMM01"; break;
		case 0x0c: type = "MMM01+RAM"; break;
		case 0x0d: type = "MMM01+RAM+BATTERY"; break;
		case 0x0f: type = "MBC3+TIMER+BATTERY"; break;
		case 0x10: type = "MBC3+TIMER+RAM+BATTERY"; break;
		case 0x11: type = "MBC3"; break;
		case 0x12: type = "MBC3+RAM"; break;
		case 0x13: type = "MBC3+RAM+BATTERY"; break;
		case 0x19: type = "MBC5"; break;
		case 0x1a: type = "MBC5+RAM"; break;
		case 0x1b: type = "MBC5+RAM+BATTERY"; break;
		case 0x1c: type = "MBC5+RUMBLE"; break;
		case 0x1d: type = "MBC5+RUMBLE+RAM"; break;
		case 0x1e: type = "MBC5+RUMBLE+RAM+BATTERY"; break;
		case 0x20: type = "MBC6"; break;
		case 0x22: type = "MBC7+SENSOR+RUMBLE+RAM+BATTERY"; break;
		case 0xfc: type = "POCKET CAMERA"; break;
		case 0xfd: type = "BANDAI TAMA5"; break;
		case 0xfe: type = "HuC3"; break;
		case 0xff: type = "HuC1+RAM+BATTERY"; break;
		default : type = "UNKNOWN TYPE"; break;
	}
	gb_log(INFO, "ROM type: %s\n", type);
	
	switch (rom[ROM_SIZE_OFFSET]) {
		case 0x00: rom_size = "32KB"; break;
		case 0x01: rom_size = "64KB"; break;
		case 0x02: rom_size = "128KB"; break;
		case 0x03: rom_size = "256KB"; break;
		case 0x04: rom_size = "512KB"; break;
		case 0x05: rom_size = "1MB"; break;
		case 0x06: rom_size = "2MB"; break;
		case 0x07: rom_size = "4MB"; break;
		case 0x08: rom_size = "8MB"; break;
		case 0x52: rom_size = "1.1MB"; break;
		case 0x53: rom_size = "1.2MB"; break;
		case 0x54: rom_size = "1.5MB"; break;
		default : rom_size = "UNKNOWN ROM SIZE"; break;
	}
	gb_log(INFO, "ROM size: %s\n", rom_size);
	
	switch (rom[ROM_SIZE_OFFSET]) {
		case 0x00: ram_size = "None"; break;
		case 0x01: ram_size = "2KB"; break;
		case 0x02: ram_size = "8KB"; break;
		case 0x03: ram_size = "32KB"; break;
		case 0x04: ram_size = "128KB"; break;
		case 0x05: ram_size = "64KB"; break;
		default : ram_size = "UNKNOWN RAM SIZE"; break;
	}
	gb_log(INFO, "RAM size: %s\n", ram_size);
	
	gb_log(INFO, "ROM loaded successfully\n");
	return 0;
}

uint8_t read8(uint16_t addr) {
	if (addr < 0x100 && BOOT_ROM_IS_ENABLE()) return bootstrap[addr];
	else if (addr < 0x4000) return rom[addr];
	else if (addr < 0x8000) return rom[addr-0x4000+rombank*0x4000];
	else if (addr < 0xa000) return memory.VRAM[addr-0x8000];
	else if (addr < 0xc000) return rambanks[addr-0xa000+(rambank*0x2000)];
	else if (addr < 0xfe00) return memory.WRAM[(addr-0xc000)%0x2000];
	else if (addr == 0xff00) return get_joypad_state();
	else return memory.MEM[addr-OAM_OFFSET];
}

void write8(uint16_t addr, uint8_t data) {
	if (addr < 0x8000) banking_handler(addr, data);
	else if (addr < 0xa000) memory.VRAM[addr-0x8000] = data;
	else if (addr < 0xc000) { if (ram_enable) rambanks[addr-0xa000+(rambank*0x2000)] = data; }
	else if (addr < 0xfe00) memory.WRAM[(addr-0xc000)%0x2000] = data;
	else if (addr < 0xfea0) memory.OAM[addr-0xfe00] = data;
	else if (addr == 0xff04) DIV = 0;
	else if (addr == 0xff44) LY = 0;
	else if (addr == 0xff46) dma_transfer(data);
	else if ((addr > 0xff4b) && (addr < 0xff80) && (addr != 0xff50)) return;
	else memory.MEM[addr-OAM_OFFSET] = data;
}

uint16_t read16(uint16_t addr) {
  return read8(addr) | (read8(addr + 1) << 8);
}

void write16(uint16_t addr, uint16_t val) {
  write8(addr, (val & 0xff));
  write8(addr + 1, ((val & 0xff00) >> 8));
}
