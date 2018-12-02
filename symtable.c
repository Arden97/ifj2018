#include "symtable.h"
#include "semantics.h"

/*
 * Set hash `key` to `val`.
 */

inline void ifj18_hash_set(khash_t(value) * self, char *key, ifj18_obj_t *val) {
  int ret;
  khiter_t k = kh_put(value, self, key, &ret);
  kh_value(self, k) = val;
}

/*
 * Get hash `key`, or NULL.
 */

inline ifj18_obj_t *ifj18_hash_get(khash_t(value) * self, char *key) {
  khiter_t k = kh_get(value, self, key);
  return k == kh_end(self) ? NULL : kh_value(self, k);
}

/*
 * Check if hash `key` exists.
 */

inline int ifj18_hash_has(khash_t(value) * self, char *key) {
  printf("inside of hash_has\n");

  khiter_t k = kh_get(value, self, key);

  if(k == NULL){
    return 0;
  }

  return kh_exist(self, k);
}

/*
 * Remove hash `key`.
 */

void ifj18_hash_remove(khash_t(value) * self, char *key) {
  khiter_t k = kh_get(value, self, key);
  kh_del(value, self, k);
}