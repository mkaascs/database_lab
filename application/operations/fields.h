#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "../domain/models.h"
#include "../parsing/parser.h"

typedef enum {
    Int, Float, Enum, String, Time
} ValueType;

typedef struct {
    void* value;
    ValueType type;
} FieldInfo;

int set_value(const char*, const char*, ProcessInfo*);
int set_values(ParsedCommand, ProcessInfo*);
int get_value(const char*, ProcessInfo*, FieldInfo*);

#endif //OPERATIONS_H
