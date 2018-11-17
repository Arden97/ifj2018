#ifndef IFJ18_TOKEN_H
#define IFJ18_TOKEN_H

#include <assert.h>
#include "strlib.h"

#define IFJ18_TOKEN_LIST \
  t(DEF, "def") \
	t(DO, "do") \
	t(END, "end") \
	t(ELSE, "else") \
	t(ID, "id") \
	t(IF, "if") \
	t(INPUTS, "inputs") \
	t(INPUTI, "inputi") \
	t(INPUTF, "inputf") \
	t(PRINT, "print") \
	t(WHILE, "while") \
	t(THEN, "then") \
	t(OP_PLUS, "+") \
	t(OP_MINUS, "-") \
	t(OP_MUL, "*") \
	t(OP_DIV, "/") \
	t(OP_LT, "<") \
	t(OP_GT, ">") \
	t(OP_LTE, "<=") \
	t(OP_GTE, ">=") \
	t(OP_ASSIGN, "=") \
	t(OP_NEQ, "!=") \
	t(INT, "int") \
	t(FLOAT, "float") \
	t(STRING, "string") \
	t(END_OF_FILE, "eof") \
	t(END_OF_LINE, "eol") \
	t(LENGTH, "length") \
  t(LPAREN, "(") \
  t(RPAREN, ")") \
  t(OP_NOT, "!") \
  t(COMMA, ",") \
  t(OP_DOT, ".") \
	t(SUBSTR, "substr") \
  t(OP_EQ, "==") \
	t(ORD, "ord") \
  t(OP_AND, "&&") \
  t(OP_OR, "||") \
  t(EOS, "end-of-source") \
  t(ILLEGAL, "illegal") \
	t(CHR, "chr")

/*
 * Tokens enum.
 */

typedef enum {
#define t(tok, str) TOKEN_##tok,
IFJ18_TOKEN_LIST
#undef t
} ifj18_token;

/*
 * Token strings.
 */

static char *ifj18_token_strings[] = {
#define t(tok, str) str,
IFJ18_TOKEN_LIST
#undef t
};

typedef struct {
  string *as_string;
  float as_float;
  int as_int;
} t_token_value;

typedef struct {
  int len;
  ifj18_token type;
  t_token_value *value;
} ifj18_token_t;

/*
 * Return the string associated with the
 * given token `type`.
 */

static inline const char *ifj18_token_type_string(ifj18_token type) {
  assert(type <= TOKEN_CHR);
  return ifj18_token_strings[type];
}

ifj18_token_t *getToken();
#endif /* IFJ18_TOKEN_H */
