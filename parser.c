#include "parser.h"
#include "semantics.h"

int PROG() {
    print_instruction_no_args("CREATEFRAME");
    print_instruction_no_args("PUSHFRAME");
    print_instruction("DEFVAR", "LF@%s\n", FUNC_RETURN_VARNAME);
    print_instruction("DEFVAR", "LF@%s\n", COND_EXPR_RESULT_VARNAME);
    debug_info("start PROG\n");
    token_prettyprint(token);
    ifj18_obj_t *main_func = init_func();
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
                }
                else if(statement_return == TOKEN_DEF){
                    continue;
                }
                else{
                    error(SYNTAX_ERROR, "Unexpected instruction: %s", ifj18_token_type_string(token->type));
                }
                break;
        }
        get_token();

    }
}

int DEFINE_FUNCTION() {
    char paren_found = 0;
    debug_info("Start define_function()\n");
    ifj18_obj_t *func = init_func();
    debug_info("init_func() complete \n");
    get_token();

    token_prettyprint(token);


    check_token_type_msg(TOKEN_ID, SYNTAX_ERROR, 1, "Incorrect token after def");

    debug_info("Check hash_has\n");


    if (ifj18_hash_has((kh_value_t *) global_table, token->value->as_string->value)) {
        printf("inside of hash_has error\n");
        error(SEMANTIC_ERROR, "ID has been defined already");
    }

    debug_info("Set hash funct\n");

    char *func_name = token->value->as_string->value;

    ifj18_hash_set((kh_value_t *) global_table, func_name, func);

    debug_info("Hash function set %d\n", ifj18_hash_has((kh_value_t *) global_table, func_name));

    get_token();

    if (token->type == TOKEN_LPAREN) {
        paren_found = 1;
        get_token();
    }
    string *parameters[30];


    PARAM_LIST(func, paren_found, parameters);

    for(int i=0; i<func->obj_type.func.params_num; i++){
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
    print_instruction("MOVE", "LF@%s nil@nil\n", FUNC_RETURN_VARNAME);

    for(int i=0; i<func->obj_type.func.params_num; i++){
        debug_info("parameters[%d] %s\n", i, parameters[i]->value);
        print_instruction("MOVE", "LF@%s LF@%%%d\n",parameters[i]->value, i+1);
    }

    get_token();

    debug_info("Statements inside of function\n");

    if (STATEMENT(func) != TOKEN_END){
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
    token_prettyprint(token);
    char *token_id_name;
    char if_complete = 0;
    while(1){
        switch (token->type) {
            case TOKEN_END_OF_FILE:
                return TOKEN_END_OF_FILE;
            case TOKEN_END_OF_LINE:
                break;
            case TOKEN_ID:
                token_id_name = token->value->as_string->value;
                get_token();
                if (token->type == TOKEN_OP_ASSIGN) {
                    print_instruction("DEFVAR", "LF@%s\n", token_id_name);
                    debug_info("Sent LF@%s as variable to save expr\n", token_id_name);
                    get_token();
                    expression(func, token_id_name);
                    print_instruction("MOVE", "LF@%s LF@%s\n", FUNC_RETURN_VARNAME, token_id_name);
                } else {
                    expression(func, FUNC_RETURN_VARNAME);
                }
                break;
            case TOKEN_IF:
                PARSE_IF(func);
                break;
            case TOKEN_DEF:
                return TOKEN_DEF;
            case TOKEN_WHILE:
            case TOKEN_PRINT:
                return 0;
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
void PARSE_IF(ifj18_obj_t *func){
    get_token();
    if(STATEMENT(func) != TOKEN_ELSE){
        error(SYNTAX_ERROR, "Expected else");
    }
    debug_info("If terminated with else");

    get_token();

    if(STATEMENT(func) != TOKEN_END){
        error(SYNTAX_ERROR, "Expected end for if");
    }
    debug_info("If terminated with end");
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

    if (ifj18_hash_has((kh_value_t *) func->obj_type.func.local_symtable, token->value->as_string->value)) {
        error(SEMANTIC_ERROR, "Implicit declaration of function argument");
    }



    ifj18_hash_set((kh_value_t *) func->obj_type.func.local_symtable, token->value->as_string->value, NULL);


    parameters[func->obj_type.func.params_num++] = token->value->as_string;

    get_token();

    if (token->type == TOKEN_COMMA) {
        get_token();
        check_token_type_msg(TOKEN_ID, SYNTAX_ERROR, 1, "Expecting identifier after comma");
    }

    PARAM_LIST(func, param_found, parameters);

}
