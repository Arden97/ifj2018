#include "error.h"

void error_msg(int error_code, char *details){
  switch(error_code){
    case LEXICAL_ERROR:
      fprintf(stderr, "%s: %s\n", "Lexical error", details);
      break;

    case SYNTAX_ERROR:
      fprintf(stderr, "%s: %s\n", "Syntax error", details);
      break;

    case DEFINITION_ERROR:
      fprintf(stderr, "%s: %s\n", "Undefined/redefined function/variable", details);
      break;

    case TYPE_ERROR:
      fprintf(stderr, "%s: %s\n", "Type compatibility error", details);
      break;

    case ARGS_ERROR:
      fprintf(stderr, "%s: %s\n", "Wrong number of arguments", details);
      break;

    case SEMANTIC_ERROR:
      fprintf(stderr, "%s: %s\n", "Semantic error", details);
      break;

    case DIVBYZERO_ERROR:
      fprintf(stderr, "%s: %s\n", "Attempting to divide by zero", details);
      break;

    case INTER_ERROR:
      fprintf(stderr, "%s: %s\n", "Internal error of compiler", details);
      break;
  }
  /// clear_all() clear all allocated memory
  /// free(ptr) clear memory for pointer of dll
}
