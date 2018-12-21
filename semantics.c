///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Semantic control 	                                               //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
///////////////////////////////////////////////////////////////////////////////////

#include "semantics.h"
#include "symtable.h"
#include "strlib.h"

ifj18_obj_t *init_var() {
  ifj18_var_t *var = (ifj18_var_t *)malloc(sizeof(ifj18_var_t));
  if (var == NULL) {
    error(INTERNAL_ERROR, "can't allocate memory for variable");
  }

  var->type = IFJ18_TYPE_NULL;
  var->var_name = (char *)malloc(20);

  ifj18_obj_t *obj_var = (ifj18_obj_t *)malloc(sizeof(ifj18_obj_t));
  obj_var->obj_type.var = *var;
  obj_var->obj_type_flag = 0;
  return obj_var;
}

ifj18_obj_t *init_func() {
  ifj18_var_t *var = (ifj18_var_t *)malloc(sizeof(ifj18_var_t));
  if (var == NULL) {
    error(INTERNAL_ERROR, "can't allocate memory for variable");
  }
  var->type = IFJ18_TYPE_NULL;
  var->var_name = (char *)malloc(20);

  ifj18_func_t *func = (ifj18_func_t *)malloc(sizeof(ifj18_func_t));
  if (func == NULL) {
    error(INTERNAL_ERROR, "can't allocate memory for function");
  }
  string *params = malloc(sizeof(string));
  string_init(params);
  func->sparams = params;
  func->return_var = var;
  func->params_num = 0;
  func->local_symtable = ifj18_hash_new();

  ifj18_obj_t *obj_func = (ifj18_obj_t *)malloc(sizeof(ifj18_obj_t));
  obj_func->obj_type.func = *func;
  obj_func->obj_type_flag = 1;

  return obj_func;
}
