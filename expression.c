#include "expression.h"

char precedence_table[17][17] = {
    //        | ADD | MUL| SUB| DIV|IDIV| LT | GT | LTE| GTE| EQ | NEQ| PL | PR | ID | FNC| CM | END|
    //        |   + |  * |  - |  / |  \ |  < |  > | <= | >= |  = | != |  ( |  ) |  i |  f |  , |   $|
    //-----------------------------------------------------------------------------------------------
    {/* ADD| + |*/ '>', '<', '>', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '<', '>', '>'},
    {/* MUL| * |*/ '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '<', '>', '>'},
    {/* SUB| - |*/ '>', '<', '>', '<', '<', '>', '>', '>', '>', '>', '>', '<', '>', '<', '<', '>', '>'},
    {/* DIV| / |*/ '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '<', '>', '>'},
    {/* IDI| \ |*/ '>', '<', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '<', '>', '>'},
    {/* LT | < |*/ '<', '<', '<', '<', '<', '#', '#', '#', '#', '#', '#', '<', '>', '<', '<', '#', '>'},
    {/* GT | > |*/ '<', '<', '<', '<', '<', '#', '#', '#', '#', '#', '#', '<', '>', '<', '<', '#', '>'},
    {/* LTE|<= |*/ '<', '<', '<', '<', '<', '#', '#', '#', '#', '#', '#', '<', '>', '<', '<', '#', '>'},
    {/* GTE|>= |*/ '<', '<', '<', '<', '<', '#', '#', '#', '#', '#', '#', '<', '>', '<', '<', '#', '>'},
    {/* EQ | = |*/ '<', '<', '<', '<', '<', '#', '#', '#', '#', '#', '#', '<', '>', '<', '<', '#', '>'},
    {/* NEQ|!= |*/ '<', '<', '<', '<', '<', '#', '#', '#', '#', '#', '#', '<', '>', '<', '<', '#', '>'},
    {/* PL | ( |*/ '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '<', '<', '=', '#'},
    {/* PR | ) |*/ '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '#', '>', '#', '#', '>', '>'},
    {/* ID | i |*/ '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '#', '>', '#', '#', '>', '>'},
    {/* FNC| f |*/ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '=', '#', '#', '#', '#', '#'},
    {/* CM | , |*/ '<', '<', '<', '<', '<', '#', '#', '#', '#', '#', '#', '<', '=', '<', '<', '=', '#'},
    {/* END| $ |*/ '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '#', '<', '<', '#', '\0'},
};

int expression(ifj18_obj_t *func) {
  if (is_function())
    return function(func);
  else
    return inf_to_post(func);
}

int is_function() {
  switch (token->type) {
  case TOKEN_CHR:
  case TOKEN_ORD:
  case TOKEN_PRINT:
  case TOKEN_INPUTS:
  case TOKEN_INPUTI:
  case TOKEN_INPUTF:
  case TOKEN_LENGTH:
    return 1;
  default:
    ifj18_obj_t *symbol = ifj18_hash_get(global_table, token->value->as_string);
    if (symbol != NULL && symbol->obj_type_flag) {
      return 1;
    } else {
      return 0;
    }
  }
}

int function(ifj18_obj_t *act_function) {
  switch (token->type) {
    case TOKEN_INPUTF:
      return inputf();
    break;

    case TOKEN_INPUTI:
      return inputi();
    break;

    case TOKEN_INPUTS:
      return inputs();
    break;

    case TOKEN_LENGTH:
      return length();
    break;

    case TOKEN_SUBSTR:
      return substr();
    break;

    case TOKEN_ORD:
      return ord();
    break;
  }

  char *key = token->value->as_string;
  ifj18_obj_t *func = ifj18_hash_get(global_table, key);
  int params_num = func->obj_type.func.params_num;

  get_token();
  if (token->type != TOKEN_LPAREN) {
    return 0;
  }

  get_token();
  if ((token->type == TOKEN_RPAREN && params_num) || (token->type != TOKEN_RPAREN && !params_num)) {
    error(ARGS_ERROR, "wrong number of parameters");
  }

  for (int i = 0; i < params_num; i++) {
    printf("PUSHS %s\n", func->obj_type.func.sparams[i]->value);
    get_token();
    if ((i != params_num-1) && (token->type != TOKEN_COMMA)){
      return 0;
    }
  }
  get_token();
  if(token->type != TOKEN_RPAREN){
    return 0;
  }
  printf("CALL %s\n", key);

}

int length() { ifj18_obj_t *func = init_func(); }
