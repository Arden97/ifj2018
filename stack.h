#ifndef STACK_H
#define STACK_H

typedef struct stack_elem_t {
  void *data;                    ///< pointer to stack data
  struct stack_elem_t *next_ptr; ///< pointer to next item
} stack_elem_t;

typedef struct ifj18_stack_t {
  stack_elem_t *top_ptr; ///< pointer to the stack top
} ifj18_stack_t;

ifj18_stack_t *stack_init();
void stack_push(ifj18_stack_t *s, void *data);
void stack_pop(ifj18_stack_t *s);
void *stack_top(ifj18_stack_t *s);
int stack_empty(ifj18_stack_t *s);
void stack_print(ifj18_stack_t *s);
void stack_copy(ifj18_stack_t *dst_stack, ifj18_stack_t *src_stack);

#endif // STACK_Hifj18_