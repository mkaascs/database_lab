#include <string.h>
#include <stdlib.h>

#include "commands.h"

#include "../memory/stats.h"
#include "domain/converting/converter.h"

typedef int bool;
#define true 1
#define false 0

#define MAX_FIELDS_COUNT 7

void trim_quotation(char* origin, char* result) {
    const int length = strlen(origin);

    int left_border = origin[0] == '\'' || origin[0] == '\"';
    int right_border = length - 1 - (origin[length - 1] == '\'' || origin[length - 1] == '\"');

    strncpy(result, origin + left_border, right_border - left_border + 1);
    result[right_border - left_border + 1] = '\0';
}

bool set_value(char* field, char* value, ProcessInfo* record) {
    if (strcmp(field, "pid") == 0)
        return string_to_int(value, &record->pid);

    if (strcmp(field, "name") == 0) {
        trim_quotation(value, record->name);
        return true;
    }

    if (strcmp(field, "priority") == 0)
        return string_to_int(value, &record->priority);

    if (strcmp(field, "kern_tm") == 0) {
        char time[10];
        trim_quotation(value, time);
        return string_to_time(time, &record->kern_tm);
    }

    if (strcmp(field, "file_tm") == 0) {
        char time[10];
        trim_quotation(value, time);
        return string_to_time(time, &record->file_tm);
    }

    if (strcmp(field, "cpu_usage") == 0)
        return string_to_float(value, &record->cpu_usage);

    if (strcmp(field, "status") == 0)
        return string_to_status(value, &record->status);

    return false;
}

bool set_values(ParsedCommand command, ProcessInfo* record) {
    for (int index = 0; index < command.fields_count; index++) {
        for (int prev = 0; prev < index; prev++) {
            if (strcmp(command.fields[index].field, command.fields[prev].field) == 0) {
                return false;
            }
        }

        set_value(command.fields[index].field, command.fields[index].value, record);
    }

    return true;
}

int insert_command(Database* database, ParsedCommand command) {
    if (command.type != Insert)
        return -1;

    if (command.fields_count != MAX_FIELDS_COUNT)
        return -1;

    ProcessInfo record;
    if (!set_values(command, &record))
        return -1;

    Node* node = (Node*)track_malloc(sizeof(Node));

    node->data = record;
    node->next = NULL;

    if (database->length == 0) {
        database->head = node;
        database->tail = node;
    }

    else {
        database->tail->next = node;
        database->tail = node;
    }

    database->length++;
    return 0;
}