///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Header file of Semantic control	                                 //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
//              Alexandr Demicev    (xdemic00)                                   //
///////////////////////////////////////////////////////////////////////////////////

#ifndef IFJ18_SEMANTICS_H
#define IFJ18_SEMANTICS_H

#include "strlib.h"

typedef enum {
  IFJ18_TYPE_NULL,
  IFJ18_TYPE_INT,
  IFJ18_TYPE_FLOAT,
  IFJ18_TYPE_STRING,
} ifj18_var;

typedef struct ifj18_var_t {
  ifj18_var type;
  char *var_name;
  union {
    void *as_pointer;
    int as_int;
    float as_float;
  } value;
} ifj18_var_t;

typedef struct ifj18_func_t {
  struct ifj18_hash_t *local_symtable;
  int params_num;
  string *sparams;
  ifj18_var_t *return_var;
} ifj18_func_t;

typedef struct ifj18_obj_t {
  union {
    ifj18_func_t func;
    ifj18_var_t var;
  } obj_type;
  int obj_type_flag; // 1 - func, 0 - var
} ifj18_obj_t;

ifj18_obj_t *init_var();
ifj18_obj_t *init_func();

struct ifj18_hash_t *global_table;

// char flags[4] = {0, 0, 0, 0};
// #define FG_LENGTH 0
// #define FG_SUBSTR 1
// #define FG_ORD 2
// #define FG_CHR 3

#endif
