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
