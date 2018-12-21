///////////////////////////////////////////////////////////////////////////////////
// School:      Brno University of Technology, Faculty of Information Technology //
// Course:      Formal Languages and Compilers                                   //
// Project:     IFJ18                                                            //
// Module:      Header file of list 	                                           //
// Authors:     Artem Denisov       (xdenis00)                                   //
//              Volodymyr Piskun    (xpisku03)                                   //
///////////////////////////////////////////////////////////////////////////////////

typedef struct tListElement {
  void *val;
  struct tListElement *lptr;
  struct tListElement *rptr;

} * tListElementPtr;

typedef struct tList {
  tListElementPtr First;
  tListElementPtr Last;
  tListElementPtr Active;
} tList;

void *copy_last(tList *L);

void init_list(tList *L);
