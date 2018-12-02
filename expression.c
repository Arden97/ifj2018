#include "expression.h"
// #define apply_on_new_token(stat) get_token();stat
char precedence_table[16][16] = {
    /*            +    -    *    /    (    )    \    <    >   <=   >=    =   <>   id   lit   $ */
    /*  +  */ {'>', '>', '<', '<', '<', '>', '<', '>', '>', '>', '>', '>', '>', '<', '<', '>'},
    /*  -  */ {'>', '>', '<', '<', '<', '>', '<', '>', '>', '>', '>', '>', '>', '<', '<', '>'},
    /*  *  */ {'>', '>', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>'},
    /*  /  */ {'>', '>', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>'},
    /*  (  */ {'<', '<', '<', '<', '<', '=', '<', '<', '<', '<', '<', '<', '<', '<', '<', ' '},
    /*  )  */ {'>', '>', '>', '>', ' ', '>', '>', '>', '>', '>', '>', '>', '>', ' ', ' ', '>'},
    /*  \  */ {'>', '>', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>'},
    /*  <  */ {'<', '<', '<', '<', '<', '>', '<', ' ', ' ', ' ', ' ', ' ', ' ', '<', '<', '>'},
    /*  >  */ {'<', '<', '<', '<', '<', '>', '<', ' ', ' ', ' ', ' ', ' ', ' ', '<', '<', '>'},
    /*  <= */ {'<', '<', '<', '<', '<', '>', '<', ' ', ' ', ' ', ' ', ' ', ' ', '<', '<', '>'},
    /*  >= */ {'<', '<', '<', '<', '<', '>', '<', ' ', ' ', ' ', ' ', ' ', ' ', '<', '<', '>'},
    /*  =  */ {'<', '<', '<', '<', '<', '>', '<', ' ', ' ', ' ', ' ', ' ', ' ', '<', '<', '>'},
    /*  <> */ {'<', '<', '<', '<', '<', '>', '<', ' ', ' ', ' ', ' ', ' ', ' ', '<', '<', '>'},
    /*  ID */ {'>', '>', '>', '>', ' ', '>', '>', '>', '>', '>', '>', '>', '>', ' ', ' ', '>'},
    /* LIT */ {'>', '>', '>', '>', ' ', '>', '>', '>', '>', '>', '>', '>', '>', ' ', ' ', '>'},
    /*  $  */ {'<', '<', '<', '<', '<', ' ', '<', '<', '<', '<', '<', '<', '<', '<', '<', ' '},
};
char flags[4] = {0, 0, 0, 0};
#define FG_LENGTH 0
#define FG_INPUTI 1
#define FG_INPUTF 2
#define FG_INPUTS 3

int expression(ifj18_obj_t *func) {
  if (is_function())
    return function(func);
  else
  return inf_to_post(func);
}

int is_function() {
  ifj18_obj_t *symbol = ifj18_hash_get(global_table, token->value->as_string);
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

int function(ifj18_obj_t *act_function) {
  switch (token->type) {
  case TOKEN_INPUTF:
    // return inputf();
    break;

  case TOKEN_INPUTI:
    // return inputi();
    break;

  case TOKEN_INPUTS:
    // return inputs();
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
    printf("PUSHS %s\n", func->obj_type.func.sparams[i].value);
    get_token();
    if ((i != params_num - 1) && (token->type != TOKEN_COMMA)) {
      return 0;
    }
  }
  get_token();
  if (token->type != TOKEN_RPAREN) {
    return 0;
  }
  printf("CALL %s\n", key);
}

int length() {
  get_token();

  check_token_type(TOKEN_LPAREN, SYNTAX_ERROR, 1);

  get_token();

  check_arg(TOKEN_STRING, 1);

  printf("PUSHS %sCREATEFRAME\nUSHFRAME, token->value->as_string");
}


void print_length() {
  if (!flags[FG_LENGTH] && flags[FG_LENGTH]++) {
    printf("LABEL $_length\nPUSHFRAME\nSTRLEN GF@$_stack_temp LF@$_arg_0\nPUSHS GF@$_stack_temp\nPOPFRAME\nRETURN\n\n");
  }
  printf("%d", flags[FG_LENGTH]);
}

int shift_to_stack(ifj18_token_t *stack_token) {

  /// calculate index to the precedence table
  unsigned index_y = token->type - TOKEN_OP_PLUS; // coordinates y
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
    ifj18_token_t *stack_token = copy_token(stack_top(operators_stack));
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
    stack_token = copy_token(token);
    stack_push(operators_stack, stack_token);
  } else {
    /// have to marks of reduction in the table and stack dont have to be empty
    while (!stack_empty(operators_stack) && !shift_to_stack(stack_token)) {
      stack_token = copy_token(stack_token);
      stack_push(output_stack, stack_token);
      stack_pop(operators_stack);
      stack_token = stack_top(operators_stack);
    }
    stack_token = copy_token(token);
    stack_push(operators_stack, stack_token);
  }
}

ifj18_token_t *copy_token(ifj18_token_t *act_token) {
  ifj18_token_t *new_token;
  new_token = malloc(sizeof(ifj18_token_t));
  new_token->value = malloc(sizeof(t_token_value));
  new_token->value->as_string = malloc(sizeof(string));
  /// initialization token string about old size of string
  string_init(new_token->value->as_string);
  /// copy type, string and his length to the atributes of new token
  new_token->type = act_token->type;

  if (act_token->value->as_string)
    strcpy(new_token->value->as_string->value, act_token->value->as_string->value);
  new_token->value->as_string->length = act_token->value->as_string->length;
  return new_token;
}

ifj18_var check_operands(ifj18_obj_t *operand_1, ifj18_obj_t *operand_2) {
  if (operand_1->obj_type.var.type == operand_2->obj_type.var.type) {
    return operand_1->obj_type.var.type;
  } else
    return 0;
}

void post_to_instr(ifj18_stack_t *postfix_stack, ifj18_obj_t *act_function) {
  ifj18_stack_t *output_stack = stack_init();
  printf("123\n");
  while (!stack_empty(postfix_stack)) {
    ifj18_token_t *act_token = stack_top(postfix_stack);
    stack_pop(postfix_stack);
    // if token == operand
    if (act_token->type == TOKEN_ID || act_token->type == TOKEN_FLOAT || act_token->type == TOKEN_INT) {
      ifj18_obj_t *operand = ifj18_hash_get(act_function->obj_type.func.local_symtable, act_token->value->as_string);
      if (!operand) {
        error(SEMANTIC_ERROR, "there is no such variable");
      }
      stack_push(output_stack, operand);
      // list_insert("PUSHS ", operand, NULL, NULL);
    } else {
      ifj18_obj_t *operand_1 = stack_top(output_stack); /// obtaining fist operand
      stack_pop(output_stack);
      ifj18_obj_t *operand_2 = stack_top(output_stack); /// obtaining second operand
      stack_pop(output_stack);
      switch (act_token->type) {
      case TOKEN_OP_PLUS:
        // TODO: third variable?
        if (check_operands(operand_1, operand_2) == IFJ18_TYPE_INT) {
          printf("ADD %s %s\n", operand_1->obj_type.var.value.as_int, operand_2->obj_type.var.value.as_int);
        } else if (check_operands(operand_1, operand_2) == IFJ18_TYPE_FLOAT) {
          printf("ADD %s %s\n", operand_1->obj_type.var.value.as_float, operand_2->obj_type.var.value.as_float);
        } else {
          error(TYPE_ERROR, "operands should have same type");
        }
        break;

      case TOKEN_OP_MINUS:
        if (check_operands(operand_1, operand_2) == IFJ18_TYPE_INT) {
          printf("SUB %s %s\n", operand_1->obj_type.var.value.as_int, operand_2->obj_type.var.value);
        } else if (check_operands(operand_1, operand_2) == IFJ18_TYPE_FLOAT) {
          printf("SUB %s %s\n", operand_1->obj_type.var.value.as_float, operand_2->obj_type.var.value.as_float);
        } else {
          error(TYPE_ERROR, "operands should have same type");
        }
        break;

      case TOKEN_OP_MUL:
        if (check_operands(operand_1, operand_2) == IFJ18_TYPE_INT) {
          printf("MUL %s %s\n", operand_1->obj_type.var.value.as_int, operand_2->obj_type.var.value.as_int);
        } else if (check_operands(operand_1, operand_2) == IFJ18_TYPE_FLOAT) {
          printf("MUL %s %s\n", operand_1->obj_type.var.value.as_float, operand_2->obj_type.var.value.as_float);
        } else {
          error(TYPE_ERROR, "operands should have same type");
        }
        break;

      case TOKEN_OP_DIV:
        if (check_operands(operand_1, operand_2) == IFJ18_TYPE_FLOAT) {
          printf("DIV %s %s\n", operand_1->obj_type.var.value.as_float, operand_2->obj_type.var.value.as_float);
        } else {
          error(TYPE_ERROR, "operands should have same type");
        }
        break;

      case TOKEN_OP_LT:
        if (check_operands(operand_1, operand_2) == IFJ18_TYPE_INT) {
          printf("LT %s %s\n", operand_1->obj_type.var.value.as_int, operand_2->obj_type.var.value.as_int);
        } else if (check_operands(operand_1, operand_2) == IFJ18_TYPE_FLOAT) {
          printf("LT %s %s\n", operand_1->obj_type.var.value.as_float, operand_2->obj_type.var.value.as_float);
        } else if (check_operands(operand_1, operand_2) == IFJ18_TYPE_STRING) {
          printf("LT %s %s\n", operand_1->obj_type.var.value.as_pointer, operand_2->obj_type.var.value.as_pointer);
        } else {
          error(TYPE_ERROR, "operands should have same type");
        }
        break;

      case TOKEN_OP_GT:
        if (check_operands(operand_1, operand_2) == IFJ18_TYPE_INT) {
          printf("GT %s %s\n", operand_1->obj_type.var.value.as_int, operand_2->obj_type.var.value.as_int);
        } else if (check_operands(operand_1, operand_2) == IFJ18_TYPE_FLOAT) {
          printf("GT %s %s\n", operand_1->obj_type.var.value.as_float, operand_2->obj_type.var.value.as_float);
        } else if (check_operands(operand_1, operand_2) == IFJ18_TYPE_STRING) {
          printf("GT %s %s\n", operand_1->obj_type.var.value.as_pointer, operand_2->obj_type.var.value.as_pointer);
        } else {
          error(TYPE_ERROR, "operands should have same type");
        }
        break;

      case TOKEN_OP_LTE:
        if (check_operands(operand_1, operand_2) == IFJ18_TYPE_INT) {
          printf("GTS %s %s\n", operand_1->obj_type.var.value.as_int, operand_2->obj_type.var.value.as_int);
          printf("NOTS %s %s\n", operand_1->obj_type.var.value.as_int, operand_2->obj_type.var.value.as_int);
        } else if (check_operands(operand_1, operand_2) == IFJ18_TYPE_FLOAT) {
          printf("GTS %s %s\n", operand_1->obj_type.var.value.as_float, operand_2->obj_type.var.value.as_float);
          printf("NOTS %s %s\n", operand_1->obj_type.var.value.as_float, operand_2->obj_type.var.value.as_float);
        } else if (check_operands(operand_1, operand_2) == IFJ18_TYPE_STRING) {
          printf("GTS %s %s\n", operand_1->obj_type.var.value.as_pointer, operand_2->obj_type.var.value.as_pointer);
          printf("NOTS %s %s\n", operand_1->obj_type.var.value.as_pointer, operand_2->obj_type.var.value.as_pointer);
        } else {
          error(TYPE_ERROR, "operands should have same type");
        }
        break;

      case TOKEN_OP_GTE:
        if (check_operands(operand_1, operand_2) == IFJ18_TYPE_INT) {
          printf("LTS %s %s\n", operand_1->obj_type.var.value.as_int, operand_2->obj_type.var.value.as_int);
          printf("NOTS %s %s\n", operand_1->obj_type.var.value.as_int, operand_2->obj_type.var.value.as_int);
        } else if (check_operands(operand_1, operand_2) == IFJ18_TYPE_FLOAT) {
          printf("LTS %s %s\n", operand_1->obj_type.var.value.as_float, operand_2->obj_type.var.value.as_float);
          printf("NOTS %s %s\n", operand_1->obj_type.var.value.as_float, operand_2->obj_type.var.value.as_float);
        } else if (check_operands(operand_1, operand_2) == IFJ18_TYPE_STRING) {
          printf("LTS %s %s\n", operand_1->obj_type.var.value.as_pointer, operand_2->obj_type.var.value.as_pointer);
          printf("NOTS %s %s\n", operand_1->obj_type.var.value.as_pointer, operand_2->obj_type.var.value.as_pointer);
        } else {
          error(TYPE_ERROR, "operands should have same type");
        }
        break;

      case TOKEN_OP_ASSIGN:
        if (check_operands(operand_1, operand_2) == IFJ18_TYPE_INT) {
          printf("EQ %s %s\n", operand_1->obj_type.var.value.as_int, operand_2->obj_type.var.value.as_int);
        } else if (check_operands(operand_1, operand_2) == IFJ18_TYPE_FLOAT) {
          printf("EQ %s %s\n", operand_1->obj_type.var.value.as_float, operand_2->obj_type.var.value.as_float);
        } else if (check_operands(operand_1, operand_2) == IFJ18_TYPE_STRING) {
          printf("EQ %s %s\n", operand_1->obj_type.var.value.as_pointer, operand_2->obj_type.var.value.as_pointer);
        } else {
          error(TYPE_ERROR, "operands should have same type");
        }
        break;

      case TOKEN_OP_NEQ:
        if (check_operands(operand_1, operand_2) == IFJ18_TYPE_INT) {
          printf("EQS %s %s\n", operand_1->obj_type.var.value.as_int, operand_2->obj_type.var.value.as_int);
          printf("NOTS %s %s\n", operand_1->obj_type.var.value.as_int, operand_2->obj_type.var.value.as_int);
        } else if (check_operands(operand_1, operand_2) == IFJ18_TYPE_FLOAT) {
          printf("EQS %s %s\n", operand_1->obj_type.var.value.as_float, operand_2->obj_type.var.value.as_float);
          printf("NOTS %s %s\n", operand_1->obj_type.var.value.as_float, operand_2->obj_type.var.value.as_float);
        } else if (check_operands(operand_1, operand_2) == IFJ18_TYPE_STRING) {
          printf("EQS %s %s\n", operand_1->obj_type.var.value.as_pointer, operand_2->obj_type.var.value.as_pointer);
          printf("NOTS %s %s\n", operand_1->obj_type.var.value.as_pointer, operand_2->obj_type.var.value.as_pointer);
        } else {
          error(TYPE_ERROR, "operands should have same type");
        }
        break;
      }
    }
  }
}

int inf_to_post(ifj18_obj_t *act_function) {
  ifj18_token_t *stack_token;

  unsigned count_of_bracket = 0;
  int sum_count = 1;

  ifj18_stack_t *output_stack = stack_init();
  ifj18_stack_t *infix_stack = stack_init();

  get_token();
  int prev_token = 99; // default
  /// marks, which means end of the expression
  while (token->type != TOKEN_END_OF_LINE && token->type != TOKEN_THEN && token->type != TOKEN_COMMA && token->type != TOKEN_END_OF_FILE) {
    /// operand adds to the output stack with postfix expression
    if (token->type == TOKEN_ID || token->type == TOKEN_FLOAT || token->type == TOKEN_INT) {      
      sum_count--;
      stack_token = copy_token(token);
      stack_push(output_stack, stack_token);
    }
    /// left bracket adds to the temporary stack
    else if (token->type == TOKEN_LPAREN) {
      count_of_bracket++;
      stack_token = copy_token(token);
      stack_push(infix_stack, stack_token);
    }
    /// right bracket means reducing expression on the temporary stack until to left bracket
    else if (token->type == TOKEN_RPAREN) {
      count_of_bracket--;
      do_until_left_bracket(infix_stack, output_stack);
    }
    /// at the operator decides multiple factors in other function, which is calling
    else if (token->type == TOKEN_OP_PLUS || token->type == TOKEN_OP_MINUS || token->type == TOKEN_OP_MUL || token->type == TOKEN_OP_DIV ||
             token->type == TOKEN_OP_LT || token->type == TOKEN_OP_GT || token->type == TOKEN_OP_LTE || token->type == TOKEN_OP_GTE ||
             token->type == TOKEN_OP_ASSIGN || token->type == TOKEN_OP_NEQ) {

      sum_count++;
      stack_token = stack_top(infix_stack);
      psa_operation(infix_stack, output_stack, stack_token);
    }
    get_token();
  }

  /// relocating eventually hinges on the temporary stack
  while (!stack_empty(infix_stack)) {
    stack_push(output_stack, copy_token(stack_top(infix_stack)));
    stack_pop(infix_stack);
  }
  /// ensure the correct order
  stack_copy(infix_stack, output_stack);

  /// incorrect count of brackets or operands and operators
  if (count_of_bracket || sum_count)
    error(SYNTAX_ERROR, "error while parsing expressions");

  /// generating instruction
  post_to_instr(infix_stack, act_function);
}