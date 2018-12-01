#include "parser.h"

int PROG() {
  ifj18_obj_t *main_func = init_func();

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
    if (STATEMENT(main_func)) {
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
    if (STATEMENT(main_func)) {
      if (token->type == TOKEN_END_OF_LINE) {
        get_token();
        return PROG();
      }
    }
    break;
  case TOKEN_WHILE:
    if (STATEMENT(main_func)) {
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
    STATEMENT(func);
    return STATEMENT_LIST(func);
  default:
    return 0;
    break;
  }
}

int STATEMENT(ifj18_obj_t *func) {
  switch (token->type) {
  case TOKEN_END_OF_LINE:
    return 1;
  case TOKEN_ID:
    get_token();
    if (token->type == TOKEN_OP_ASSIGN) {
      return expression(func);
    }
  case TOKEN_IF:
  case TOKEN_WHILE:
  case TOKEN_PRINT:
    return 0;
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