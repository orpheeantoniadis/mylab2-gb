#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "logger.h"

const char *log_level_str[10] = { "ERROR", "WARNING", "INFO", "DEBUG"};

void logger(uint8_t level, char *format, ...) {
	va_list arguments;
	va_start(arguments, format);

	if (level <= LOG_LEVEL) {
		printf("[ML2GB] %s: ", log_level_str[level]);
		vprintf(format, arguments);
	}

	va_end(arguments);
}
