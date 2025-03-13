#include "hash.h"

#include "../../../memory/stats.h"

HashTable* create_hash_table() {
    HashTable* table = track_malloc(sizeof(HashTable));
    table->buckets = track_calloc(HASH_SIZE, sizeof(HashNode*));
    table->size = HASH_SIZE;
    return table;
}

void free_hash_table(HashTable *table) {
    if (table == NULL) return;

    for (size_t index = 0; index < table->size; index++) {
        HashNode *hn = table->buckets[index];
        while (hn != NULL) {
            HashNode *temp = hn;
            hn = hn->next;

            if (temp->key != NULL) {
                track_free(temp->key);
                temp->key = NULL;
            }

            track_free(temp);
        }

        table->buckets[index] = NULL;
    }

    track_free(table->buckets);
    track_free(table);
}

unsigned int hash(const char *str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = (hash << 5) + hash + c;

    return hash % HASH_SIZE;
}