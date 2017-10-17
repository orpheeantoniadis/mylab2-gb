/*
===============================================================================
 Name        : fat.h
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#ifndef FAT_H_
#define FAT_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <ctype.h>
#include <string.h>
#include "sd_card.h"

#define FAT16				0
#define FAT32				1

#define MBR_OFFSET			0x1BE
#define NB_PARTITION		4
#define DATA_CLUSTER_OFFSET	2
#define ARCHIVE_FLAG		0x20

#define FILENAME_LEN		13

typedef struct {
	uint8_t partition_state;
	uint8_t partition_start[3];
	uint8_t partition_type;
	uint8_t partition_end[3];
	uint32_t partition_secOffset;
	uint32_t partition_size;
} __attribute((packed)) partition_t;

typedef struct {
	uint8_t jmp[3];
	uint8_t oem[8];
	uint16_t sector_size;
	uint8_t sectors_per_cluster;
	uint16_t reserved_sectors;
	uint8_t number_of_fats;
	uint16_t root_dir_entries;
	uint16_t total_sectors_short;
	uint8_t media_descriptor;
	uint16_t fat_size_16;
	uint16_t sectors_per_track;
	uint16_t number_of_heads;
	uint32_t hidden_sectors;
	uint32_t total_sectors_long;
	union {
		struct {
			uint8_t drive_number_16;
			uint8_t reserved1_16;
			uint8_t boot_signature_16;
			uint32_t volume_id_16;
			uint8_t volume_label_16[11];
			uint8_t fs_type_16[8];
			uint8_t boot_code_16[448];
			uint16_t bs_signature_16;
		};
		struct {
			uint32_t fat_size_32;
			uint16_t ext_flags;
			uint16_t fs_version;
			uint32_t root_cluster;
			uint16_t fs_info;
			uint16_t bk_boot_sector;
			uint8_t reserved[12];
			uint8_t drive_number_32;
			uint8_t reserved1_32;
			uint8_t boot_signature_32;
			uint32_t volume_id_32;
			uint8_t volume_label_32[11];
			uint8_t fs_type_32[8];
			uint8_t boot_code_32[372];
			uint16_t bs_signature_32;
		};
	};

} __attribute((packed)) boot_sector_t;

typedef struct {
    uint8_t filename[8];
    uint8_t ext[3];
    uint8_t attributes;
    uint8_t reserved[10];
    uint16_t modify_time;
    uint16_t modify_date;
    uint16_t starting_cluster;
    uint32_t file_size;
} __attribute((packed)) dir_entry_t;

void init_fat(void);
void maj_str(char *maj, char *str);
void get_entry_filename(char *filename, dir_entry_t *entry);
uint16_t get_file_content(char *buffer, dir_entry_t *entry);
uint16_t fat_read_file(char *buf, char *filename);
uint16_t get_all_entries(dir_entry_t *entries);

#endif /* FAT_H_ */
