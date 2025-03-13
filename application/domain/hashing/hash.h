#ifndef HASH_H
#define HASH_H

#include "../models.h"
#include <sys/_types/_size_t.h>

#define HASH_SIZE 1000

typedef struct HashNode {
    char *key;
    Node *last_node;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode **buckets;
    size_t size;
} HashTable;

HashTable* create_hash_table();
void free_hash_table(HashTable*);
unsigned int hash(const char*);

#endif //HASH_H
