///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Header file of Semantic control	                                 //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
///////////////////////////////////////////////////////////////////////////////////

#ifndef IFJ18_SEMANTICS_H
#define IFJ18_SEMANTICS_H

#include "../hash_table/khash.h"
#include "../utils/strlib.h"

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
  char *func_name;
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

KHASH_MAP_INIT_STR(value, ifj18_obj_t *);

/// IFJ18 hash.
typedef khash_t(value) ifj18_hash_t;

/// Allocate a new hash.
#define ifj18_hash_new() kh_init(value)

///Destroy the hash.
#define ifj18_hash_destroy(self) kh_destroy(value, self)

/// Hash size.
#define ifj18_hash_size kh_size

/// Save the object with by "key" in symbol table
void ifj18_hash_set(khash_t(value) * self, char *key, ifj18_obj_t *val);

/// Get the object with by "key" from symbol table
ifj18_obj_t *ifj18_hash_get(khash_t(value) * self, char *key);

/// Returns true if there is an object with "key" in symbol table
int ifj18_hash_has(khash_t(value) * self, char *key);

/// Removes `key` from symbol table
void ifj18_hash_remove(khash_t(value) * self, char *key);

/// Creates a variable object
ifj18_obj_t *init_var();

/// Creates a function object
ifj18_obj_t *init_func();

/// Global hash table
extern struct ifj18_hash_t *global_table;

#endif
