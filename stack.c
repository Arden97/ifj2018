///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Stack implementation 	                                           //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
///////////////////////////////////////////////////////////////////////////////////

#include "stack.h"
#include "error.h"
#include "prettyprint.h"
#include "scanner.h"
#include "semantics.h"
#include <stdio.h>
#include <stdlib.h>

#define DEBUG

ifj18_stack_t *stack_init() {
  ifj18_stack_t *s;
  if ((s = (ifj18_stack_t *)malloc(sizeof(ifj18_stack_t))) == NULL)
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
  ifj18_token_t *tokend;
  stack_elem_t *tmp = s->top_ptr;
  while (s->top_ptr != NULL) {
    debug_info("#STACK_SHIT:");
    token_prettyprint((ifj18_token_t *)s->top_ptr->data);
    s->top_ptr = s->top_ptr->next_ptr;
  }
  s->top_ptr = tmp;
#endif
}
void stack_print_objects(ifj18_stack_t *s) {
  (void)s;
#ifdef DEBUG
  ifj18_token_t *tokend;
  stack_elem_t *tmp = s->top_ptr;
  while (s->top_ptr != NULL) {
    debug_info("#STACK_SHIT Object:");
    debug_info("%d\n", ((ifj18_obj_t *)s->top_ptr->data)->obj_type.var.value.as_int);
    s->top_ptr = s->top_ptr->next_ptr;
  }
  s->top_ptr = tmp;
#endif
}
