#include "expression.h"
#include "semantics.h"

// #define apply_on_new_token(stat) get_token();stat
char precedence_table[16][16] = {
        /*            +    -    *    /    (    )    \    <    >   <=   >=    =   <>   id   lit   $ */
        /*  +  */ {'>', '>', '<', '<', '<', '>', '<', '>', '>', '>', '>', '>', '>', '<', '<', '>'},
        /*  -  */
                  {'>', '>', '<', '<', '<', '>', '<', '>', '>', '>', '>', '>', '>', '<', '<', '>'},
        /*  *  */
                  {'>', '>', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>'},
        /*  /  */
                  {'>', '>', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>'},
        /*  (  */
                  {'<', '<', '<', '<', '<', '=', '<', '<', '<', '<', '<', '<', '<', '<', '<', ' '},
        /*  )  */
                  {'>', '>', '>', '>', ' ', '>', '>', '>', '>', '>', '>', '>', '>', ' ', ' ', '>'},
        /*  \  */
                  {'>', '>', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>'},
        /*  <  */
                  {'<', '<', '<', '<', '<', '>', '<', ' ', ' ', ' ', ' ', ' ', ' ', '<', '<', '>'},
        /*  >  */
                  {'<', '<', '<', '<', '<', '>', '<', ' ', ' ', ' ', ' ', ' ', ' ', '<', '<', '>'},
        /*  <= */
                  {'<', '<', '<', '<', '<', '>', '<', ' ', ' ', ' ', ' ', ' ', ' ', '<', '<', '>'},
        /*  >= */
                  {'<', '<', '<', '<', '<', '>', '<', ' ', ' ', ' ', ' ', ' ', ' ', '<', '<', '>'},
        /*  =  */
                  {'<', '<', '<', '<', '<', '>', '<', ' ', ' ', ' ', ' ', ' ', ' ', '<', '<', '>'},
        /*  <> */
                  {'<', '<', '<', '<', '<', '>', '<', ' ', ' ', ' ', ' ', ' ', ' ', '<', '<', '>'},
        /*  ID */
                  {'>', '>', '>', '>', ' ', '>', '>', '>', '>', '>', '>', '>', '>', ' ', ' ', '>'},
        /* LIT */
                  {'>', '>', '>', '>', ' ', '>', '>', '>', '>', '>', '>', '>', '>', ' ', ' ', '>'},
        /*  $  */
                  {'<', '<', '<', '<', '<', ' ', '<', '<', '<', '<', '<', '<', '<', '<', '<', ' '},
};

char flags[4] = {0, 0, 0, 0};
#define FG_LENGTH 0
#define FG_INPUTI 1
#define FG_INPUTF 2
#define FG_INPUTS 3

int expression(ifj18_obj_t *func, char *ret_var) {
    if (is_function())
        return function(func, ret_var);
    else
        return inf_to_post(func, ret_var);
}

int is_function() {
    ifj18_obj_t *symbol = ifj18_hash_get((kh_value_t *) global_table, token->value->as_string->value);
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
            if (symbol != NULL && symbol->obj_type_flag) {
                return 1;
            } else {
                return 0;
            }
    }
}

int function(ifj18_obj_t *act_function, char *ret_var) {
    switch (token->type) {
        case TOKEN_INPUTF:
            printf("WRITE LF@%s float\n", ret_var);
            break;

        case TOKEN_INPUTI:
            printf("WRITE LF@%s int\n", ret_var);
            break;

        case TOKEN_INPUTS:
            printf("WRITE LF@%s string\n", ret_var);
            break;

        case TOKEN_LENGTH:
            // return length();
            break;

        case TOKEN_SUBSTR:
            // return substr();
            break;

        case TOKEN_ORD:
            // return ord();
            break;
    }

    // char *key = token->value->as_string->value;
    // ifj18_obj_t *func = ifj18_hash_get((kh_value_t *)global_table, key);
    // int params_num = func->obj_type.func.params_num;

    // get_token();
    // if (token->type != TOKEN_LPAREN) {
    //   return 0;
    // }

    // get_token();
    // if ((token->type == TOKEN_RPAREN && params_num) || (token->type != TOKEN_RPAREN && !params_num)) {
    //   error(ARGS_ERROR, "wrong number of parameters");
    // }

    // for (int i = 0; i < params_num; i++) {
    //   printf("PUSHS %s\n", func->obj_type.func.sparams[i].value);
    //   get_token();
    //   if ((i != params_num - 1) && (token->type != TOKEN_COMMA)) {
    //     return 0;
    //   }
    // }
    // get_token();
    // if (token->type != TOKEN_RPAREN) {
    //   return 0;
    // }
    // printf("CALL %s\n", key);
}

int shift_to_stack(ifj18_token_t *stack_token) {

    /// calculate index to the precedence table
    unsigned index_y = token->type - TOKEN_OP_PLUS;       // coordinates y
    unsigned index_x = stack_token->type - TOKEN_OP_PLUS; // coordinates x

    /// special recalculating for both types brackets
    if (stack_token->type == TOKEN_LPAREN)
        index_x = 4;
    else if (stack_token->type == TOKEN_RPAREN)
        index_x = 5;

    if (precedence_table[index_x][index_y] == '<')
        return 1;
    else
        return 0;
}

void do_until_left_bracket(ifj18_stack_t *operators_stack, ifj18_stack_t *output_stack) {

    while (!stack_empty(operators_stack)) {
        /// obtaining operands and operators from temporary stack and their relocating to the output stack
        ifj18_token_t *stack_token = stack_top(operators_stack);
        if (stack_token->type != TOKEN_LPAREN) { /// check conditions for terminates before emptying all stack
            stack_push(output_stack, stack_token);
            stack_pop(operators_stack);
        } else {
            stack_pop(operators_stack);
            break;
        }
    }
}

void psa_operation(ifj18_stack_t *operators_stack, ifj18_stack_t *output_stack, ifj18_token_t *stack_token) {
    /// case for empty stack, marks in precedence table or left bracket
    if (stack_empty(operators_stack) || shift_to_stack(stack_token) || stack_token->type == TOKEN_LPAREN) {
        stack_token = token;
        // debug_info("#Current token in psa: ");
        //        token_prettyprint(stack_token);
        stack_push(operators_stack, stack_token);
    } else {
        /// have to marks of reduction in the table and stack dont have to be empty
        while (!stack_empty(operators_stack) && !shift_to_stack(stack_token)) {
            stack_token = stack_token;
            stack_push(output_stack, stack_token);
            stack_pop(operators_stack);
            stack_token = stack_top(operators_stack);
        }
        stack_token = token;
        stack_push(operators_stack, stack_token);
    }
}



void print_operation_operand(ifj18_obj_t *operand, char *prefix, int type) {
    if (strlen(operand->obj_type.var.var_name) != 0) {
        prefix = "LF";
        printf("%s@%s", prefix, operand->obj_type.var.var_name);

    } else {
        if (type == IFJ18_TYPE_INT) {
            printf("%s@%d", prefix, operand->obj_type.var.value.as_int);
        } else {
            printf("%s@%f", prefix, operand->obj_type.var.value.as_float);
        }
    }
}

void convert_operand(ifj18_obj_t *operand) {
    debug_info("Convert operand: %s\n", operand->obj_type.var.var_name);
    if (strlen(operand->obj_type.var.var_name) != 0) {
        print_instruction("INT2FLOAT", "LF@%s LF@%s\n", TEMP_EXP_CONVERTION_VARNAME, operand->obj_type.var.var_name);
    } else {
        print_instruction("INT2FLOAT", "LF@%s int@%d\n", TEMP_EXP_CONVERTION_VARNAME,
                          operand->obj_type.var.value.as_int);
    }
}

void generate_3ac_expressions(char *prefix_1, ifj18_obj_t *operand_1, char *prefix_2, ifj18_obj_t *operand_2,
                              ifj18_obj_t *tmp_var_obj,
                              ifj18_var type1, ifj18_var type2, char *operation, int operations_count) {

    int flag1 = 0, flag2 = 0, type = 0;

    if (type1 == IFJ18_TYPE_FLOAT || type2 == IFJ18_TYPE_FLOAT) {
        type = IFJ18_TYPE_FLOAT;
    } else {
        type = IFJ18_TYPE_INT;
    }

    debug_info("Explicit type: %d\n", type);
    if (type1 != type) {
        flag1 = 1;
        convert_operand(operand_1);
    } else if (type2 != type) {
        convert_operand(operand_2);
        flag2 = 1;
    }

    print_instruction(operation, "LF@%s ", tmp_var_obj->obj_type.var.var_name);

    if (flag2) {
        printf("LF@%s", TEMP_EXP_CONVERTION_VARNAME);
    } else {
        print_operation_operand(operand_2, prefix_2, type2);
    }
    printf(" ");

    if (flag1) {
        printf("LF@%s", TEMP_EXP_CONVERTION_VARNAME);
    } else {
        print_operation_operand(operand_1, prefix_1, type1);
    }

    printf("\n");

    tmp_var_obj->obj_type.var.type = type;

    debug_info("tmpvar Type:%d\n", tmp_var_obj->obj_type.var.type);
}

void set_object_value(ifj18_token_t *token_d, ifj18_obj_t *func, ifj18_stack_t *stack) {
    ifj18_obj_t *var;
    if (token_d->type == TOKEN_ID) {
        var = find_var(token_d, func);
    }
    else {
        var = init_var();
    }

    if (token_d->type == TOKEN_INT) {
        debug_info("INT PASSED");
        var->obj_type.var.value.as_int = token_d->value->as_int;
        var->obj_type.var.type = IFJ18_TYPE_INT;
    } else if (token_d->type == TOKEN_FLOAT) {
        debug_info("FLOAT PASSED");
        var->obj_type.var.value.as_float = token_d->value->as_float;
        var->obj_type.var.type = IFJ18_TYPE_FLOAT;
    }

    debug_info("Object type: %d\n", var->obj_type.var.type);

    stack_push(stack, var);
}

char *get_bytecode_objtype(ifj18_obj_t *operand) {
    if (operand->obj_type.var.type == IFJ18_TYPE_INT) {
        return "int";
    } else if (operand->obj_type.var.type == IFJ18_TYPE_FLOAT) {
        return "float";
    } else if (operand->obj_type.var.type == IFJ18_TYPE_STRING) {
        return "string";
    } else if (strlen(operand->obj_type.var.var_name) != 0) {
        return "LF";
    } else {
        error(INTERNAL_ERROR, "Unknown operand: %f. Type: %d", operand->obj_type.var.value.as_float,
              operand->obj_type.var.type);
    }
}

int post_to_instr(ifj18_stack_t *postfix_stack, ifj18_obj_t *act_function, char *ret_var, int operations_count) {
     stack_print(postfix_stack);
    // printf("-----------\n");
    int while_opened = 0;
    int type;
    ifj18_stack_t *output_stack = stack_init();

    debug_info("post_to_insta()\n");
    stack_print(postfix_stack);

    print_instruction_no_args("CREATEFRAME");
    print_instruction_no_args("PUSHFRAME");

    for (int i = 1; i <= operations_count; ++i) {
        print_instruction("DEFVAR", "LF@%s_%d\n", TEMP_EXPRESSION_VARNAME, i);
    }
    print_instruction("DEFVAR", "LF@%s\n", ret_var);

    while (!stack_empty(postfix_stack) && operations_count >= 1) {
        while_opened = 1;
        ifj18_obj_t *tmp_var_obj = init_var();
        sprintf(tmp_var_obj->obj_type.var.var_name, "%s_%d", TEMP_EXPRESSION_VARNAME, operations_count);

        ifj18_token_t *act_token = stack_top(postfix_stack);
        stack_pop(postfix_stack);

        debug_info("PIZDOZO\n");
        token_prettyprint(act_token);

        if (act_token->type == TOKEN_ID || act_token->type == TOKEN_INT || act_token->type == TOKEN_FLOAT) {
            set_object_value(act_token, act_function, output_stack);
        } else {
            ifj18_obj_t *operand_1 = stack_top(output_stack);
            stack_pop(output_stack);
            ifj18_obj_t *operand_2 = stack_top(output_stack);
            stack_pop(output_stack);

            stack_print(postfix_stack);
            stack_print_objects(output_stack);

            char *prefix_1 = get_bytecode_objtype(operand_1);
            char *prefix_2 = get_bytecode_objtype(operand_2);

            char *operation;

            operation = (char *) ifj18_token_type_string(act_token->type);

            generate_3ac_expressions(prefix_1, operand_1, prefix_2, operand_2, tmp_var_obj,
                                     operand_1->obj_type.var.type,
                                     operand_2->obj_type.var.type, operation, operations_count);

            operations_count--;

            debug_info("Operations_count = %d\n", operations_count);

            debug_info("Stack push: %s\n", tmp_var_obj->obj_type.var.var_name);

            type = tmp_var_obj->obj_type.var.type;
            stack_push(output_stack, tmp_var_obj);
        }
    }

    if (!while_opened && operations_count == 0) {
        ifj18_token_t *act_token = stack_top(postfix_stack);
        stack_pop(postfix_stack);
        switch (act_token->type) {
            case TOKEN_STRING:
                print_instruction("MOVE", "LF@%s_%d string@%s\n", TEMP_EXPRESSION_VARNAME, operations_count + 1,
                                  act_token->value->as_string->value);
                type = IFJ18_TYPE_STRING;
                break;
            case TOKEN_ID:
                print_instruction("MOVE", "LF@%s_%d LF@%s\n", TEMP_EXPRESSION_VARNAME, operations_count + 1,
                                  act_token->value->as_string->value);
                type = ifj18_hash_get(act_function->obj_type.func.local_symtable, act_token->value->as_string->value)->obj_type.var.type;
                break;
            case TOKEN_INT:
                print_instruction("MOVE", "LF@%s_%d int@%d\n", TEMP_EXPRESSION_VARNAME, operations_count + 1,
                                  act_token->value->as_int);
                type = IFJ18_TYPE_INT;
                break;
            case TOKEN_FLOAT:
                print_instruction("MOVE", "LF@%s_%d float@%f\n", TEMP_EXPRESSION_VARNAME, operations_count + 1,
                                  act_token->value->as_float);
                type = IFJ18_TYPE_FLOAT;
                break;
            default:
                error(SYNTAX_ERROR, "Unexpected token");
        }
    }

    //    stack_pop(output_stack);

    print_instruction_no_args("POPFRAME");
    print_instruction("MOVE", "LF@%s TF@%s_1\n", ret_var, TEMP_EXPRESSION_VARNAME, operations_count + 1);
    return type;
}

int inf_to_post(ifj18_obj_t *act_function, char *ret_var) {
    ifj18_token_t *stack_token;

    unsigned count_of_bracket = 0;
    int sum_count = 1;
    int operatiobs_ocount = 0;

    ifj18_stack_t *output_stack = stack_init();
    ifj18_stack_t *infix_stack = stack_init();

    int prev_token = 99; // default
    /// marks, which means end of the expression
    while (token->type != TOKEN_END_OF_LINE && token->type != TOKEN_THEN && token->type != TOKEN_COMMA &&
           token->type != TOKEN_END_OF_FILE) {
        /// operand adds to the output stack with postfix expression
        if (token->type == TOKEN_ID || token->type == TOKEN_FLOAT || token->type == TOKEN_INT) {
            sum_count--;
            // printf("#%d\n", token->value->as_int);
            stack_token = token;
            // printf("#%d\n", stack_token->value->as_int);
            stack_push(output_stack, stack_token);
        }
            /// left bracket adds to the temporary stack
        else if (token->type == TOKEN_LPAREN) {
            count_of_bracket++;
            stack_token = token;
            stack_push(infix_stack, stack_token);
        }
            /// right bracket means reducing expression on the temporary stack until to left bracket
        else if (token->type == TOKEN_RPAREN) {
            count_of_bracket--;
            do_until_left_bracket(infix_stack, output_stack);
        }
            /// at the operator decides multiple factors in other function, which is calling
        else if (token->type == TOKEN_OP_PLUS || token->type == TOKEN_OP_MINUS || token->type == TOKEN_OP_MUL ||
                 token->type == TOKEN_OP_DIV ||
                 token->type == TOKEN_OP_LT || token->type == TOKEN_OP_GT || token->type == TOKEN_OP_LTE ||
                 token->type == TOKEN_OP_GTE ||
                 token->type == TOKEN_OP_ASSIGN || token->type == TOKEN_OP_NEQ) {

            sum_count++;
            operatiobs_ocount++;
            stack_token = stack_top(infix_stack);
            psa_operation(infix_stack, output_stack, stack_token);
        }
        debug_info("TOKEN PRETTY SPRINT\n");
                token_prettyprint(token);
        get_token();

    }

    /// relocating eventually hinges on the temporary stack
    while (!stack_empty(infix_stack)) {
        stack_push(output_stack, stack_top(infix_stack));
        stack_pop(infix_stack);
    }
    /// ensure the correct order
    stack_copy(infix_stack, output_stack);

    /// incorrect count of brackets or operands and operators
    if (count_of_bracket || sum_count)
        error(SYNTAX_ERROR, "error while parsing expressions");

    debug_info("sumcum: %d\n", operatiobs_ocount);
    /// generating instruction
    return post_to_instr(infix_stack, act_function, ret_var, operatiobs_ocount);


}

ifj18_obj_t *find_var(ifj18_token_t *find_token, ifj18_obj_t *act_function) {
    token_prettyprint(find_token);
    /// find variable in local symtable of actual function
    if (find_token->type == TOKEN_ID) {
        ifj18_obj_t *found = ifj18_hash_get((kh_value_t *) act_function->obj_type.func.local_symtable,
                                            find_token->value->as_string->value);
        /// we did not find
        if (!ifj18_hash_has((kh_value_t *) act_function->obj_type.func.local_symtable, find_token->value->as_string->value)) {
            error(SEMANTIC_ERROR, "\n");
        }

        return found;
    }
}

void length() {
    printf("LABEL $_length\n"
           "PUSHFRAME\n"
           "STRLEN GF@$_stack_temp LF@$_arg_0\n"
           "PUSHS GF@$_stack_temp\n"
           "POPFRAME\n"
           "RETURN\n\n");
}

// void substr() {
//   printf("LABEL $_substr\n"
//          "PUSHFRAME\n"a
//          "STRLEN GF@$_str_temp_2 LF@$_arg_0\n"
//          "JUMPIFEQ $_assert_zero GF@$_str_temp_2 int@0\n"
//          "\n"
//          "LT GF@$_str_temp_1 LF@$_arg_1 int@1\n"
//          "JUMPIFEQ $_assert_zero GF@$_str_temp_1 bool@true\n"
//          "\n"
//          "LT GF@$_str_temp_1 LF@$_arg_2 int@0\n"
//          "JUMPIFEQ $_truncate_N GF@$_str_temp_1 bool@true\n"
//          "\n"
//          "SUB GF@$_str_temp_1 GF@$_str_temp_2 LF@$_arg_1\n"
//          "GT GF@$_str_temp_1 LF@$_arg_2 GF@$_str_temp_1\n"
//          "JUMPIFEQ $_truncate_N GF@$_str_temp_1 bool@true\n"
//          "JUMP $_cycle_start\n"
//          "LABEL $_truncate_N\n"
//          "SUB GF@$_str_temp_1 GF@$_str_temp_2 LF@$_arg_1\n"
//          "ADD GF@$_str_temp_1 GF@$_str_temp_1 int@1\n"
//          "MOVE LF@$_arg_2 GF@$_str_temp_1\n"
//          "\n"
//          "LABEL $_cycle_start\n"
//          "SUB LF@$_arg_1 LF@$_arg_1 int@1\n"
//          "MOVE GF@$_stack_temp string@\n"
//          "LABEL $_compare_next\n"
//          "JUMPIFEQ $_all_done LF@$_arg_2 int@0\n"
//          "GETCHAR GF@$_str_temp_1 LF@$_arg_0 LF@$_arg_1\n"
//          "CONCAT GF@$_stack_temp GF@$_stack_temp GF@$_str_temp_1\n"
//          "ADD LF@$_arg_1 LF@$_arg_1 int@1\n"
//          "SUB LF@$_arg_2 LF@$_arg_2 int@1\n"
//          "JUMP $_compare_next\n"
//          "\n"
//          "LABEL $_assert_zero\n"
//          "MOVE GF@$_stack_temp string@\n"
//          "\n"
//          "LABEL $_all_done\n"
//          "PUSHS GF@$_stack_temp\n"
//          "POPFRAME\n"
//          "RETURN\n\n");
// }

void chr() {
    printf("LABEL $_chr\n"
           "PUSHFRAME\n"
           "INT2CHAR GF@$_stack_temp LF@$_arg_0\n"
           "PUSHS GF@$_stack_temp\n"
           "POPFRAME\n"
           "RETURN\n\n");
}

void ord() {
    printf("LABEL $_ord\n"
           "PUSHFRAME\n"
           "STRLEN GF@$_stack_temp LF@$_arg_0\n"
           "JUMPIFEQ $_assert_zero_end GF@$_stack_temp int@0\n"
           "\n"
           "SUB GF@$_stack_temp GF@$_stack_temp int@1\n"
           "LT GF@$_str_temp_1 LF@$_arg_1 int@0\n"
           "JUMPIFEQ $_assert_zero_end GF@$_str_temp_1 bool@true\n"
           "\n"
           "GT GF@$_str_temp_1 LF@$_arg_1 GF@$_stack_temp\n"
           "JUMPIFEQ $_assert_zero_end GF@$_str_temp_1 bool@true\n"
           "\n"
           "STRI2INT GF@$_stack_temp LF@$_arg_0 LF@$_arg_1\n"
           "JUMP $_ord_end\n"
           "\n"
           "LABEL $_assert_zero_end\n"
           "MOVE GF@$_stack_temp nil@nil\n"
           "LABEL $_ord_end\n"
           "PUSHS GF@$_stack_temp\n"
           "POPFRAME\n"
           "RETURN\n\n");
}
