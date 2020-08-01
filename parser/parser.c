///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Syntax analysis (Recursive descent) 	                           //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
///////////////////////////////////////////////////////////////////////////////////

#include "parser.h"

int PROG() {
    printf("LABEL MAIN\n");
    print_instruction_no_args("CREATEFRAME");
    print_instruction_no_args("PUSHFRAME");
    print_instruction("DEFVAR", "LF@%s\n", FUNC_RETURN_VARNAME);
    print_instruction("MOVE", "LF@%s nil@nil\n", FUNC_RETURN_VARNAME);
    print_instruction("DEFVAR", "LF@%s\n", COND_EXPR_RESULT_VARNAME);
    print_instruction("MOVE", "LF@%s nil@nil\n", COND_EXPR_RESULT_VARNAME);
    print_instruction("DEFVAR", "GF@%s1\n", TEMP_EXP_CONVERTION_VARNAME);
    print_instruction("DEFVAR", "GF@%s2\n", TEMP_EXP_CONVERTION_VARNAME);
    debug_info("start PROG\n");
    token_prettyprint(token);
    ifj18_obj_t *main_func = init_func();
    main_func->obj_type.func.func_name = "main";
    debug_info("init_func() complete\n");
    int statement_return;
    while (1) {
        switch (token->type) {
            case TOKEN_DEF:
                DEFINE_FUNCTION();
                continue;
            case TOKEN_END:
                error(SYNTAX_ERROR, "Closing tag end without matching opening one. Expected end. Got: %s",
                      ifj18_token_type_string(token->type));
            case TOKEN_ELSE:
                error(SYNTAX_ERROR, "Unpexpected else");
            default:
                statement_return = STATEMENT(main_func);
                if (statement_return == TOKEN_END_OF_FILE) {
                    return 1;
                } else if (statement_return == TOKEN_DEF) {
                    continue;
                } else {
                    error(SYNTAX_ERROR, "Unexpected instruction: %s", ifj18_token_type_string(token->type));
                }
                break;
        }
        get_token();

    }
}

int DEFINE_FUNCTION() {
    char paren_found = 1;
    debug_info("Start define_function()\n");
    ifj18_obj_t *func = init_func();
    debug_info("init_func() complete \n");
    get_token();

    token_prettyprint(token);


    check_token_type_msg(TOKEN_ID, SYNTAX_ERROR, 1, "Incorrect token after def");

    debug_info("Check hash_has\n");

    ifj18_obj_t *check = ifj18_hash_get((kh_value_t *)global_table, token->value->as_string->value);
    if (check != NULL) {
        error(DEFINITION_ERROR, "Function has been defined already");
    }

    debug_info("Set hash funct\n");

    char *func_name = token->value->as_string->value;
    func->obj_type.func.func_name = token->value->as_string->value;

    ifj18_hash_set((kh_value_t *) global_table, func_name, func);

    //debug_info("Hash function set %d\n", ifj18_hash_has((kh_value_t *) global_table, func_name));

    get_token();
    if (token->type != TOKEN_LPAREN) {
      error(SYNTAX_ERROR, "left paren expected");
    }
    get_token();

    string *parameters[30];

    PARAM_LIST(func, paren_found, parameters);

    for (int i = 0; i < func->obj_type.func.params_num; i++) {
        debug_info("parameters[%d] %s\n", i, parameters[i]->value);
    }

    print_instruction("JUMP", FUNC_JUMP_AFTER_TEMPLATE, func_name);
    printf("\n");

    debug_info("Function: %s, param count: %d\n", func_name, func->obj_type.func.params_num);
    print_instruction("LABEL", FUNCTION_LABEL_TEMPLATE, func_name);
    printf("\n");

    print_instruction_no_args("PUSHFRAME");

    print_instruction("DEFVAR", "LF@%s\n", FUNC_RETURN_VARNAME);
    print_instruction("DEFVAR", "LF@%s\n", COND_EXPR_RESULT_VARNAME);
    print_instruction("MOVE", "LF@%s nil@nil\n", COND_EXPR_RESULT_VARNAME);
    print_instruction("MOVE", "LF@%s nil@nil\n", FUNC_RETURN_VARNAME);

    for (int i = 0; i < func->obj_type.func.params_num; i++) {
        debug_info("parameters[%d] %s\n", i, parameters[i]->value);
        print_instruction("DEFVAR", "LF@%s\n", parameters[i]->value);
        print_instruction("DEFVAR", "GF@$$_expr_res_%s$$%s\n", func->obj_type.func.func_name, parameters[i]->value);
        print_instruction("MOVE", "LF@%s LF@%%%d\n", parameters[i]->value, i + 1);
    }

    get_token();

    debug_info("Statements inside of function\n");

    if (STATEMENT(func) != TOKEN_END) {
        error(SYNTAX_ERROR, "Unterminated function declaration");
    }

    get_token();
    token_prettyprint(token);
    debug_info("End of function found\n");

    print_instruction_no_args("POPFRAME");
    print_instruction_no_args("RETURN");

    print_instruction("LABEL", FUNC_JUMP_AFTER_TEMPLATE, func_name);
    printf("\n");

    return 0;

}

int STATEMENT(ifj18_obj_t *func) {
    debug_info("Inside of STATEMENT function");
    ifj18_obj_t *check;
    int id_len;
    token_prettyprint(token);
    int type = 0;
    char *token_id_name;
    char if_complete = 0;
    while (1) {
        switch (token->type) {
            case TOKEN_END_OF_FILE:
                return TOKEN_END_OF_FILE;
            case TOKEN_END_OF_LINE:
                break;
            case TOKEN_ID:
                id_len = strlen(token->value->as_string->value)-1;
                token_id_name = token->value->as_string->value;

                if(strcmp(token_id_name, "def") == 0  ||
                   strcmp(token_id_name, "do") == 0   ||
                   strcmp(token_id_name, "else") == 0 ||
                   strcmp(token_id_name, "end") == 0  ||
                   strcmp(token_id_name, "if") == 0   ||
                   strcmp(token_id_name, "not") == 0  ||
                   strcmp(token_id_name, "nil") == 0  ||
                   strcmp(token_id_name, "then") == 0 ||
                   strcmp(token_id_name, "while") == 0) error(SYNTAX_ERROR, "Unexpected key word");

                get_token();
                if (token->type == TOKEN_OP_ASSIGN) {
                    check = ifj18_hash_get(func->obj_type.func.local_symtable, token_id_name);
                    if (check == NULL){ // variable was not defined in this function
                      print_instruction("DEFVAR", "LF@%s\n", token_id_name);
                      print_instruction("DEFVAR", "GF@$$_expr_res_%s$$%s\n", func->obj_type.func.func_name, token_id_name);
                      print_instruction("MOVE", "LF@%s nil@nil\n", token_id_name);
                    }

                    if(token_id_name[id_len] == '?' || token_id_name[id_len] == '!')
                      error(LEXICAL_ERROR, "variable id should not end with ! or ?");

                    debug_info("Sent LF@%s as variable to save expr\n", token_id_name);
                    get_token();
                    type = expression(func, token_id_name);
                    print_instruction("MOVE", "GF@$$_expr_res_%s$$%s " "LF@%s\n", func->obj_type.func.func_name, token_id_name, token_id_name);
                    print_instruction("MOVE", "LF@%s LF@%s\n", FUNC_RETURN_VARNAME, token_id_name);

                    ifj18_obj_t *obj = init_var();
                    obj->obj_type.var.type = type;
                    obj->obj_type.var.var_name = token_id_name;
                    ifj18_hash_set((kh_value_t *) func->obj_type.func.local_symtable, token_id_name, obj);
                }
                else if(token->type == TOKEN_END_OF_LINE || token->type == TOKEN_END_OF_FILE){
                    check = ifj18_hash_get(func->obj_type.func.local_symtable, token_id_name);
                    if (check == NULL) error(DEFINITION_ERROR, "unknown variable");
                    print_instruction("MOVE", "LF@%s LF@%s\n", FUNC_RETURN_VARNAME, token_id_name);
                }
                else {
                    expression(func, FUNC_RETURN_VARNAME);
                }
                break;
            case TOKEN_IF:
                PARSE_IF(func);
                break;
            case TOKEN_DEF:
                return TOKEN_DEF;
            case TOKEN_WHILE:
                PARSE_WHILE(func);
                break;
            case TOKEN_PRINT:
                PARSE_PRINT(func);
                break;
            case TOKEN_END:
                return TOKEN_END;
            case TOKEN_ELSE:
                return TOKEN_ELSE;
            default:
                debug_info("Default branch switched\n");
                expression(func, FUNC_RETURN_VARNAME);
                break;
        }
        get_token();
        token_prettyprint(token);
    }

}

char *get_3ac_token_type() {
    switch (token->type) {
        case TOKEN_STRING:
            return "string";
        case TOKEN_INT:
            return "int";
        case TOKEN_ID:
            return "LF";
        case TOKEN_FLOAT:
            return "float";
        case TOKEN_NIL:
            return "nil";
    }
}


void PARSE_PRINT(ifj18_obj_t *func) {
  int param_found = 0;
  int param_count = 0;
  get_token();

  if (token->type == TOKEN_LPAREN) {
      param_found = 1;
      get_token();
  }

    while (1) {
        param_count++;
        if (!param_found && token->type == TOKEN_RPAREN) {
            error(SYNTAX_ERROR, "Closing parenthesis without opening one.");
        } else if ((param_found && token->type == TOKEN_RPAREN)) {
            return;
        }
        parmas:

        printf("WRITE ");
        ifj18_obj_t *check;
        switch (token->type) {
            case TOKEN_ID:
                check = ifj18_hash_get((kh_value_t *)func->obj_type.func.local_symtable, token->value->as_string->value);
                if (check == NULL){
                    error(DEFINITION_ERROR, "Variable is not defined");
                }
            case TOKEN_STRING:
                printf("%s@%s", get_3ac_token_type(), token->value->as_string->value);
                break;
            case TOKEN_INT:
                printf("%s@%d", get_3ac_token_type(), token->value->as_int);
                break;
            case TOKEN_FLOAT:
                printf("%s@%a", get_3ac_token_type(), token->value->as_float);
                break;
            case TOKEN_NIL:
                printf("string@");

        }

        print_instruction("\nWRITE", "string@\\010\n");
        get_token();

        if (token->type == TOKEN_COMMA) {
            get_token();
            goto parmas;
        }
        else if(token->type  == TOKEN_RPAREN || token->type == TOKEN_END_OF_LINE){
            return;
        }
        else if(!param_found && token->type == TOKEN_RPAREN) {
            error(SYNTAX_ERROR, "Closing parenthesis without opening one.");
        }
        else{
            error(SYNTAX_ERROR, "Unknown token");
        }

    }
    print_instruction("MOVE", "LF@%s nil@nil\n", FUNC_RETURN_VARNAME);
}


void PARSE_IF(ifj18_obj_t *func) {
    static int if_count = 0;
    if_count++;

    get_token();

    expression(func, COND_EXPR_RESULT_VARNAME);

    print_instruction("JUMPIFEQ", "$$_IF_STATEMENT_%d LF@%s bool@true\n", if_count, COND_EXPR_RESULT_VARNAME);
    print_instruction("JUMPIFNEQ", "$$_IF_ELSE_STATEMENT_%d LF@%s bool@true\n", if_count, COND_EXPR_RESULT_VARNAME);

    check_token_type(TOKEN_THEN, SYNTAX_ERROR, 1);


    get_token();

    print_instruction("LABEL", "$$_IF_STATEMENT_%d\n", if_count);

    if (STATEMENT(func) != TOKEN_ELSE) {
        error(SYNTAX_ERROR, "Expected else");
    }

// :D
    print_instruction("JUMP", "$$_IF_AFTER_%d\n", if_count);

    print_instruction("LABEL", "$$_IF_ELSE_STATEMENT_%d\n", if_count);


    debug_info("If terminated with else");

    get_token();

    if (STATEMENT(func) != TOKEN_END) {
        error(SYNTAX_ERROR, "Expected end for if");
    }

    debug_info("If terminated with end");

    print_instruction("LABEL", "$$_IF_AFTER_%d\n", if_count);

    get_token();

}

void PARSE_WHILE(ifj18_obj_t *func){
  static int while_count = 0;
  while_count++;

  get_token();
  expression(func, COND_EXPR_RESULT_VARNAME);

  print_instruction("JUMPIFEQ", "$$_WHILE_STATEMENT_%d LF@%s bool@true\n", while_count, COND_EXPR_RESULT_VARNAME);
  print_instruction("JUMPIFNEQ", "$$_WHILE_AFTER_%d LF@%s bool@true\n", while_count, COND_EXPR_RESULT_VARNAME);

  check_token_type(TOKEN_DO, SYNTAX_ERROR, 1);
  get_token();
  print_instruction("LABEL", "$$_WHILE_STATEMENT_%d\n", while_count);

  if (STATEMENT(func) != TOKEN_END) {
      error(SYNTAX_ERROR, "Expected end");
  }
  print_instruction("JUMPIFEQ", "$$_WHILE_STATEMENT_%d LF@%s bool@true\n", while_count, COND_EXPR_RESULT_VARNAME);
  print_instruction("JUMPIFNEQ", "$$_WHILE_AFTER_%d LF@%s bool@true\n", while_count, COND_EXPR_RESULT_VARNAME);
  print_instruction("LABEL", "$$_WHILE_AFTER_%d\n", while_count);
  get_token();
}

void PARAM_LIST(ifj18_obj_t *func, char param_found, string **parameters) {
    token_prettyprint(token);
    if (!param_found && token->type == TOKEN_RPAREN) {
        error(SYNTAX_ERROR, "Closing parenthesis without opening one.");
    } else if ((param_found && token->type == TOKEN_RPAREN)) {
        return;
    }

    check_token_type_msg(TOKEN_ID, SYNTAX_ERROR, 1, "function parameter expected to be identifier");


    ifj18_obj_t *check = ifj18_hash_get((kh_value_t *)func->obj_type.func.local_symtable, token->value->as_string->value);
    if (check != NULL) {
        error(DEFINITION_ERROR, "Implicit declaration of function argument");
    }

    ifj18_obj_t *object = init_var();
    //object->obj_type.var.var_name = token->value->as_string->value;
    ifj18_hash_set((kh_value_t *) func->obj_type.func.local_symtable, token->value->as_string->value, object);
    //print_instruction("DEFVAR", "GF@$$_expr_res_%s$$%s\n", func->obj_type.func.func_name, object->obj_type.var.var_name);


    parameters[func->obj_type.func.params_num++] = token->value->as_string;

    get_token();

    if (token->type == TOKEN_COMMA) {
        get_token();
        check_token_type_msg(TOKEN_ID, SYNTAX_ERROR, 1, "Expecting an identifier after comma");
    }
    else if(token->type != TOKEN_RPAREN){
        error(SYNTAX_ERROR, "expected right paren or comma");
    }

    PARAM_LIST(func, param_found, parameters);

}
