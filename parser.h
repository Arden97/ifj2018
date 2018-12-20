///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Header file of Syntax analysis                                   //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
//              Alexandr Demicev    (xdemic00)                                   //
///////////////////////////////////////////////////////////////////////////////////

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

// void PARSE_FUNCTION_CALL();
