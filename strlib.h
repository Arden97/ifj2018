#ifndef IFJ_STR_H

#define IFJ_STR_H
#define MAX_STRING_SIZE 200

#include "error.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
    int length;
    int mem_alloc;
    char *value;
} string;

int string_init(string *str);


void string_free(string *str);

void string_reset(string *str);


int string_append(string *str, char sym);

int string_compare(string *str1, string *str2);

int string_compare_literal(string *str1, char *str2);

int string_in_list(string *substr, char **strings, int strings_length);

int string_copy_literal(string *str1, char *str2);

int string_copy(string *str1, string *str2);

void string_print(string *str);

#endif
