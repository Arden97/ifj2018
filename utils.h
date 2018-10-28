#ifndef IFJ18_UTIL_H
#define IFJ18_UTIL_H

#include <sys/stat.h>

size_t
file_size(FILE *handle);

char *
file_read(const char *filename);

char *
read_until_eof(FILE *stream);

#endif
