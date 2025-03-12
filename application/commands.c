#include <stdio.h>
#include <stdlib.h>

#include "../memory/stats.h"
#include "operations/conditions/conditions.h"
#include "operations/fields.h"
#include "commands.h"

#include <string.h>

#define MAX_FIELDS_COUNT 7

int insert_command(Database* database, ParsedCommand command, void (*presenter)(char*)) {
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

    char buffer[32];
    sprintf(buffer, "insert:%d", database->length);
    presenter(buffer);
    return 0;
}

int select_command(const Database* database, ParsedCommand command, void (*presenter)(char*)) {
    if (command.type != Select)
        return -1;

    Node* current = database->head;
    while (current != NULL) {
        int is_match = 1;
        for (int index = 0; index < command.conditions_count; index++) {
            const int code = match(command.conditions[index], current->data, &is_match);
            if (code == -1)
                return -1;

            if (!is_match)
                break;
        }

        if (!is_match)
            continue;

        char* line = (char*)track_malloc(command.fields_count * (FIELD_LENGTH + VALUE_LENGTH));
        for (int index = 0; index < command.fields_count; index++) {
            FieldInfo field;
            if (get_value(command.fields[index].field, &current->data, &field) == 0)
                return -1;

            const char buffer[FIELD_LENGTH + VALUE_LENGTH + 1];
            if (field.type == Time) {
                struct tm* value = (struct tm*)field.value;
                snprintf(buffer, sizeof(buffer), "%s='%02d:%02d:%02d' ",
                    command.fields[index].field,
                    value->tm_hour, value->tm_min, value->tm_sec);
            }

            else if (field.type == Enum) {
                Status* value = (Status*)field.value;
                snprintf(buffer, sizeof(buffer), "%s='%s' ",
                    command.fields[index].field,
                    status_string[*value]);
            }

            else if (field.type == Int) {
                int* value = (int*)field.value;
                snprintf(buffer, sizeof(buffer), "%s=%d ",
                    command.fields[index].field, *value);
            }

            else if (field.type == String) {
                char* value = (char*)field.value;
                snprintf(buffer, sizeof(buffer), "%s=\"%s\" ",
                    command.fields[index].field, value);
            }

            else if (field.type == Float) {
                float* value = (float*)field.value;
                snprintf(buffer, sizeof(buffer), "%s=%.2f ",
                    command.fields[index].field, *value);
            }

            strcat(line, buffer);
        }

        presenter(line);
        track_free(line);
        current = current->next;
    }

    return 0;
}