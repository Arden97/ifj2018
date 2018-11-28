#include "scanner.h"
#include "semantics.h"
#include "stdlib.h"
#include "symtable.h"
#include <stdio.h>

int PROG();
int DEFINE_FUNCTION();
int STATEMENT_LIST(ifj18_obj_t *func);
int STATEMENT();
int CALL_ASIGN();
int PARAM_LIST(ifj18_obj_t *func);
int NEXT_PARAM();
int parse_id();
int parse_if();
int parse_while();
int parse_print();
int parse_inputf();
int parse_inputs();
int parse_inputi();
int expression();

int PROG() {
  switch (token->type) {
  case TOKEN_DEF:
    if (DEFINE_FUNCTION()) {
      if (token->type == TOKEN_END_OF_LINE) {
        get_token();
        return PROG();
      }
    }
    break;
  case TOKEN_END_OF_LINE:
    get_token();
    return PROG();
  case TOKEN_IF:
    if (STATEMENT()) {
      if (token->type == TOKEN_THEN) {
        get_token();
        if (token->type == TOKEN_END_OF_LINE) {
          get_token();
          return PROG();
        }
      }
    }
    break;
  case TOKEN_ID:
    if (STATEMENT()) {
      if (token->type == TOKEN_END_OF_LINE) {
        get_token();
        return PROG();
      }
    }
    break;
  case TOKEN_WHILE:
    if (STATEMENT()) {
      if (token->type == TOKEN_END_OF_LINE) {
        get_token();
        return PROG();
      } else if (token->type == TOKEN_DO) {
        get_token();
        if (token->type == TOKEN_END_OF_LINE) {
          get_token();
          return PROG();
        }
      }
    }
    break;
  case TOKEN_END_OF_FILE:
    return 0;
    break;
  }
}

int DEFINE_FUNCTION() {
  ifj18_obj_t *func = init_func();

  get_token();

  if (token->type == TOKEN_ID) {

    if (ifj18_hash_has(global_table, token->value->as_string)) {
      error(SEMANTIC_ERROR, "id already defined");
    }

    ifj18_hash_set(global_table, token->value->as_string, func);

    get_token();

    if (token->type == TOKEN_LPAREN) {
      if (PARAM_LIST(func)) {
        get_token();
        char *type;

        if (token->type == TOKEN_END_OF_LINE) {
          get_token();
          if (STATEMENT_LIST(func)) {
            switch (func->obj_type.func.return_var->type) {
            case IFJ18_TYPE_INT:
              type = "int@0";
              break;
            case IFJ18_TYPE_STRING:
              type = "string@";
              break;
            case IFJ18_TYPE_FLOAT:
              type = "float@0.0";
              break;
            }
            printf("PUSHS %s\n \
                  POPFRAME\n \
                  RETURN\n",
                   type);
          }
        }
      }
    }
  }
}

int STATEMENT_LIST(ifj18_obj_t *func) {
  switch (token->type) {
  case TOKEN_END_OF_LINE:
    get_token();
    return STATEMENT_LIST(func);
  case TOKEN_END:
    get_token();
    return 1;
  case TOKEN_ID:
  case TOKEN_IF:
  case TOKEN_WHILE:
  case TOKEN_PRINT:
  case TOKEN_INPUTF:
  case TOKEN_INPUTS:
  case TOKEN_INPUTI:
    STATEMENT();
    return STATEMENT_LIST(func);
  default:
    return 0;
    break;
  }
}

int STATEMENT() {
  switch (token->type) {
  case TOKEN_END_OF_LINE:
    return 1;
  case TOKEN_ID:
    // return parse_id();
  case TOKEN_IF:
    // return parse_if();
  case TOKEN_WHILE:
    // return parse_while();
  case TOKEN_PRINT:
    // return parse_print();
  case TOKEN_INPUTF:
    // return parse_inputf();
  case TOKEN_INPUTS:
    // return parse_inputs();
  case TOKEN_INPUTI:
    // return parse_inputi();
  default:
    return 0;
  }
}

int PARAM_LIST(ifj18_obj_t *func) {
  get_token();

  if (token->type == TOKEN_COMMA) {
    return 0;
  }

  if (token->type == TOKEN_RPAREN) {
    return 1;
  }

  if (token->type != TOKEN_COMMA) {
    // if (!expression())
    //   return 0;
  }

  if (token->type == TOKEN_COMMA) {
    func->obj_type.func.params_num++;
    return PARAM_LIST(func);
  }

  return 0;
}