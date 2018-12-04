#include "parser.h"

int PROG() {
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%s\n", FUNC_RETURN_VARNAME);
    printf("# start PROG\n");
    token_prettyprint(token);
    ifj18_obj_t *main_func = init_func();
    printf("# init_func() complete\n");
    while (1) {
        switch (token->type) {
            case TOKEN_DEF:
                DEFINE_FUNCTION();
                break;
            case TOKEN_END_OF_LINE:
                get_token();
                break;
            default:
                if (STATEMENT(main_func)) {
                    return 1;
                };
                break;
        }

    }
}

int DEFINE_FUNCTION() {
    char paren_found = 0;
    printf("# start define_function()\n");
    ifj18_obj_t *func = init_func();
    printf("# nit_func() complete \n");
    get_token();

    token_prettyprint(token);


    check_token_type_msg(TOKEN_ID, SYNTAX_ERROR, 1, "Incorrect token after def");

    printf("# Check hash_has\n");


    if (ifj18_hash_has(global_table, token->value->as_string->value)) {
        printf("inside of hash_has error\n");
        error(SEMANTIC_ERROR, "ID has been defined already");
    }

    printf("# Set hash funct\n");

    ifj18_hash_set(global_table, token->value->as_string->value, func);

    char *func_name = token->value->as_string->value;

    get_token();
//    printf("pizdozo: %s\n",func_name);

    if (token->type == TOKEN_LPAREN) {
        paren_found = 1;
        get_token();
    }

    PARAM_LIST(func, paren_found);

    printf("JUMP ");
    printf(FUNC_JUMP_AFTER_TEMPLATE, func_name);
    printf("\n\n");

    printf("#Function: %s, param count: %d\n", func_name, func->obj_type.func.params_num);
    printf("LABEL");
    printf(FUNCTION_LABEL_TEMPLATE, func_name);
    printf("\n");

    printf("# Statements inside of function\n");
    int statement_token;
    while (1) {
        get_token();
        token_prettyprint(token);
        statement_token = STATEMENT(func);
        if(statement_token == TOKEN_END_OF_FILE){
            error(SYNTAX_ERROR, "Unexpected EOF in function definition");
        }
        else if(statement_token == TOKEN_END){
            return 1;
        }

    }
}

int STATEMENT(ifj18_obj_t *func) {
    token_prettyprint(token);
    char *token_id_name;

    switch (token->type) {
        case TOKEN_END_OF_FILE:
            return TOKEN_END_OF_FILE;

        case TOKEN_ID:
            token_id_name = token->value->as_string->value;
            get_token();
            if (token->type == TOKEN_OP_ASSIGN) {
                printf("DEF LF@%s\n", token_id_name);
                expression(func, token_id_name);
                printf("#Sent LF@%s as variable to save expr\n", token_id_name);
                printf("MOV LF@%s LF@%s\n", FUNC_RETURN_VARNAME, token_id_name);
            }
            else if(token->type == TOKEN_LPAREN) {
                PARSE_FUNCTION_CALL();
            }
            else {
//                expression(func, FUNC_RETURN_VARNAME);
            }


            break;
        case TOKEN_IF:
        case TOKEN_WHILE:
        case TOKEN_PRINT:
            return 0;
        case TOKEN_END:
            return TOKEN_END;
        default:
            error(SYNTAX_ERROR, "Unexpected token");
    }
}

void PARSE_FUNCTION_CALL(){
    if(!ifj18_hash_has(global_table, token->value->as_string->value)){

    }
}

void PARAM_LIST(ifj18_obj_t *func, char param_found) {
    token_prettyprint(token);
    if (!param_found && token->type == TOKEN_RPAREN) {
        error(SYNTAX_ERROR, "Closing parenthesis without opening one.");
    } else if ((param_found && token->type == TOKEN_RPAREN) || (!param_found && token->type == TOKEN_END_OF_LINE)) {
        return;
    }

    check_token_type_msg(TOKEN_ID, SYNTAX_ERROR, 1, "function parameter expected to be identifier");

    if (ifj18_hash_has(func->obj_type.func.local_symtable, token->value->as_string->value)) {
        error(SEMANTIC_ERROR, "Implicit declaration of function argument");
    }


    ifj18_hash_set(func->obj_type.func.local_symtable, token->value->as_string->value, NULL);

    func->obj_type.func.params_num++;

    get_token();

    if (token->type == TOKEN_COMMA) {
        get_token();
        check_token_type_msg(TOKEN_ID, SYNTAX_ERROR, 1, "Expecting identifier after comma");
    }

    PARAM_LIST(func, param_found);
}
