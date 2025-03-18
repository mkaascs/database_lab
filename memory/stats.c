#include "../memory/stats.h"
#include <stdlib.h>

DataStats data_stats = {0};

void* track_malloc(size_t data_size) {
    data_stats.malloc_state++;
    return malloc(data_size);
}

void* track_calloc(size_t num, size_t data_size) {
    data_stats.calloc_state++;
    return calloc(num, data_size);
}

void* track_realloc(void* pointer, size_t data_size) {
    data_stats.realloc_state++;
    return realloc(pointer, data_size);
}

void track_free(void* pointer) {
    if (!pointer) return;
    data_stats.free_count++;
    free(pointer);
}

void get_memstat(DataStats* memstat) {
    *memstat = data_stats;
}