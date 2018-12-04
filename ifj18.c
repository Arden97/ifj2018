#include <stdlib.h>
#include <stdio.h>

#include "gc.h"
#include "semantics.h"
#include "scanner.h"
#include "prettyprint.h"
#include "parser.h"


int main() {
  if((garbage_list=malloc(sizeof(tList))) == NULL) {
    fprintf(stderr, "InternalError: Memory Allocation has failed\n");
    exit(99);
  }
  //get_token();

  global_table = ifj18_hash_new();
  init_list(garbage_list);



//  ifj18_obj_t *tmp = init_var();




//   tmp->obj_type.var.value.as_int = 42;
//
//   ifj18_hash_set(global_table, "foo", tmp);

  // printf("%d\n", tmp->obj_type.var.value.as_int);
//   printf("%d\n", ifj18_hash_has(global_table, "ooo"));

  get_token();
  PROG();
//  PROG();
}
