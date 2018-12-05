#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "gc.h"

void *find(void *ptr) {

  tListElementPtr tmp = garbage_list->Last;

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
    gc_dispose(); // free all

    error(INTERNAL_ERROR, "Memory allocation failed");
  }

  gc_append(ptr); // if failed, exits in gc_append func
  return ptr;
}

void *gc_realloc(void *ptr, size_t size) {
  void *tmp = ptr;
  // realloc feature size = 0, is for now not implemented

  ptr = realloc(ptr, size);
  if (ptr == NULL) {
    // deletes origin pointer from log,
    // but it won't be freed, cause, it is maybe already freed by realloc
    gc_delete(find(tmp));
    gc_dispose(); // free all
    error(INTERNAL_ERROR, "Memory allocation failed");
  }
  if (ptr != tmp) {
    if (tmp != NULL) {
      gc_delete(find(tmp));
    }               // remove and free from log
    gc_append(ptr); // if failed, exits in gc_append func
  }

  return ptr;
}

void gc_free(void *ptr) {

  free(ptr);            // free usefull val
  gc_delete(find(ptr)); // remove and free from log
}

void gc_dispose() {
  while (copy_last(garbage_list) != NULL) {
    free(copy_last(garbage_list)); // free usefull val
    gc_pop();                      // free structure from G
  }
}

void gc_delete(void *element) {
  tListElementPtr tmp = garbage_list->Last;

  while (tmp != NULL) {
    if (tmp == element) { // delete this one
      if (tmp->rptr != NULL && tmp->lptr != NULL) {
        tmp->rptr->lptr = tmp->lptr;
        tmp->lptr->rptr = tmp->rptr;
      } else if (tmp->rptr != NULL) {
        tmp->rptr->lptr = tmp->lptr;
        garbage_list->First = tmp->rptr;

      } else if (tmp->lptr != NULL) {
        tmp->lptr->rptr = tmp->rptr;
        garbage_list->Last = tmp->lptr;
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
    gc_dispose(); // free all
    error(INTERNAL_ERROR, "Memory allocation failed");

  } else {

    result->val = val;

    if (garbage_list->First == NULL) { // Empty list
      garbage_list->First = result;
      garbage_list->Last = result;
      result->rptr = NULL;
      result->lptr = NULL;
    } else { // One or more elements in list
      result->lptr = garbage_list->Last;
      garbage_list->Last->rptr = result;
      result->rptr = NULL;
      garbage_list->Last = result;
    }
  }
}

void gc_pop() {
  tListElementPtr tmp;

  if (garbage_list->First == NULL) {
    return;
  }

  if (garbage_list->Last == garbage_list->Active) {
    garbage_list->Active = NULL;
  }

  if (garbage_list->First == garbage_list->Last) {
    free(garbage_list->First);
    garbage_list->First = NULL;
    garbage_list->Last = NULL;
  } else {
    tmp = garbage_list->Last->lptr;
    free(garbage_list->Last);
    garbage_list->Last = tmp;
    garbage_list->Last->rptr = NULL;
  }
}
