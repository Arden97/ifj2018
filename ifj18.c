///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Main module	                                                     //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
//              Alexandr Demicev    (xdemic00)                                   //
///////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#include "gc.h"
#include "parser.h"
#include "prettyprint.h"
#include "scanner.h"
#include "semantics.h"

int main() {
  if ((garbage_list = malloc(sizeof(tList))) == NULL) {
    fprintf(stderr, "InternalError: Memory Allocation has failed\n");
    exit(99);
  }

  global_table = ifj18_hash_new();
  init_list(garbage_list);

  // ifj18_obj_t *tmp = init_var();

  // tmp->obj_type.var.value.as_int = 42;
  // ifj18_hash_set(global_table, "foo", tmp);

  // printf("%d\n", tmp->obj_type.var.value.as_int);
  // printf("%d\n", ifj18_hash_has(global_table, "ooo"));

  ifj18_obj_t *func_length = init_func();
  func_length->obj_type.func.params_num = 1;
  func_length->obj_type.func.return_var->type = IFJ18_TYPE_INT;
  ifj18_hash_set((kh_value_t *)global_table, "length", func_length);

  ifj18_obj_t *func_substr = init_func();
  func_substr->obj_type.func.params_num = 3;
  func_substr->obj_type.func.return_var->type = IFJ18_TYPE_STRING;
  ifj18_hash_set((kh_value_t *)global_table, "substr", func_substr);

  ifj18_obj_t *func_ord = init_func();
  func_ord->obj_type.func.params_num = 2;
  func_ord->obj_type.func.return_var->type = IFJ18_TYPE_INT;
  ifj18_hash_set((kh_value_t *)global_table, "ord", func_ord);

  ifj18_obj_t *func_chr = init_func();
  func_chr->obj_type.func.params_num = 1;
  func_chr->obj_type.func.return_var->type = IFJ18_TYPE_STRING;
  ifj18_hash_set((kh_value_t *)global_table, "chr", func_chr);

  ifj18_obj_t *func_inputi = init_func();
  func_length->obj_type.func.params_num = 0;
  func_length->obj_type.func.return_var->type = IFJ18_TYPE_INT;
  ifj18_hash_set((kh_value_t *)global_table, "inputi", func_inputi);

  ifj18_obj_t *func_inputs = init_func();
  func_length->obj_type.func.params_num = 0;
  func_length->obj_type.func.return_var->type = IFJ18_TYPE_STRING;
  ifj18_hash_set((kh_value_t *)global_table, "inputs", func_inputs);

  ifj18_obj_t *func_inputf = init_func();
  func_length->obj_type.func.params_num = 0;
  func_length->obj_type.func.return_var->type = IFJ18_TYPE_FLOAT;
  ifj18_hash_set((kh_value_t *)global_table, "inputf", func_inputf);

  print_instruction_no_args(".IFJcode18");

  printf(
      "LABEL chr\n"
      "PUSHFRAME\n"
      "DEFVAR LF@%%retval\n"
      "DEFVAR LF@param1\n"
      "MOVE LF@param1 LF@%%1\n"

      "INT2CHAR LF@%%retval LF@param1\n"
      "POPFRAME\n"
      "RETURN\n\n"
  );

  printf(
      "LABEL length\n"
      "PUSHFRAME\n"
      "DEFVAR LF@%%retval\n"
      "DEFVAR LF@param1\n"
      "MOVE LF@param1 LF@%%1\n"
      "STRLEN LF@%%retval LF@param1\n"
      "POPFRAME\n"
      "RETURN\n\n"
  );
  //
  // printf(
  //     "LABEL substr\n"
  //     "PUSHFRAME\n"
  //     "DEFVAR LF@%%retval\n"
  //     "DEFVAR LF@param1\n"
  //     "MOVE LF@param1 LF@%%1\n"
  //     "DEFVAR LF@param2\n"
  //     "MOVE LF@param1 LF@%%2\n"
  //     "DEFVAR LF@param2\n"
  //     "MOVE LF@param1 LF@%%2\n"
  //     "DEFVAR LF@param3\n"
  //     "MOVE LF@param1 LF@%%3\n"
  //
  //     "STRLEN LF@%%retval LF@param1\n"
  //
  // );

  // printf(
  //     "LABEL ord\n"
  //     "PUSHFRAME\n"
  //     "STRLEN LF@$_stack_temp LF@$_arg_0\n"
  //     "JUMPIFEQ $_assert_zero_end GF@$_stack_temp int@0\n"
  //     "\n"
  //     "SUB GF@$_stack_temp GF@$_stack_temp int@1\n"
  //     "LT GF@$_str_temp_1 LF@$_arg_1 int@0\n"
  //     "JUMPIFEQ $_assert_zero_end GF@$_str_temp_1 bool@true\n"
  //     "\n"
  //     "GT GF@$_str_temp_1 LF@$_arg_1 GF@$_stack_temp\n"
  //     "JUMPIFEQ $_assert_zero_end GF@$_str_temp_1 bool@true\n"
  //     "\n"
  //     "STRI2INT GF@$_stack_temp LF@$_arg_0 LF@$_arg_1\n"
  //     "JUMP $_asc_end\n"
  //     "\n"
  //     "LABEL $_assert_zero_end\n"
  //     "MOVE GF@$_stack_temp int@0\n"
  //     "LABEL $_asc_end\n"
  //     "PUSHS GF@$_stack_temp\n"
  //     "POPFRAME\n"
  //     "RETURN\n\n"
  //     );

  get_token();
  PROG();

  return 0;
}
