#include "scanner.h"
#include "semantics.h"
#include "stdlib.h"
#include "symtable.h"
#include <stdio.h>

int PROG();
int DEFINE_FUNCTION();
int STATEMENT_LIST();
int STATEMENT();
int ELSE_BRANCH();
int CALL_ASIGN();
int PARAM_LIST(ifj18_obj_t *func);
int NEXT_PARAM();

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

    get_token();

    if (PARAM_LIST(func)) {
    }
  }

  ifj18_hash_set(global_table, token->value->as_string, func);
}