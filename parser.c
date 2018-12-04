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
    while (1) {
        switch (token->type) {
            case TOKEN_DEF:
                DEFINE_FUNCTION();
                break;
            case TOKEN_END_OF_LINE:
                get_token();
                break;
            case TOKEN_END:
                error(SYNTAX_ERROR, "Closing tag end without matching opening one.");
            default:
                if (STATEMENT(main_func) == TOKEN_END_OF_FILE) {
                    return 1;
                }
                get_token();
        }

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
    int statement_token;

    while (1) {
        debug_info("Current token: ");
        token_prettyprint(token);
        statement_token = STATEMENT(func);
        if (statement_token == TOKEN_END) {
            get_token();
            break;
        }
        else if(statement_token == TOKEN_END_OF_FILE){
            error(SYNTAX_ERROR, "Unexpected EOF\n");
        }
    }

    debug_info("End of function found\n");

    print_instruction_no_args("POPFRAME");
    print_instruction_no_args("RETURN");

    print_instruction("LABEL", FUNC_JUMP_AFTER_TEMPLATE, func_name);
    printf("\n");

    return 1;

}

int STATEMENT(ifj18_obj_t *func) {
    token_prettyprint(token);
    char *token_id_name;

    switch (token->type) {
        case TOKEN_END_OF_FILE:
            return TOKEN_END_OF_FILE;
        case TOKEN_END_OF_LINE:
            get_token();
            return TOKEN_END_OF_LINE;
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
            get_token();
        case TOKEN_WHILE:
        case TOKEN_PRINT:
            return 0;
        case TOKEN_END:
            return TOKEN_END;
        default:
            expression(func, FUNC_RETURN_VARNAME);
            break;
    }
    return 1;
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
