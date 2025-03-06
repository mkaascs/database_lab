#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "../bool.h"
#include "../domain/models.h"
#include "../parsing/parser.h"

bool set_value(const char*, const char*, ProcessInfo*);
bool set_values(ParsedCommand, ProcessInfo*);
void* get_value(const char*, ProcessInfo*);

#endif //OPERATIONS_H
