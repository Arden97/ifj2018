#include "list.h"
#include <stdlib.h>

void *copy_last(tList *L){

    if(L->Last != NULL){
        return L->Last->val;
    }
    else {
        return NULL;
    }
}

void init_list(tList *L) {
    L->First = NULL;
    L->Last = NULL;
    L->Active = NULL;
}

void print_list(tList *L){

}
