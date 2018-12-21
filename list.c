///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      List implementation	                                             //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
///////////////////////////////////////////////////////////////////////////////////

#include "list.h"
#include <stdlib.h>

void *copy_last(tList *L) {

  if (L->Last != NULL) {
    return L->Last->val;
  } else {
    return NULL;
  }
}

void init_list(tList *L) {
  L->First = NULL;
  L->Last = NULL;
  L->Active = NULL;
}

void print_list(tList *L) {}
