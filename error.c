#include "error.h"

void error_msg(int error_code, char *details){
  char *err_msg;
  switch(error_code){
    case LEXICAL_ERROR:
      err_msg = "Lexical error";
      break;

    case SYNTAX_ERROR:
      err_msg = "Syntax error";
      break;

    case DEFINITION_ERROR:
      err_msg = "Undefined/redefined function/variable";
      break;

    case TYPE_ERROR:
      err_msg = "Type compatibility error";
      break;

    case ARGS_ERROR:
      err_msg = "Wrong number of arguments";
      break;

    case SEMANTIC_ERROR:
      err_msg = "Semantic error";
      break;

    case DIVBYZERO_ERROR:
      err_msg = "Attempting to divide by zero";
      break;

    case INTER_ERROR:
      err_msg = "Internal error of compiler";
      break;
  }
  fprintf(stderr, "%s:  %s", err_msg, details);
  /// clear_all() clear all allocated memory
  /// free(ptr) clear memory for pointer of dll
}
