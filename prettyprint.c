///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Function for printing token 	                                   //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
//              Alexandr Demicev    (xdemic00)                                   //
///////////////////////////////////////////////////////////////////////////////////

#include "prettyprint.h"

void token_prettyprint(ifj18_token_t *token) {
  debug_info("%sTOKEN:", KBLU);
  switch (token->type) {
  case TOKEN_INT:
    debug_info_unwrapped(" %d", token->value->as_int);
    break;
  case TOKEN_FLOAT:
    debug_info_unwrapped(" %f", token->value->as_float);
    break;
  case TOKEN_STRING:
    debug_info_unwrapped(" %s", token->value->as_string->value);
    break;
  case TOKEN_ID:
    debug_info_unwrapped(" %s", token->value->as_string->value);
    break;
  default:
    debug_info_unwrapped(" %s", ifj18_token_strings[token->type]);
    break;
  }
  debug_info_unwrapped("%s", RESET);
  debug_info_unwrapped("\n");
}
