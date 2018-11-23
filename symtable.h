#ifndef IFJ18_HASH_H
#define IFJ18_HASH_H

#include "khash.h"
#include "semantics.h"

KHASH_MAP_INIT_STR(value, ifj18_obj_t *);

/*
 * IFJ18 hash.
 */

typedef khash_t(value) ifj18_hash_t;

/*
 * Allocate a new hash.
 */

#define ifj18_hash_new() kh_init(value)

/*
 * Destroy the hash.
 */

#define ifj18_hash_destroy(self) kh_destroy(value, self)

/*
 * Hash size.
 */

#define ifj18_hash_size kh_size

    void
    ifj18_hash_set(khash_t(value) * self, char *key, ifj18_obj_t *val);

ifj18_obj_t *ifj18_hash_get(khash_t(value) * self, char *key);

int ifj18_hash_has(khash_t(value) * self, char *key);

void ifj18_hash_remove(khash_t(value) * self, char *key);

#endif