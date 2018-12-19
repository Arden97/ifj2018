///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Main module	                                                     //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
//              Alexandr Demicev    (xdemic00)                                   //
///////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#include "gc.h"
#include "parser.h"
#include "prettyprint.h"
#include "scanner.h"
#include "semantics.h"

int main() {
  if ((garbage_list = malloc(sizeof(tList))) == NULL) {
    fprintf(stderr, "InternalError: Memory Allocation has failed\n");
    exit(99);
  }

  global_table = ifj18_hash_new();
  init_list(garbage_list);

  // ifj18_obj_t *tmp = init_var();

  // tmp->obj_type.var.value.as_int = 42;
  // ifj18_hash_set(global_table, "foo", tmp);

  // printf("%d\n", tmp->obj_type.var.value.as_int);
  // printf("%d\n", ifj18_hash_has(global_table, "ooo"));

  ifj18_obj_t *func_length = init_func();
  func_length->obj_type.func.params_num = 1;
  func_length->obj_type.func.return_var->type = IFJ18_TYPE_INT;
  ifj18_hash_set(global_table, "length", func_length);

  ifj18_obj_t *func_substr = init_func();
  func_substr->obj_type.func.params_num = 3;
  func_substr->obj_type.func.return_var->type = IFJ18_TYPE_STRING;
  ifj18_hash_set(global_table, "substr", func_substr);

  ifj18_obj_t *func_ord = init_func();
  func_ord->obj_type.func.params_num = 2;
  func_ord->obj_type.func.return_var->type = IFJ18_TYPE_INT;
  ifj18_hash_set(global_table, "ord", func_ord);

  ifj18_obj_t *func_chr = init_func();
  func_chr->obj_type.func.params_num = 1;
  func_chr->obj_type.func.return_var->type = IFJ18_TYPE_STRING;
  ifj18_hash_set(global_table, "chr", func_chr);

  get_token();
  PROG();
  return 0;
}
