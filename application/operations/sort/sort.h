#ifndef SORT_H
#define SORT_H

#include "../../domain/models.h"
#include "../../parsing/parser.h"

typedef struct {
    char field[FIELD_LENGTH];
    int ascending;
} SortField;

void merge_sort(Database*, SortField*, int);

#endif //SORT_H
