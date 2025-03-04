#include <string.h>
#include <stdlib.h>

#include "commands.h"

#include <stdio.h>

#include "domain/converting/converter.h"

typedef int bool;
#define true 1
#define false 0

#define MAX_FIELDS_COUNT 7

void trim_quotation(char* origin, char* result) {
    int left_border = 0;
    int right_border = strlen(origin) - 1;

    for (; origin[left_border] == '\'' || origin[left_border] == '\"'; left_border++) {}
    for (; origin[right_border] == '\'' || origin[right_border] == '\"'; right_border--) {}

    strncpy(result, origin + left_border, right_border - left_border + 1);
    result[right_border - left_border + 1] = '\0';
}

bool set_value(char* field, char* value, Record* record) {
    if (strcmp(field, "pid") == 0)
        return string_to_int(value, &record->pid);

    if (strcmp(field, "name") == 0) {
        trim_quotation(value, record->name);
        return true;
    }

    if (strcmp(field, "priority") == 0)
        return string_to_int(value, &record->priority);

    if (strcmp(field, "kern_tm") == 0) {
        char time[20];
        trim_quotation(value, time);
        return string_to_time(time, &record->kern_tm);
    }

    if (strcmp(field, "file_tm") == 0) {
        char time[20];
        trim_quotation(value, time);
        return string_to_time(time, &record->file_tm);
    }

    if (strcmp(field, "cpu_usage") == 0)
        return string_to_float(value, &record->cpu_usage);

    if (strcmp(field, "status") == 0)
        return string_to_status(value, &record->status);

    return false;
}

int insert_command(Node* database, ParsedCommand command) {
    if (command.type != Insert)
        return -1;

    if (command.fields_count != MAX_FIELDS_COUNT)
        return -1;

    Record record;
    for (int index = 0; index < command.fields_count; index++) {
        set_value(command.fields[index].field, command.fields[index].value, &record);
    }

    printf("cpu: %f\npid: %d\npriority: %d\nname: %s\nstatus: %d\nkern: %d:%d:%d\nfile: %d:%d:%d", record.cpu_usage, record.pid, record.priority, record.name, record.status,
        record.kern_tm.tm_hour, record.kern_tm.tm_min, record.kern_tm.tm_sec,
        record.file_tm.tm_hour, record.file_tm.tm_min, record.file_tm.tm_sec);

    return 0;
}