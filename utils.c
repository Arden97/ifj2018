#include "utils.h"

#define DEBUG
#define COLORIFY

/*
 * Return the filesize of `filename` or -1.
 */

size_t file_size(FILE *stream) {
  size_t size = 0;
  fseek(stream, 0, SEEK_END);
  size = ftell(stream);
  rewind(stream);
  return size;
}

/*
 * Read the contents of `filename` or return NULL.
 */

char *file_read(const char *filename) {
  FILE *fh = fopen(filename, "r");
  size_t len = file_size(fh);

  char *buf = malloc(len + 1);
  if (!buf)
    return NULL;

  size_t read = fread(buf, sizeof(char), len, fh);

  fclose(fh);
  buf[read] = 0;
  return buf;
}

/*
 * Read `stream` until EOF.
 */

char *read_until_eof(FILE *stream) {
  // alloc
  off_t len = 0;
  char buf[1024];
  char *str = malloc(1);
  assert(str);

  // read
  while (!feof(stream) && !ferror(stream)) {
    size_t n = fread(buf, 1, 1024, stream);
    len += strlen(buf);
    str = realloc(str, len);
    strncat(str, buf, n);
  }

  return str;
}

void debug_info(const char *format, ...) {
#ifdef DEBUG
  va_list ap;
  va_start(ap, format);
  printf("%s# ", KGREY);
  vprintf(format, ap);
  printf("%s", RESET);
  va_end(ap);
#endif
}

void debug_info_unwrapped(const char *format, ...) {
#ifdef DEBUG
  va_list ap;
  va_start(ap, format);
  printf("%s", KGREY);
  vprintf(format, ap);
  printf("%s", RESET);
  va_end(ap);
#endif
}

void print_instruction(char *instruction, const char *format, ...) {
#if defined(DEBUG) || defined(COLORIFY)
  printf("%s", KYEL);
#endif

  printf("%s ", instruction);

#if defined(COLORIFY) || defined(DEBUG)
  printf("%s", RESET);
#endif

  va_list ap;
  va_start(ap, format);
  vprintf(format, ap);
  va_end(ap);
}

void print_instruction_no_args(char *instruction) {
#if defined(DEBUG) || defined(COLORIFY)
  printf("%s", KYEL);
#endif

  printf("%s\n", instruction);
#if defined(COLORIFY) || defined(DEBUG)
  printf("%s", RESET);
#endif
}