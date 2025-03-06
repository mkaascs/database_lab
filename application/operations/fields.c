#include <string.h>

#include "fields.h"
#include "../bool.h"
#include "../domain/models.h"
#include "../parsing/parser.h"
#include "../domain/converting/converter.h"

void trim_quotation(const char* origin, char* result) {
    const int length = strlen(origin);

    int left_border = origin[0] == '\'' || origin[0] == '\"';
    int right_border = length - 1 - (origin[length - 1] == '\'' || origin[length - 1] == '\"');

    strncpy(result, origin + left_border, right_border - left_border + 1);
    result[right_border - left_border + 1] = '\0';
}

#pragma region Setters
bool set_pid(const char* value, ProcessInfo* record) {
    return string_to_int(value, &record->pid);
}

bool set_name(const char* value, ProcessInfo* record) {
    trim_quotation(value, record->name);
    return true;
}

bool set_priority(const char* value, ProcessInfo* record) {
    return string_to_int(value, &record->priority);
}

bool set_kern_tm(const char* value, ProcessInfo* record) {
    char time[24];
    trim_quotation(value, time);
    return string_to_time(time, &record->kern_tm);
}

bool set_file_tm(const char* value, ProcessInfo* record) {
    char time[24];
    trim_quotation(value, time);
    return string_to_time(time, &record->file_tm);
}

bool set_cpu_usage(const char* value, ProcessInfo* record) {
    return string_to_float(value, &record->cpu_usage);
}

bool set_status(const char* value, ProcessInfo* record) {
    return string_to_status(value, &record->status);
}
#pragma endregion

#pragma region Getters
void* get_pid(ProcessInfo* record) { return &record->pid; }
void* get_name(ProcessInfo* record) { return &record->name; }
void* get_priority(ProcessInfo* record) { return &record->priority; }
void* get_kern_tm(ProcessInfo* record) { return &record->kern_tm; }
void* get_file_tm(ProcessInfo* record) { return &record->file_tm; }
void* get_cpu_usage(ProcessInfo* record) { return &record->cpu_usage; }
void* get_status(ProcessInfo* record) { return &record->status; }
#pragma endregion

static bool (*setters[FIELDS_COUNT])(const char*, ProcessInfo*) = {
    set_pid, set_name, set_priority, set_kern_tm, set_file_tm, set_cpu_usage, set_status
};

static void* (*getters[FIELDS_COUNT])(ProcessInfo*) = {
    get_pid, get_name, get_priority, get_kern_tm, get_file_tm, get_cpu_usage, get_status
};

bool set_value(const char* field, const char* value, ProcessInfo* record) {
    for (int index = 0; index < FIELDS_COUNT; index++) {
        if (strcmp(field, fields_string[index]) == 0)
            return setters[index](value, record);
    }

    return false;
}

bool set_values(ParsedCommand command, ProcessInfo* record) {
    for (int index = 0; index < command.fields_count; index++) {
        for (int prev = 0; prev < index; prev++) {
            if (strcmp(command.fields[index].field, command.fields[prev].field) == 0)
                return false;
        }

        const bool success = set_value(command.fields[index].field, command.fields[index].value, record);
        if (!success) return false;
    }

    return true;
}

void* get_value(const char* field, ProcessInfo* record) {
    for (int index = 0; index < FIELDS_COUNT; index++) {
        if (strcmp(field, fields_string[index]) == 0)
            return getters[index](record);
    }

    return NULL;
}