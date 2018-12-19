///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Different tools 	                                               //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
//              Alexandr Demicev    (xdemic00)                                   //
///////////////////////////////////////////////////////////////////////////////////

#ifndef IFJ18_UTIL_H

#define IFJ18_UTIL_H

#include "utils.h"
#include <assert.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define KRED "\x1B[31m"
#define RESET "\033[0m"
#define KBLU "\x1B[34m"
#define KGREY "\e[90m"
#define KYEL "\e[33m"

size_t file_size(FILE *handle);

char *file_read(const char *filename);

char *read_until_eof(FILE *stream);

void debug_info(const char *format, ...);
void debug_info_unwrapped(const char *format, ...);

void print_instruction(char *instruction, const char *format, ...);
void print_instruction_no_args(char *instruction);
#endif
