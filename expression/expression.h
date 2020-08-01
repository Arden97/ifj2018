///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Header file of predence analysis 	                             //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
///////////////////////////////////////////////////////////////////////////////////

#include "../scanner/scanner.h"
#include "../semantics/semantics.h"
#include "../utils/stack.h"
#include "../utils/strlib.h"
#include "../utils/prettyprint.h"
#include <stdlib.h>

/// Determines the type of expression an calling appropriate function
int expression(ifj18_obj_t *func, char *ret_var);

/// Determines if we dealing with built-in or user's function
int is_function(ifj18_obj_t *func);

/// Generating code for built-in or user's function parametrs
void call_param_list(ifj18_obj_t *act_function, char param_found, ifj18_obj_t *call_function);

/// Generating code for given function
int function(ifj18_obj_t *act_function, char *ret_var);

/// Return TRUE if we should push given token in to stack and FALSE otherwise, depending on symbol in precedence table
int shift_to_stack(ifj18_token_t *stack_token);

/// Executes the appropriate commands depending on operators stack state and symbol in precedence table
void psa_operation(ifj18_stack_t *operators_stack, ifj18_stack_t *output_stack, ifj18_token_t *stack_token);

/// Poping all tokens from temporary stack until it find left paren (using for paren expressions)
void do_until_left_bracket(ifj18_stack_t *operators_stack, ifj18_stack_t *output_stack);

/// Generating code for an appropriate prefix for given operand depending on type of this operand
void print_operation_operand(ifj18_obj_t *operand, char *prefix, int type);

/// Converting an operand for to an appropriate type
void convert_operand(ifj18_obj_t *operand, int oprnd_num);

/// Generating an appropriate prefix for given object
char *get_bytecode_objtype(ifj18_obj_t *operand);

/// Turns an infix expression into postfix and push it in to stack
int inf_to_post(ifj18_obj_t *act_function, char *ret_var);

/// Turns an infix expression into postfix and push it in to stack
int post_to_instr(ifj18_stack_t *postfix_stack, ifj18_obj_t *act_function, char *ret_var, int operations_count);

/// Determines if the variable exists in given function local symbol table
ifj18_obj_t *find_var(ifj18_token_t *find_token, ifj18_obj_t *act_function);

/// Generating code from given parametrs
void generate_3ac_expressions(char *prefix_1, ifj18_obj_t *operand_1, char *prefix_2, ifj18_obj_t *operand_2,
                              ifj18_obj_t *tmp_var_obj, ifj18_obj_t *func,
                              ifj18_var type1, ifj18_var type2, char *operation, int operations_count);

/// Determines and set object type of given variable or primitive and push it in to operands stack
void set_object_value(ifj18_token_t *token_d, ifj18_obj_t *func, ifj18_stack_t *stack, char *ret_var);
