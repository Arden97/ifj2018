///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Header file of lexical analysis 	                               //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
///////////////////////////////////////////////////////////////////////////////////

#ifndef IFJ18_TOKEN_H
#define IFJ18_TOKEN_H

#include "../error/error.h"
#include "../utils/strlib.h"
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IFJ18_TOKEN_LIST                                                                                                                   \
  t(DEF, "def") t(DO, "do") t(END, "end") t(ELSE, "else") t(ID, "id") t(IF, "if") t(INPUTS, "inputs") t(INPUTI, "inputi")                  \
      t(INPUTF, "inputf") t(PRINT, "print") t(WHILE, "while") t(THEN, "then") t(OP_PLUS, "ADD") t(OP_MINUS, "SUB") t(OP_MUL, "MUL")        \
          t(OP_DIV, "DIV") t(OP_LT, "LT") t(OP_GT, "GT") t(OP_LTE, "<=") t(OP_GTE, ">=") t(OP_ASSIGN, "=") t(OP_NEQ, "!=") t(INT, "int")   \
              t(FLOAT, "float") t(STRING, "string") t(END_OF_FILE, "EOF") t(END_OF_LINE, "EOL") t(LENGTH, "length") t(LPAREN, "(")         \
                  t(RPAREN, ")") t(OP_NOT, "!") t(COMMA, ",") t(OP_DOT, ".") t(SUBSTR, "substr") t(OP_EQ, "EQ") t(ORD, "ord")              \
                      t(OP_AND, "&&") t(OP_OR, "||") t(EOS, "end-of-source") t(ILLEGAL, "illegal") t(CHR, "chr") t(NIL, "nil")

/// Tokens enum
typedef enum {
#define t(tok, str) TOKEN_##tok,
  IFJ18_TOKEN_LIST
#undef t
} ifj18_token;

/// Token strings
static char *ifj18_token_strings[] = {
#define t(tok, str) str,
    IFJ18_TOKEN_LIST
#undef t
};

/// Token value
typedef struct {
  string *as_string;
  float as_float;
  int as_int;
} t_token_value;

/// Token structure
typedef struct {
  int len;
  ifj18_token type;
  t_token_value *value;
} ifj18_token_t;

/// Global active token
extern ifj18_token_t *token;

/// Token initialization
void init_token();

/// Return the string associated with the given token `type`.
const char *ifj18_token_type_string(ifj18_token type);

/// Save token to token structure
ifj18_token_t *save_token(ifj18_token type);

/// Convert hex digit `c` to a base 10 int, returning -1 on failure.
static int hex(int c);

/// Scan identifier
static ifj18_token_t *scan_ident(int c);

/// Scan string hex literal, returning -1 on invalid digits.
static int hex_literal();

/// Scan string
static ifj18_token_t *scan_string();

/// Scan number
static ifj18_token_t *scan_number(int c);

/// Reading symbols from stdin and save an apropriate token
ifj18_token_t *get_token();

/// Determines if token have a correct type, calling an error otherwise
void check_token_type(int required_type, int error_type, int inv);
void check_token_type_msg(int required_type, int error_type, int inv, char *message);

/// Global macros for parsing expressions later
#define FUNC_RETURN_VARNAME "$$_retval"
#define FUNC_JUMP_AFTER_TEMPLATE "$$__%s_after__$$"
#define FUNCTION_LABEL_TEMPLATE "$__%s"
#define TEMP_EXPRESSION_VARNAME "$$__tmp_val"
#define TEMP_EXP_CONVERTION_VARNAME "$$__tmp_val_convert"
#define COND_EXPR_RESULT_VARNAME "$$__expr_res"

/*
 * INTERNAL FUNCTION $__name
 * INTERNAL VARIABLE $$__name
 * INTERNAL LABEL $$__name__$$
 */

#endif
