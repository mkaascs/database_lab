#ifndef TYPES_H
#define TYPES_H

#include "../../../domain/models.h"

typedef enum {
    Equal, NotEqual, More, Less, MoreOrEqual, LessOrEqual, In, NotIn
} OperationType;

int get_operation_type(const char*, OperationType*);
int match_int(int, int, OperationType, int*);
int match_float(float, float, OperationType, int*);
int match_string(const char*, const char*, OperationType, int*);
int match_time(struct tm, struct tm, OperationType, int*);

#endif
