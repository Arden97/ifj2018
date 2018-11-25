#include <stdio.h>
#include <stdlib.h>

#ifndef ifj_error

#define ifj_error

#define LEXICAL_ERROR 1
#define SYNTAX_ERROR 2
#define DEFINITION_ERROR 3
#define TYPE_ERROR 4
#define ARGS_ERROR 5
#define SEMANTIC_ERROR 6
#define DIVBYZERO_ERROR 9
#define INTERNAL_ERROR 99

#define KRED  "\x1B[31m"
#define RESET "\033[0m"
#define KBLU  "\x1B[34m"

void error_msg(int error_code, char *details);
void error(int error_code, char *details);

#endif
