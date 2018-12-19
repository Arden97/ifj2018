///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Header file of garbage collector 	                               //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
//              Alexandr Demicev    (xdemic00)                                   //
///////////////////////////////////////////////////////////////////////////////////

#ifndef ifj_GC

#define ifj_GC

#include "list.h"
#include <stddef.h>

tList *garbage_list;

void *find(void *ptr);
void *gc_malloc(size_t size);
void *gc_realloc(void *ptr, size_t size);
void gc_free(void *ptr);
void gc_dispose();

void gc_delete(void *element);
void gc_append(void *val);
void gc_pop();

#endif
