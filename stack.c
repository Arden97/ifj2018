#include "stack.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>

ifj18_stack_t *stack_init() {
  ifj18_stack_t *s;
  if (!(s = (stack_t *)malloc(sizeof(stack_t))))
    error(INTERNAL_ERROR, "stack memory allocation");
  s->top_ptr = NULL;
  return s;
}

void stack_push(ifj18_stack_t *s, void *d) {
  stack_elem_t *new_element;
  if ((new_element = (stack_elem_t *)malloc(sizeof(struct stack_elem_t))) == NULL)
    error(INTERNAL_ERROR, "stack push error");

  /// insert new element on top of stack
  new_element->data = d;
  new_element->next_ptr = s->top_ptr;
  s->top_ptr = new_element;
}

void stack_pop(ifj18_stack_t *s) {

  if (s->top_ptr != NULL)
    s->top_ptr = s->top_ptr->next_ptr;
}

void *stack_top(ifj18_stack_t *s) {

  if (s->top_ptr != NULL)
    return s->top_ptr->data;
  else
    return NULL;
}

int stack_empty(ifj18_stack_t *s) { return (s->top_ptr != NULL ? 0 : -1); }

void stack_copy(ifj18_stack_t *dst_stack, ifj18_stack_t *src_stack) {

  while (!stack_empty(src_stack)) {
    stack_push(dst_stack, stack_top(src_stack));
    stack_pop(src_stack);
  }
}

void stack_print(ifj18_stack_t *s) {
  (void)s;
#ifdef DEBUG
  stack_elem_t *tmp = s->top_ptr;
  while (s->top_ptr != NULL) {
    printf("%d\n", *((int *)s->top_ptr->data));
    s->top_ptr = s->top_ptr->next_ptr;
  }
  s->top_ptr = tmp;
#endif
}