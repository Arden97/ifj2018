///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Semantic control 	                                               //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
///////////////////////////////////////////////////////////////////////////////////

#include "semantics.h"

ifj18_obj_t *init_var() {
  ifj18_var_t *var = (ifj18_var_t *)malloc(sizeof(ifj18_var_t));
  if (var == NULL) {
    error(INTERNAL_ERROR, "can't allocate memory for variable");
  }

  var->type = IFJ18_TYPE_NULL;
  var->var_name = (char *)malloc(128);

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
  var->var_name = (char *)malloc(128);

  ifj18_func_t *func = (ifj18_func_t *)malloc(sizeof(ifj18_func_t));
  if (func == NULL) {
    error(INTERNAL_ERROR, "can't allocate memory for function");
  }
  string *params = malloc(sizeof(string));
  string_init(params);
  func->sparams = params;
  func->return_var = var;
  func->params_num = 0;
  func->func_name = (char *)malloc(128);
  func->local_symtable = ifj18_hash_new();

  ifj18_obj_t *obj_func = (ifj18_obj_t *)malloc(sizeof(ifj18_obj_t));
  obj_func->obj_type.func = *func;
  obj_func->obj_type_flag = 1;

  return obj_func;
}

inline void ifj18_hash_set(khash_t(value) * self, char *key, ifj18_obj_t *val) {
  int ret;
  khiter_t k = kh_put(value, self, key, &ret);
  kh_value(self, k) = val;
}

inline ifj18_obj_t *ifj18_hash_get(khash_t(value) * self, char *key) {
  khiter_t k = kh_get(value, self, key);
  return k == kh_end(self) ? NULL : kh_value(self, k);
}

inline int ifj18_hash_has(khash_t(value) * self, char *key) {
  khiter_t k = kh_get(value, self, key);
  if (k == NULL) {
    return 0;
  }
  return kh_exist(self, k);
}

void ifj18_hash_remove(khash_t(value) * self, char *key) {
  khiter_t k = kh_get(value, self, key);
  kh_del(value, self, k);
}
