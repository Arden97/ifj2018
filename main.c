#include <stdio.h>
#include <stdlib.h>
#include "gc.h"


tList *adata = NULL;


int main() {
    if((adata= malloc(sizeof(tList))) == NULL){
        printf("Malloc garbage collector\n");
        return 1;
    }
    init_list(adata);

    int *ptr_one;

    ptr_one = (int *)gc_malloc(sizeof(int));

    if (ptr_one == 0)
    {
        printf("ERROR: Out of memory\n");
        return 1;
    }

    *ptr_one = 25;
    printf("%d\n", *ptr_one);

    gc_dispose();

    printf("%d\n", *ptr_one);


    return 0;
}
