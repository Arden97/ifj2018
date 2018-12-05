#include "expression.h"
#include "prettyprint.h"
#include "scanner.h"
#include "semantics.h"
#include "stdlib.h"
#include "symtable.h"
#include "utils.h"
#include <stdio.h>

int PROG();
int DEFINE_FUNCTION();
// int STATEMENT_LIST(ifj18_obj_t *func);
int STATEMENT(ifj18_obj_t *func);
// int CALL_ASIGN();
void PARAM_LIST(ifj18_obj_t *func, char param_found, string **parameters);
// int NEXT_PARAM();
// int parse_id();
void PARSE_IF();

void PARSE_PRINT(ifj18_obj_t *func);
// int parse_while();
// int parse_print();
// int parse_inputf();
// int parse_inputs();

// int parse_inputi();
// void PARSE_FUNCTION_CALL();