/*
===============================================================================
 Name        : sys_call.c
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include "sys_call.h"

__DATA(RAM2) static stream_t stream[NB_STREAM];

int find_fd(void) {
	uint8_t fd;
	for (fd = STANDARD_STREAM; fd < NB_STREAM; fd++)
		if (stream[fd].flag == 0) return fd;
	return -1;
}

int open(char *filename) {
	uint8_t fd = find_fd();
	if (fd != -1) {
		if (fat_read_file(stream[fd].buffer, filename) > 0) return fd;
		else return -1;
	} else {
		//fprintf(stderr, "No streams available");
		return -1;
	}
}

int close(uint8_t fd) {
	if (fd < NB_STREAM) {
		stream[fd].flag = 0;
		//stream[fd].fpos = &(stream[fd].buffer[0]);
		return 0;
	} else {
		//fprintf(stderr, "File descriptor is not valid");
		return -1;
	}
}

uint8_t openfiles(void) {
	uint8_t i;
	dir_entry_t entries[255];
	uint8_t cnt = get_all_entries(entries);
	if (cnt > FILE_STREAM) cnt = FILE_STREAM;
	for (i = 0; i < cnt; i++) {
		get_entry_filename(stream[i].fname, &entries[i]);
		stream[i].flen = get_file_content(stream[i].buffer, &entries[i]);
	}
	return cnt;
}

int read(uint8_t fd, void *buffer, uint16_t nbytes) {
	if (fd >= NB_STREAM) return -1;
	uint16_t i;
	for (i = 0; i < nbytes - 1; i++) {
		((char *)buffer)[i] = stream[fd].buffer[i];
	}
	((char *)buffer)[i] = 0;
	return 0;
}

char *get_fname(uint8_t fd) {
	return stream[fd].fname;
}

int get_fd(char *fname) {
	uint8_t i;
	char maj_fname[strlen(fname) + 1];
	maj_str(maj_fname, fname);
	for (i = STANDARD_STREAM; i < FILE_STREAM; i++) {
		if (strcmp(stream[i].fname, maj_fname) == 0) return i;
	}
	return -1;
}

void rewind_file(uint8_t fd){
	stream[fd].fpos = 0;
}

uint8_t get_line(char *str, uint8_t fd) {
	uint16_t pos = 0;
	if (stream[fd].buffer[stream[fd].fpos] == 0) return 0;
	while (stream[fd].buffer[stream[fd].fpos] != '\n' && stream[fd].buffer[stream[fd].fpos] != 0) {
		str[pos] = stream[fd].buffer[stream[fd].fpos];
		stream[fd].fpos++;
		pos++;
	}
	stream[fd].fpos++;
	//pos++;
	str[pos] = 0;
	return 1;
}
