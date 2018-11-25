#include "scanner.h"
#include <stdio.h>

void token_prettyprint(ifj18_token_t *token) {
  printf("%s", ifj18_token_type_string(token->type));
  switch (token->type) {
  case TOKEN_INT:
    printf(" %d", token->value->as_int);
    break;
  case TOKEN_FLOAT:
    printf(" %f", token->value->as_float);
    break;
  case TOKEN_STRING:
    printf(" %s", token->value->as_string->value);
    break;
  case TOKEN_ID:
    printf(" %s", token->value->as_string->value);
    break;
  }
  printf("\n");
}
