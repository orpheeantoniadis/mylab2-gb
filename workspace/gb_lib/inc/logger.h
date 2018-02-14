#ifndef _LOGGER_H
#define _LOGGER_H

#include <stdint.h>

enum log_level { ERROR, WARNING, INFO, _DEBUG};
#define LOG_LEVEL	INFO
extern const char *log_level_str[10];

void logger(uint8_t level, char *format, ...);

#endif
