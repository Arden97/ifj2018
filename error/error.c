///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Error states	                                                   //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
///////////////////////////////////////////////////////////////////////////////////

#include "error.h"

char  *error_msg(int error_code){
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

  return err_msg;
}

void error(int error_code, const char *format, ...){
    fprintf(stderr,RESET);
    printf(RESET);
    fprintf(stderr, "%s%s. ", KRED, error_msg(error_code));

    va_list ap;
    va_start (ap, format);
    vfprintf(stderr, format, ap);
    va_end (ap);

    fprintf(stderr, "%s\n", RESET);
    // gc_dispose();
    exit(error_code);
}
