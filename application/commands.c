#include <stdio.h>
#include <stdlib.h>

#include "../memory/stats.h"
#include "operations/conditions/conditions.h"
#include "operations/fields.h"
#include "commands.h"

#include <string.h>

#define MAX_FIELDS_COUNT 7

int insert_command(Database* database, ParsedCommand command, void (*presenter)(char*)) {
    if (command.type != Insert || command.fields_count != MAX_FIELDS_COUNT)
        return -1;

    ProcessInfo record;
    if (!set_values(command, &record))
        return -1;

    Node* node = (Node*)track_malloc(sizeof(Node));

    node->data = record;
    node->next = NULL;

    if (database->length == 0) {
        node->previous = NULL;
        database->head = node;
        database->tail = node;
    }

    else {
        node->previous = database->tail;
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
        int conditions_met = 1;
        for (int index = 0; index < command.conditions_count; index++) {
            const int code = match(command.conditions[index], current->data, &conditions_met);
            if (code == -1)
                return -1;

            if (!conditions_met)
                break;
        }

        if (!conditions_met) {
            current = current->next;
            continue;
        }

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
                int* value = (int*)field.value;
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

int uniq_command(Database* database, ParsedCommand command, void (*presenter)(char*)) {
    if (command.type != Uniq || command.fields_count == 0 || command.conditions_count != 0)
        return -1;

    int removed = 0;
    Node* current = database->tail;

    while (current != NULL) {
        Node* runner = current->previous;
        while (runner != NULL) {
            int is_different = 0;
            for (int index = 0; index < command.fields_count; index++) {
                FieldInfo first, second;
                if (!get_value(command.fields[index].field, &current->data, &first))
                    return -1;

                if (!get_value(command.fields[index].field, &runner->data, &second))
                    return -1;

                if (first.type == Int || first.type == Enum)
                    is_different = *(int*)first.value != *(int*)second.value;

                if (first.type == Float)
                    is_different = *(float*)first.value != *(float*)second.value;

                if (first.type == String)
                    is_different = strncmp((char*)first.value, (char*)second.value, VALUE_LENGTH) != 0;

                if (first.type == Time)
                    is_different = memcmp(first.value, second.value, sizeof(struct tm)) != 0;

                if (is_different) break;
            }

            if (is_different) {
                runner = runner->previous;
                continue;
            }

            Node* to_delete = runner;
            runner = runner->previous;

            if (to_delete->previous != NULL)
                to_delete->previous->next = to_delete->next;

            else database->head = to_delete->next;

            if (to_delete->next != NULL)
                to_delete->next->previous = to_delete->previous;

            else database->tail = to_delete->previous;

            track_free(to_delete);
            removed++;
            database->length--;
        }

        current = current->previous;
    }

    char buffer[32];
    sprintf(buffer, "uniq:%d", removed);
    presenter(buffer);
    return 0;
}