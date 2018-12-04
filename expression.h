#include "error.h"
#include "scanner.h"
#include "semantics.h"
#include "stack.h"
#include "strlib.h"
#include "symtable.h"
#include "prettyprint.h"

int inputf();
int inputi();
int inputs();
int length();
int substr();
int ord();
int expression(ifj18_obj_t *func, char *ret_var);
int is_function();
int function(ifj18_obj_t *act_function);
int inf_to_post(ifj18_obj_t *act_function, char *ret_var);
ifj18_obj_t *find_var(ifj18_token_t *find_token, ifj18_obj_t *act_function);
ifj18_token_t *copy_token(ifj18_token_t *act_token);
