#include <stdio.h>
#include <stdlib.h>

#include "gc.h"
#include "error.h"

void *find(void *ptr) {

    tListElementPtr tmp = adata->Last;

    while (tmp != NULL) {
        if (tmp->val == ptr) {
            return tmp;
        }
        tmp = tmp->lptr;
    }
    return NULL;
}

void *gc_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        gc_dispose();      //free all

        error(INTERNAL_ERROR, "Memory allocation failed");
    }

    gc_append(ptr);    //if failed, exits in gc_append func
    return ptr;
}

void *gc_realloc(void *ptr, size_t size) {
    void *tmp = ptr;
    //realloc feature size = 0, is for now not implemented

    ptr = realloc(ptr, size);
    if (ptr == NULL) {
        //deletes origin pointer from log,
        // but it won't be freed, cause, it is maybe already freed by realloc
        gc_delete(find(tmp));
        gc_dispose();      //free all
        error(INTERNAL_ERROR, "Memory allocation failed");

    }
    if (ptr != tmp) {
        if (tmp != NULL) { gc_delete(find(tmp)); }  //remove and free from log
        gc_append(ptr);    //if failed, exits in gc_append func
    }

    return ptr;
}

void gc_free(void *ptr) {

    free(ptr);                //free usefull val
    gc_delete(find(ptr));      //remove and free from log

}

void gc_dispose() {
    while (copy_last(adata) != NULL) {
        free(copy_last(adata)); //free usefull val
        gc_pop();     //free structure from G

    }
}

void gc_delete(void *element) {
    tListElementPtr tmp = adata->Last;

    while (tmp != NULL) {
        if (tmp == element) {            //delete this one
            if (tmp->rptr != NULL && tmp->lptr != NULL) {
                tmp->rptr->lptr = tmp->lptr;
                tmp->lptr->rptr = tmp->rptr;
            } else if (tmp->rptr != NULL) {
                tmp->rptr->lptr = tmp->lptr;
                adata->First = tmp->rptr;

            } else if (tmp->lptr != NULL) {
                tmp->lptr->rptr = tmp->rptr;
                adata->Last = tmp->lptr;
            }
            free(tmp);
            break;

        } else {
            tmp = tmp->lptr;
        }
    }

}

void gc_append(void *val) {
    tListElementPtr result = malloc(sizeof(struct tListElement));

    if (result == NULL) {
        gc_dispose();      //free all
        error(INTERNAL_ERROR, "Memory allocation failed");

    } else {

        result->val = val;

        if (adata->First == NULL) {   //Empty list
            adata->First = result;
            adata->Last = result;
            result->rptr = NULL;
            result->lptr = NULL;
        } else {                  //One or more elements in list
            result->lptr = adata->Last;
            adata->Last->rptr = result;
            result->rptr = NULL;
            adata->Last = result;
        }
    }
}

void gc_pop() {
    tListElementPtr tmp;

    if (adata->First == NULL) {
        return;
    }

    if (adata->Last == adata->Active) {
        adata->Active = NULL;
    }

    if (adata->First == adata->Last) {
        free(adata->First);
        adata->First = NULL;
        adata->Last = NULL;
    } else {
        tmp = adata->Last->lptr;
        free(adata->Last);
        adata->Last = tmp;
        adata->Last->rptr = NULL;
    }
}
