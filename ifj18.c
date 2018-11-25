#include <stdlib.h>
#include <stdio.h>

#include "gc.h"
#include "semantics.h"
#include "scanner.h"
#include "prettyprint.h"

tList *adata = NULL;


int main() {
    getToken();
    int counter = 0;

    while (token->type != TOKEN_END_OF_FILE && counter++ < 10) {
        printf(KBLU);
        switch (token->type) {
            case TOKEN_ID:
                printf("%d:ID Token found: ", counter);
                break;
            case TOKEN_WHILE:
                printf("%d: While token has found:", counter);
                break;
            case TOKEN_IF:
                printf("%d: IF token has found:", counter);
                break;
            case TOKEN_PRINT:
                printf("%d: PRINT token has found:", counter);
                break;
            case TOKEN_INPUTS:
                printf("%d: INPUTS token has found:", counter);
                break;
            case TOKEN_INPUTI:
                printf("%d: INPUTI token has found:", counter);
                break;
            case TOKEN_INPUTF:
                printf("%d: INPUTF token has found:", counter);
                break;
            default:
                // TODO: Change with error
                error_msg(SEMANTIC_ERROR, "Incorrect token type");
                exit(SEMANTIC_ERROR);
        }
        printf(RESET);
        token_prettyprint(token);
        getToken();
    }
    printf(RESET);


//  ifj18_obj_t *tmp = init_var();
//
//  // tmp->obj_type.func.return_var->value.as_int = 42;
//  // tmp->obj_type.func.local_symtable = ifj18_hash_new();
//
//  ifj18_hash_set(tmp->obj_type.func.local_symtable, "foo", tmp);
//
//  printf("%d\n", tmp->obj_type.func.return_var->value.as_int);
//  printf("%d\n", ifj18_hash_has(tmp->obj_type.func.local_symtable, "ooo"));
}