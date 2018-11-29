#include "symtable.h"
#include "semantics.h"
#include "scanner.h"
#include "strlib.h"
#include "error.h"


int inputf();
int inputi();
int inputs();
int length();
int substr();
int ord();
void check_token_type(int required_type, int error_type, int inv);
void check_arg(int required_type, char id_allowed);
 