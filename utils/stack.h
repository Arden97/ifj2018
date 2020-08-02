///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Header file of Stack implementation 	                           //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
///////////////////////////////////////////////////////////////////////////////////

#ifndef STACK_H
#define STACK_H

#include "../error/error.h"
#include "prettyprint.h"
#include "../scanner/scanner.h"
#include "../semantics/semantics.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct stack_elem_t {
  void *data;
  struct stack_elem_t *next_ptr;
} stack_elem_t;

typedef struct ifj18_stack_t {
  stack_elem_t *top_ptr;
} ifj18_stack_t;

ifj18_stack_t *stack_init();
void stack_push(ifj18_stack_t *s, void *data);
void stack_pop(ifj18_stack_t *s);
void *stack_top(ifj18_stack_t *s);
int stack_empty(ifj18_stack_t *s);
void stack_print(ifj18_stack_t *s);
void stack_copy(ifj18_stack_t *dst_stack, ifj18_stack_t *src_stack);
void stack_print_objects(ifj18_stack_t *s);

#endif