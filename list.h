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
