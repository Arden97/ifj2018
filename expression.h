#include "error.h"
#include "scanner.h"
#include "semantics.h"
#include "stack.h"
#include "strlib.h"
#include "symtable.h"

int inputf();
int inputi();
int inputs();
int length();
int substr();
int ord();
void check_token_type(int required_type, int error_type, int inv);
void check_arg(int required_type, char id_allowed);
ifj18_token_t *copy_token(ifj18_token_t *act_token);
