///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Header file of Syntax analysis                                   //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
///////////////////////////////////////////////////////////////////////////////////

#include "../expression/expression.h"
#include "../utils/utils.h"
#include "stdlib.h"
#include <stdio.h>

/// Main function, which calling another non-terminals
int PROG();

/// Non-terminal function which parse function definition
int DEFINE_FUNCTION();

/// Non-terminal function which parse function parametrs
void PARAM_LIST(ifj18_obj_t *func, char param_found, string **parameters);

/// Non-terminal function which parse statements of IFJ18
int STATEMENT(ifj18_obj_t *func);

/// Parsing while statement
void PARSE_WHILE(ifj18_obj_t *func);

/// Parsing if statement
void PARSE_IF(ifj18_obj_t *func);

/// Parse print statement
void PARSE_PRINT(ifj18_obj_t *func);
