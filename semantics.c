#include "semantics.h"
#include "symtable.h"

ifj18_obj_t *init_var() {
  ifj18_var_t *var = (ifj18_var_t *)malloc(sizeof(ifj18_var_t));
  if (var == NULL) {
    // todo: add error
  }

  var->type = IFJ18_TYPE_NULL;

  ifj18_obj_t *obj_var = (ifj18_obj_t *)malloc(sizeof(ifj18_obj_t));
  obj_var->obj_type.var = *var;
  return obj_var;
}

ifj18_obj_t *init_func() {
  ifj18_var_t *var = (ifj18_var_t *)malloc(sizeof(ifj18_var_t));

  ifj18_func_t *func = (ifj18_func_t *)malloc(sizeof(ifj18_func_t));
  if (func == NULL) {
    // todo: add error
  }
  func->return_var = var;

  ifj18_obj_t *obj_func = (ifj18_obj_t *)malloc(sizeof(ifj18_obj_t));
  obj_func->obj_type.func = *func;
  
  return obj_func;
}