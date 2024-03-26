#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <glad/glad.h>
#include <limits.h>
#include <stdio.h>

// Hash Table (Linear probing) 
// Change to Red-black tree hash table or self balancing tree hash table in future

#define HT_LOG(x) fprintf(stderr, "[HASH_TABLE]: %s\n", x);
#define HASH_TABLE_NOT_FOUND INT_MAX

typedef struct entry_t {
    char *key;
    GLint value;
} entry_t;

typedef struct {
    size_t size;
    entry_t **entries; 
} hash_table_t;

hash_table_t *hash_table_init(int size);
void hash_table_destroy(hash_table_t *self);
void hash_table_insert(hash_table_t *self, char *key, GLint value);
void hash_table_delete(hash_table_t *self, char *key);
GLint hash_table_search(hash_table_t *self, char *key);
#endif

