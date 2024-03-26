#include "hashtable.h"
#include "util.h"

static inline size_t _hash(hash_table_t *self, char key[static 1]) {
    size_t idx = 0;
    for (unsigned int i = 0; i < strlen(key); i++) {
        idx += key[i];
    }
    return idx % self->size;
}

hash_table_t *hash_table_init(int size) {
    assert(size > 0);

    hash_table_t *table = malloc(sizeof(*table));
    table->size    = size;
    table->entries = malloc(size*sizeof(entry_t*));

    for (int i = 0; i < size; i++) {
        table->entries[i] = NULL;
    }
    /* memset(table->entries, NULL, size*sizeof(entry_t*)); */
    return table;
}

void hash_table_destroy(hash_table_t *self) {
    assert(self != NULL);

    for (unsigned int i = 0; i < self->size; i++) {
        entry_t *entry = self->entries[i];
        if (entry) free(entry);
    }
    free(self->entries);
    free(self);
}

void hash_table_insert(hash_table_t *self, char *key, GLint value) {
    assert(self != NULL);
    assert(key != NULL);

    if (hash_table_search(self, key) != HASH_TABLE_NOT_FOUND) {
        HT_LOG("the key is already in the table");
        return;
    }

    entry_t *entry = malloc(sizeof(*entry));
    entry->key   = key;
    entry->value = value;

    size_t idx = _hash(self, key);
    while (self->entries[idx] != NULL && idx < self->size) {
        idx++;
    }
    self->entries[idx] = entry;
}

void hash_table_delete(hash_table_t *self, char *key) {
   assert(self != NULL);
   assert(key != NULL);

    size_t idx = _hash(self, key);
    if (self->entries[idx] != NULL && strcmp(self->entries[idx]->key, key) != 0) {
        while (idx < self->size) {
            if (strcmp(self->entries[idx]->key, key) == 0) {
                free(self->entries[idx]);
                return;
            }
            idx++;
        }
    }

    HT_LOG("Not found any entries that equal to a given key");
}

GLint hash_table_search(hash_table_t *self, char *key) {
    assert(self != NULL);
    assert(key != NULL);

    size_t idx = _hash(self, key);
    if (self->entries[idx] == NULL || (self->entries[idx] != NULL && strcmp(self->entries[idx]->key, key) != 0)) {
        return HASH_TABLE_NOT_FOUND;
    }

    while (idx < self->size) {
        if (self->entries[idx] != NULL && strcmp(self->entries[idx]->key, key) == 0) {
            return self->entries[idx]->value;
        }
        idx++;
    }
    return HASH_TABLE_NOT_FOUND;
}
