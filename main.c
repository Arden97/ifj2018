///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Main module	                                                     //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
///////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "gc/gc.h"
#include "parser/parser.h"

int main() {
  if ((garbage_list = malloc(sizeof(tList))) == NULL) {
    error(INTERNAL_ERROR, "can't allocate memory for garbage colector");
  }
  init_list(garbage_list);

  global_table = ifj18_hash_new();

  // Creating built-in functions
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
  func_inputi->obj_type.func.params_num = 0;
  func_inputi->obj_type.func.return_var->type = IFJ18_TYPE_INT;
  ifj18_hash_set((kh_value_t *)global_table, "inputi", func_inputi);

  ifj18_obj_t *func_inputs = init_func();
  func_inputs->obj_type.func.params_num = 0;
  func_inputs->obj_type.func.return_var->type = IFJ18_TYPE_STRING;
  ifj18_hash_set((kh_value_t *)global_table, "inputs", func_inputs);

  ifj18_obj_t *func_inputf = init_func();
  func_inputf->obj_type.func.params_num = 0;
  func_inputf->obj_type.func.return_var->type = IFJ18_TYPE_FLOAT;
  ifj18_hash_set((kh_value_t *)global_table, "inputf", func_inputf);

  print_instruction_no_args(".IFJcode18");
  printf("JUMP MAIN\n\n");

  // Generating code for built-in functions
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

  printf(
      "LABEL substr\n"
      "PUSHFRAME\n"
      "DEFVAR LF@%%retval\n"
      "MOVE LF@%%retval string@\n"
      "DEFVAR LF@param1\n"
      "MOVE LF@param1 LF@%%1\n"
      "DEFVAR LF@param2\n"
      "MOVE LF@param2 LF@%%2\n"
      "DEFVAR LF@param3\n"
      "MOVE LF@param3 LF@%%3\n"

      "DEFVAR LF@%%str_temp1\n"
      "DEFVAR LF@%%str_temp2\n"

      "STRLEN LF@%%str_temp2 LF@param1\n"
      "JUMPIFEQ %%assert_zero LF@%%str_temp2 int@0\n"

      "LT LF@%%str_temp1 LF@param2 int@1\n"
      "JUMPIFEQ %%assert_zero LF@%%str_temp1 bool@true\n"

      "LT LF@%%str_temp1 LF@param3 int@0\n"
      "JUMPIFEQ %%truncate_N LF@%%str_temp1 bool@true\n"

      "SUB LF@%%str_temp1 LF@%%str_temp2 LF@param2\n"
      "GT LF@%%str_temp1 LF@param3 LF@%%str_temp1\n"
      "JUMPIFEQ %%truncate_N LF@%%str_temp1 bool@true\n"
      "JUMP %%cycle_start\n"

      "LABEL %%truncate_N\n"
      "SUB LF@%%str_temp1 LF@%%str_temp2 LF@param2\n"
      "ADD LF@%%str_temp1 LF@%%str_temp1 int@1\n"
      "MOVE LF@param3 LF@%%str_temp1\n"

      "LABEL %%cycle_start\n"
      "SUB LF@param2 LF@param2 int@1\n"
      "MOVE LF@%%retval string@\n"

      "LABEL %%compare_next\n"
      "JUMPIFEQ %%all_done LF@param3 int@0\n"
      "GETCHAR LF@%%str_temp1 LF@param1 LF@param2\n"
      "CONCAT LF@%%retval LF@%%retval LF@%%str_temp1\n"
      "ADD LF@param1 LF@param2 int@1\n"
      "SUB LF@param2 LF@param3 int@1\n"
      "JUMP %%compare_next\n"

      "LABEL %%assert_zero\n"
      "MOVE LF@%%retval string@\n"

      "LABEL %%all_done\n"
      "POPFRAME\n"
      "RETURN\n\n"
  );

  printf(
      "LABEL ord\n"
      "PUSHFRAME\n"

      "DEFVAR LF@%%retval\n"
      "DEFVAR LF@param1\n"
      "MOVE LF@param1 LF@%%1\n"
      "DEFVAR LF@param2\n"
      "MOVE LF@param2 LF@%%2\n"

      "DEFVAR LF@%%str_temp1\n"
      "DEFVAR LF@%%str_temp2\n"

      "STRLEN LF@%%retval LF@param1\n"
      "JUMPIFEQ %%assert_zero_end LF@%%retval int@0\n"
      "SUB LF@%%retval LF@%%retval int@1\n"
      "LT LF@%%str_temp1 LF@param2 int@0\n"
      "JUMPIFEQ %%assert_zero_end LF@%%str_temp1 bool@true\n"
      "GT LF@%%str_temp1 LF@param2 LF@%%retval\n"
      "JUMPIFEQ %%assert_zero_end LF@%%str_temp1 bool@true\n"
      "STRI2INT LF@%%retval LF@param1 LF@param2\n"
      "JUMP %%ord_end\n"

      "LABEL %%assert_zero_end\n"
      "MOVE LF@%%retval int@0\n"

      "LABEL %%ord_end\n"
      "POPFRAME\n"
      "RETURN\n\n"
  );

  get_token();

  // Calling the main non-terminal function
  PROG();

  return 0;
}
