#include "gc.h"
#include "semantics.h"
#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>

tList *adata = NULL;

int main() {
  ifj18_obj_t *tmp = init_var();

  // tmp->obj_type.func.return_var->value.as_int = 42;
  // tmp->obj_type.func.local_symtable = ifj18_hash_new();

  ifj18_hash_set(tmp->obj_type.func.local_symtable, "foo", tmp);

  printf("%d\n", tmp->obj_type.func.return_var->value.as_int);
  printf("%d\n", ifj18_hash_has(tmp->obj_type.func.local_symtable, "ooo"));
}