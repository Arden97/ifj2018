#ifndef ifj_error

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "gc.h"
#include "utils.h"
#include <stdarg.h>
#define ifj_error

#define LEXICAL_ERROR 1
#define SYNTAX_ERROR 2
#define DEFINITION_ERROR 3
#define TYPE_ERROR 4
#define ARGS_ERROR 5
#define SEMANTIC_ERROR 6
#define DIVBYZERO_ERROR 9
#define INTERNAL_ERROR 99


char *error_msg(int error_code);
void error(int error_code, const char *format, ...);

#endif
