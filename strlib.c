///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Work with strings 	                                             //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
//              Alexandr Demicev    (xdemic00)                                   //
///////////////////////////////////////////////////////////////////////////////////

#include "strlib.h"

int string_init(string *str) {
  if ((str->value = (char *)malloc(8)) == NULL) {
  }
  // exit_error("Allocation failed.", INTERNAL_ERR);
  str->value[0] = str->length = 0;
  return 1;
}

void string_free(string *str) {
  free(str->value);
  //    free(str);
}

void string_reset(string *str) {
  str->value[0] = 0;
  str->length = 0;
}

int string_append(string *str, char sym) {
  if (str->length + 1 >= str->mem_alloc) {
    if ((str->value = (char *)realloc(str->value, str->length + sizeof(char))) == NULL) {
      // exit_error("Allocation failed.", INTERNAL_ERR);
    }
    str->mem_alloc = str->length + sizeof(char);
  }
  str->value[str->length] = tolower(sym);
  str->length++;
  str->value[str->length] = '\0';

  return 1;
}

int string_compare(string *str1, string *str2) { return strcmp(str1->value, str2->value); }

int string_compare_literal(string *str1, char *str2) { return strcmp(str1->value, str2); }

int string_in_list(string *substr, char **strings, int strings_length) {
  for (int i = 0; i < strings_length; ++i) {
    if (!string_compare_literal(substr, strings[i])) {
      return i;
    }
  }
  return -1;
}

int string_copy(string *str1, string *str2) {
  if (str1->mem_alloc <= str2->length) {
    if ((str1->value = (char *)realloc(str1->value, str2->length + 1)) == NULL) {
      // exit(INTERNAL_ERR);
    }
    str1->mem_alloc = str2->length + 1;
  }
  strcpy(str1->value, str2->value);
  str1->length = str2->length;
  return 1;
}

int string_copy_literal(string *str1, char *str2) {
  int str2_length = strlen(str2);
  if (str1->mem_alloc <= str2_length) {
    if ((str1->value = (char *)realloc(str1->value, str2_length + 1)) == NULL) {
      return 0;
    }
    str1->mem_alloc = str2_length + 1;
  }
  strcpy(str1->value, str2);
  str1->length = str2_length;
  return 1;
}
