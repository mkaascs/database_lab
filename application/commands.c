#include <stdio.h>
#include <stdlib.h>

#include "../memory/stats.h"
#include "domain/hashing/hash.h"
#include "operations/conditions/conditions.h"
#include "operations/fields.h"
#include "commands.h"

#include <string.h>

#define MAX_FIELDS_COUNT 7
#define FLOAT_EPSILON 1e-6

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

int generate_key(Node* node, ParsedCommand command, char** result) {
    char buffer[MAX_FIELDS_COUNT * VALUE_LENGTH];
    for (int index = 0; index < command.fields_count; index++) {
        FieldInfo field;
        if (get_value(command.fields[index].field, &node->data, &field) == 0)
            return -1;

        if (field.type == Int || field.type == Enum)
            sprintf(buffer + strlen(buffer), "%d|", *(int*)field.value);

        else if (field.type == String)
            sprintf(buffer + strlen(buffer), "%s|", (char*)field.value);

        else if (field.type == Float)
            sprintf(buffer + strlen(buffer), "%.2f|", *(float*)field.value);

        else if (field.type == Time) {
            struct tm* time = (struct tm*)field.value;
            sprintf(buffer + strlen(buffer), "%02d:%02d:02d|",
                time->tm_hour, time->tm_min, time->tm_sec);
        }
    }

    *result = strdup(buffer);
    return 0;
}

int uniq_command(Database* database, ParsedCommand command, void (*presenter)(char*)) {
    for (int first = 0; first < command.fields_count; first++) {
        for (int second = first + 1; second < command.fields_count; second++) {
            if (strcmp(command.fields[first].field, command.fields[second].field) == 0)
                return -1;
        }
    }

    int removed = 0;
    HashTable* table = create_hash_table();
    Node* current = database->head;

    while (current != NULL) {
        char* key;
        if (generate_key(current, command, &key) == -1)
            return -1;

        unsigned int hashed = hash(key) % table->size;
        HashNode *hn = table->buckets[hashed];

        int found = 0;
        while (hn != NULL) {
            if (strcmp(hn->key, key) == 0) {
                Node *to_remove = hn->last_node;
                if (to_remove == database->head) {
                    database->head = to_remove->next;
                } else {
                    Node *tmp = database->head;
                    while (tmp->next != to_remove)
                        tmp = tmp->next;

                    tmp->next = to_remove->next;
                }

                track_free(to_remove);
                removed++;
                database->length--;
                hn->last_node = current;
                found = 1;
                break;
            }

            hn = hn->next;
        }

        if (!found) {
            HashNode* new_hn = (HashNode*)track_malloc(sizeof(HashNode));
            new_hn->key = key;
            new_hn->last_node = current;
            new_hn->next = table->buckets[hashed];
            table->buckets[hashed] = new_hn;
        } else {
            track_free(key);
        }

        current = current->next;
    }

    free_hash_table(table);
    char buffer[32];
    sprintf(buffer, "uniq:%d", removed);
    presenter(buffer);
    return 0;
}