/*
===============================================================================
 Name        : fat.c
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include "fat.h"

static partition_t mbr[NB_PARTITION];
static boot_sector_t bs;
static uint32_t fats_offset;
static uint32_t dir_entries_offset;
static uint32_t data_offset(uint16_t cluster) {return ((cluster - 2) * bs.sectors_per_cluster) + dir_entries_offset;}

/* ***********************************************************
 * @brief	FAT initialization function. It initializes the
 * 			SD card and then set the values of fat main
 * 			structs (MBR and BS).
 *
 * @param 	none
 * @return 	none
 * ***********************************************************/
void init_fat(void) {
	uint8_t block[BLOCK_LEN];
	uint32_t bs_offset, fat_size;
	init_SDcard();

	sd_read_block(block, 0);
	sd_split_block(mbr, MBR_OFFSET, sizeof(partition_t) * NB_PARTITION, block);

	bs_offset = mbr->partition_secOffset;
	sd_read_block(block, bs_offset);
	sd_split_block(&bs, 0, sizeof(boot_sector_t), block);

	fats_offset = bs.reserved_sectors;
	if (bs.fat_size_16 != 0) fat_size = bs.fat_size_16;
	else fat_size = bs.fat_size_32;
	dir_entries_offset = bs.reserved_sectors + bs.number_of_fats * fat_size + bs_offset;
}

/* ***********************************************************
 * @brief	Function used to set all the characters of a
 * 			string in maj.
 *
 * @param 	maj		string that contains the result
 * @param	str		string that contains the source
 * @return 	none
 * ***********************************************************/
void maj_str(char *maj, char *str) {
	uint8_t i;
	for (i = 0; i < strlen(str); i++) {
		maj[i] = toupper(str[i]);
	}
	maj[i] = 0;
}

/* ***********************************************************
 * @brief	Function used to get an entry filename
 * 			(filename + extension)
 *
 * @param 	filename	string that contains the result
 * @param	entry		dir_entry_t struct
 * @return 	none
 * ***********************************************************/
void get_entry_filename(char *filename, dir_entry_t *entry) {
	uint8_t i, offset;
	for (i = 0; i < sizeof(entry->filename); i++) {
		if (entry->filename[i] == 0x20) break;
		filename[i] = entry->filename[i];
	}
	filename[i] = '.';
	i++;
	offset = i;
	for (i = 0; i < sizeof(entry->ext); i++) {
		if (entry->ext[i] == 0x20) {
			filename[i + offset] = 0;
			break;
		}
		filename[i + offset] = entry->ext[i];
	}
	filename[i + offset] = 0;
}

uint16_t get_all_entries(dir_entry_t *entries) {
	uint8_t block[BLOCK_LEN];
	uint16_t i, j;
	uint16_t cnt = 0;
	for (i = 0; i < (bs.sectors_per_cluster * bs.root_cluster); i++) {
		sd_read_block(block, dir_entries_offset + i);
		for (j = 0; j < BLOCK_LEN / sizeof(dir_entry_t); j++) {
			sd_split_block(&entries[cnt], j * sizeof(dir_entry_t), sizeof(dir_entry_t), block);
			if ((entries[cnt].attributes & ~ARCHIVE_FLAG) == 0 && entries[cnt].starting_cluster != 0) cnt++;
		}
	}
	return cnt;
}

/* ***********************************************************
 * @brief	Function used to find an entry in the root
 * 			directory of the FAT using its filename. If the
 * 			entry is found the function return 1. 0 otherwise.
 *
 * @param 	entry		dir_entry_t struct
 * @param	filename	filename of the entry to find
 * @return 	uint8_t		0 or 1
 * ***********************************************************/
uint8_t get_entry(dir_entry_t *entry, char *filename) {
	uint8_t block[BLOCK_LEN];
	uint16_t i, j;
	char maj_filename[strlen(filename) + 1];
	maj_str(maj_filename, filename);
	for (i = 0; i < (bs.sectors_per_cluster * bs.root_cluster); i++) {
		sd_read_block(block, dir_entries_offset + i);
		for (j = 0; j < BLOCK_LEN / sizeof(dir_entry_t); j++) {
			sd_split_block(entry, j * sizeof(dir_entry_t), sizeof(dir_entry_t), block);
			char entry_filename[sizeof(entry->filename) + sizeof(entry->ext) + 1];
			get_entry_filename(entry_filename, entry);
			if (strcmp(maj_filename, entry_filename) == 0) return 1;
		}
	}
	return 0;
}

uint16_t get_file_content(char *buffer, dir_entry_t *entry) {
	uint8_t block[BLOCK_LEN];
	uint16_t i, j, cnt;
	uint32_t offset;
	cnt = 0;
	offset = data_offset(entry->starting_cluster);
	for (i = 0; i <= (entry->file_size / BLOCK_LEN); i++) {
		sd_read_block(block, offset);
		for (j = 0; j < BLOCK_LEN; j++) {
			buffer[cnt] = block[j];
			cnt++;
		}
		offset++;
	}
	buffer[cnt] = 0;
	return cnt;
}

/* ***********************************************************
 * @brief	Function used to read all the content of a file
 * 			in the SD card. It returns the length of the file
 * 			and if the file isn't found, 0.
 *
 * @param	buf			content of the file that will be read
 * @param	filename	name of the file to read
 * @return 	uint8_t		the length of the file
 * ***********************************************************/
uint16_t fat_read_file(char *buffer, char *filename) {
	dir_entry_t entry;
	if (!get_entry(&entry, filename)) return 0;
	else return get_file_content(buffer, &entry);
}
