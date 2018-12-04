#include "error.h"
#include "gc.h"
#include "utils.h"

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
      err_msg = "";
      break;

    case ARGS_ERROR:
      err_msg = "ArgumentsError";
      break;

    case SEMANTIC_ERROR:
      err_msg = "SemanticError";
      break;

    case DIVBYZERO_ERROR:
      err_msg = "DivisionByZeroError";
      break;

    case INTERNAL_ERROR:
      err_msg = "InternalError";
      break;
    default:
      err_msg = "UnknownError";
      break;
  }
  fprintf(stderr,RESET);
  printf(RESET);
  fprintf(stderr, "%s%s. Message:  %s\n%s", KRED,err_msg, details,RESET);
}

void error(int error_code, char *details){
  error_msg(error_code, details);
//  gc_dispose();
  exit(error_code);
}
