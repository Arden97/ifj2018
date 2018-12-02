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
ifj18_token_t *copy_token(ifj18_token_t *act_token);
