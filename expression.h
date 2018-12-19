///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Header file of predence analysis 	                               //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
//              Alexandr Demicev    (xdemic00)                                   //
///////////////////////////////////////////////////////////////////////////////////

#include "error.h"
#include "scanner.h"
#include "semantics.h"
#include "stack.h"
#include "strlib.h"
#include "symtable.h"
#include "prettyprint.h"
#include "semantics.h"
#include <stdlib.h>

int inputf();

int inputi();

int inputs();

void length();

void substr();

void ord();

void chr();

int expression(ifj18_obj_t *func, char *ret_var);

int is_function();

int function(ifj18_obj_t *act_function, char *ret_var);

int inf_to_post(ifj18_obj_t *act_function, char *ret_var);
int post_to_instr(ifj18_stack_t *postfix_stack, ifj18_obj_t *act_function, char *ret_var, int operations_count);

ifj18_obj_t *find_var(ifj18_token_t *find_token, ifj18_obj_t *act_function);

ifj18_token_t *copy_token(ifj18_token_t *act_token);

int convert_token_type(ifj18_token_t *token_d);

void generate_3ac_expressions(char *prefix_1, ifj18_obj_t *operand_1, char *prefix_2, ifj18_obj_t *operand_2,
                              ifj18_obj_t *tmp_var_obj,
                              ifj18_var type1, ifj18_var type2, char *operation, int operations_count);

void set_object_value(ifj18_token_t *token_d, ifj18_obj_t *func, ifj18_stack_t *stack);
