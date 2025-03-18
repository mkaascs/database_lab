#include <string.h>

#include "fields.h"
#include "../domain/converting/converter.h"

#pragma region Setters
int set_pid(const char* value, ProcessInfo* record) {
    return string_to_int(value, &record->pid);
}

int set_name(const char* value, ProcessInfo* record) {
    trim_quotation(value, record->name);
    return 1;
}

int set_priority(const char* value, ProcessInfo* record) {
    return string_to_int(value, &record->priority);
}

int set_kern_tm(const char* value, ProcessInfo* record) {
    return string_to_time(value, &record->kern_tm);
}

int set_file_tm(const char* value, ProcessInfo* record) {
    return string_to_time(value, &record->file_tm);
}

int set_cpu_usage(const char* value, ProcessInfo* record) {
    return string_to_float(value, &record->cpu_usage);
}

int set_status(const char* value, ProcessInfo* record) {
    return string_to_status(value, &record->status);
}
#pragma endregion

#pragma region Getters
void get_pid(ProcessInfo* record, FieldInfo* result) {
    result->value = &record->pid;
    result->type = Int;
}

void get_name(ProcessInfo* record, FieldInfo* result) {
    result->value = &record->name;
    result->type = String;
}

void get_priority(ProcessInfo* record, FieldInfo* result) {
    result->value = &record->priority;
    result->type = Int;
}

void get_kern_tm(ProcessInfo* record, FieldInfo* result) {
    result->value = &record->kern_tm;
    result->type = Time;
}

void get_file_tm(ProcessInfo* record, FieldInfo* result) {
    result->value = &record->file_tm;
    result->type = Time;
}

void get_cpu_usage(ProcessInfo* record, FieldInfo* result) {
    result->value = &record->cpu_usage;
    result->type = Float;
}

void get_status(ProcessInfo* record, FieldInfo* result) {
    result->value = &record->status;
    result->type = Enum;
}
#pragma endregion

static int (*setters[FIELDS_COUNT])(const char*, ProcessInfo*) = {
    set_pid, set_name, set_priority, set_kern_tm, set_file_tm, set_cpu_usage, set_status
};

static void (*getters[FIELDS_COUNT])(ProcessInfo*, FieldInfo*) = {
    get_pid, get_name, get_priority, get_kern_tm, get_file_tm, get_cpu_usage, get_status
};

int set_value(const char* field, const char* value, ProcessInfo* record) {
    for (int index = 0; index < FIELDS_COUNT; index++) {
        if (strcmp(field, fields_string[index]) == 0)
            return setters[index](value, record);
    }

    return 0;
}

int set_values(ParsedCommand command, ProcessInfo* record) {
    for (int index = 0; index < command.fields_count; index++) {
        for (int prev = 0; prev < index; prev++) {
            if (strcmp(command.fields[index].field, command.fields[prev].field) == 0)
                return 0;
        }

        const int success = set_value(command.fields[index].field, command.fields[index].value, record);
        if (!success)
            return 0;
    }

    return 1;
}

int get_value(const char* field, ProcessInfo* record, FieldInfo* result) {
    for (int index = 0; index < FIELDS_COUNT; index++) {
        if (strcmp(field, fields_string[index]) == 0) {
            getters[index](record, result);
            return 1;
        }
    }

    return 0;
}