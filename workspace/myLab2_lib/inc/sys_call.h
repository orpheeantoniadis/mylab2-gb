/*
===============================================================================
 Name        : sys_call.h
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#ifndef SYS_CALL_H_
#define SYS_CALL_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fat.h"

#define STDOUT				0
#define	STANDARD_STREAM		0
#define	FILE_STREAM			0
#define	NB_STREAM			STANDARD_STREAM + FILE_STREAM
#define	STREAM_BUFFER_SIZE	BLOCK_LEN * 3

typedef struct {
	uint16_t fpos;
	char fname[FILENAME_LEN];
	char buffer[STREAM_BUFFER_SIZE];
	uint16_t flen;
	uint8_t flag;
} stream_t;

int open(char *filename);
uint8_t openfiles(void);
int close(uint8_t fd);
int read(uint8_t fd, void *buffer, uint16_t nbytes);
char *get_fname(uint8_t fd);
int get_fd(char *fname);
void rewind_file(uint8_t fd);
uint8_t get_line(char *str, uint8_t fd);

#endif /* SYS_CALL_H_ */
