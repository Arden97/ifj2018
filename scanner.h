#include "strlib.h"

//States of finite automaton
typedef enum{

	S_START,
	S_IDENTIFIER,
	S_NUMBER,
	S_STRING_NUMBERS,
	S_DIV_COM,
	S_NOT_EQ,
	S_GR_EQ,
	S_LESS_EQ,
	S_STR

}states_t;

typedef enum {
  DEF,
  DO,
  END,
  ELSE,
  ID,
  IF,
  INPUTS,
  INPUTI,
  INPUTF,
  PRINT,
  WHILE,
  THEN,
  ADD,
  SUB,
  MUL,
  DIV,
  LESS,
  GREATER,
  LESS_EQ,
  GREATER_EQ,
  ASSIGNMENT_EQ,
  NEQ,
  INT_NUMBER,
  DOUBLE_NUMBER,
  TEXT,
  BACKSLASH,
  END_OF_FILE,
  COMMA,
  SEMICOLON,
  EOL,
  LENGTH,
  SUBSTR,
  ORD,
  CHR
} tokens_t;

//structure of token
typedef struct token_t{
	int type;		//identification of token
	string_t * str;	//attribute of token
}token_t;
