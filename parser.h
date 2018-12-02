#include "scanner.h"
#include "semantics.h"
#include "stdlib.h"
#include "symtable.h"
#include <stdio.h>
#include "expression.h"
#include "prettyprint.h"

int PROG();
int DEFINE_FUNCTION();
int STATEMENT_LIST(ifj18_obj_t *func);
int STATEMENT(ifj18_obj_t *func);
int CALL_ASIGN();
void PARAM_LIST(ifj18_obj_t *func, char param_found);
int NEXT_PARAM();
int parse_id();
int parse_if();
int parse_while();
int parse_print();
int parse_inputf();
int parse_inputs();
int parse_inputi();
