///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Header file of error states	                                     //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
///////////////////////////////////////////////////////////////////////////////////

#ifndef ifj_error

#include "../utils/utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
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
