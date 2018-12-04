#ifndef IFJ18_UTIL_H

#define IFJ18_UTIL_H

#include <sys/stat.h>
#include <stddef.h>
#include <stdio.h>
#include "utils.h"
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>


#define KRED  "\x1B[31m"
#define RESET "\033[0m"
#define KBLU  "\x1B[34m"
#define KGREY "\e[90m"

size_t file_size(FILE *handle);

char *file_read(const char *filename);

char *read_until_eof(FILE *stream);

void debug_info(const char *format, ...);
void debug_info_unwrapped(const char *format, ...);

#endif
